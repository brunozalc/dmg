#include "ppu.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "mmu.h"

#define CYCLES_PER_SCANLINE 456
#define CYCLES_OAM_SCAN 80
#define CYCLES_VBLANK_SCANLINE 456  // each of the 10 VBLANK scanlines takes a full scanline time
#define SCANLINES_PER_FRAME 154     // 144 visible scanlines + 10 VBLANK scanlines

/* placeholder for drawing/rendering cycles, but this can be variable */
#define CYCLES_DRAWING_AVG 172  // usually between 172 and 289 dots

/* interrupt types */
#define VBLANK_INTERRUPT 0
#define LCD_INTERRUPT 1

/* function to initialize the PPU */
void ppu_init(PPU *ppu, struct MMU *mmu, struct CPU *cpu) {
    ppu->mmu = mmu;
    ppu->cpu = cpu;

    /* reset the PPU state */
    ppu_reset(ppu);
}

/* function to reset the PPU */
void ppu_reset(PPU *ppu) {
    /* reset the PPU registers */
    ppu->lcdc = 0;
    ppu->stat = 0;
    ppu->scy  = 0;
    ppu->scx  = 0;
    ppu->lyc  = 0;
    ppu->bgp  = 0;

    /* reset the framebuffer */
    memset(ppu->framebuffer, 0, sizeof(ppu->framebuffer));

    /* reset the PPU state */
    ppu->scanline_cycles  = 0;
    ppu->current_scanline = 0;
    ppu->mode             = PPU_MODE_OAM_SEARCH;
    ppu->frame_completed  = 0;

    /* initialize the PPU I/O registers */
    uint8_t stat          = mmu_read(ppu->mmu, STAT);
    stat                  = (stat & 0xFC) | (ppu->mode);  // clear the mode bits

    mmu_write(ppu->mmu, LY, 0);       // LY register is initialized to 0
    mmu_write(ppu->mmu, STAT, stat);  // write the initial state to STAT
}

/* internal helper functions */
static inline void request_interrupt(PPU *ppu, int interrupt_type) {
    if (interrupt_type == VBLANK_INTERRUPT) {
        ppu->cpu->ifr |= 0x01;  // set bit 0 of IF register
    } else if (interrupt_type == LCD_INTERRUPT) {
        ppu->cpu->ifr |= 0x02;  // set bit 1 of IF register
    }
}

static void check_lyc_match(PPU *ppu) {
    uint8_t stat = mmu_read(ppu->mmu, STAT);
    uint8_t lyc  = mmu_read(ppu->mmu, LYC);

    if (ppu->current_scanline == lyc) {
        stat |= 0x04;  // set the LYC=LY flag
        if (stat & 0x40) {
            /* if the LYC=LY interrupt is enabled, request an interrupt */
            request_interrupt(ppu, LCD_INTERRUPT);
        }
    } else {
        stat &= ~0x04;  // clear the LYC=LY flag
    }
    mmu_write(ppu->mmu, STAT, stat);  // write the updated status back
}

static void change_mode(PPU *ppu, ppu_mode new_mode) {
    ppu->mode    = new_mode;
    uint8_t stat = mmu_read(ppu->mmu, STAT);
    stat &= 0xFC;      // clear the mode bits
    stat |= new_mode;  // set the new mode

    /* check for STAT interrupt triggers on mode change */
    int interrupt_requested = 0;
    if (new_mode == PPU_MODE_OAM_SEARCH && (stat & 0x20)) {
        interrupt_requested = 1;  // OAM search mode interrupt enabled
    } else if (new_mode == PPU_MODE_VBLANK && (stat & 0x10)) {
        interrupt_requested = 1;  // VBLANK mode interrupt enabled
    } else if (new_mode == PPU_MODE_HBLANK && (stat & 0x08)) {
        interrupt_requested = 1;  // HBLANK mode interrupt enabled
    }

    if (interrupt_requested) {
        request_interrupt(ppu, LCD_INTERRUPT);  // request an interrupt
    }

    mmu_write(ppu->mmu, STAT, stat);  // write the updated status back
}

static void render_background_in_scanline(PPU *ppu) {
    uint8_t lcdc = mmu_read(ppu->mmu, LCDC);
    if (!(lcdc & 0x01)) {  // bit 0: BG display enable. fill with white if disabled
        for (int x = 0; x < LCD_WIDTH; x++) {
            ppu->framebuffer[ppu->current_scanline][x] = COLOR_WHITE;  // white
        }
        return;
    }

    uint8_t scx                     = mmu_read(ppu->mmu, SCX);
    uint8_t scy                     = mmu_read(ppu->mmu, SCY);
    uint8_t bgp                     = mmu_read(ppu->mmu, BGP);

    /* get general info about the tiles */
    uint16_t tile_map_base_address  = (lcdc & 0x08) ? 0x9C00 : 0x9800;
    uint16_t tile_data_base_address = (lcdc & 0x10) ? 0x8000 : 0x9000;
    int signed_tile_data            = (lcdc & 0x10) ? 0 : 1;  // signed or unsigned tile data

    /* for each pixel in current scanline */
    for (int x = 0; x < LCD_WIDTH; x++) {
        /* calculate the tile index and pixel index */
        uint8_t y                 = (ppu->current_scanline + scy) & 0xFF;
        uint8_t xw                = (x + scx) & 0xFF;

        int tile_x                = (xw >> 3);
        int tile_y                = (y >> 3);
        int pixel_x               = xw % 8;
        int pixel_y               = y % 8;

        /* get the tile index from the tile map */
        uint16_t tile_map_address = tile_map_base_address + tile_y * 32 + tile_x;
        uint8_t tile_index        = mmu_read(ppu->mmu, tile_map_address);

        /* get the tile data */
        uint16_t tile_data_address =
            tile_data_base_address + (signed_tile_data ? (int8_t)tile_index : tile_index) * 16;
        uint8_t low_byte  = mmu_read(ppu->mmu, tile_data_address + pixel_y * 2);
        uint8_t high_byte = mmu_read(ppu->mmu, tile_data_address + pixel_y * 2 + 1);

        /* get the color index from the low and high bytes */
        uint8_t color_index =
            ((high_byte >> (7 - pixel_x)) & 0x01) | (((low_byte >> (7 - pixel_x)) & 0x01) << 1);

        /* set the color in the framebuffer */
        ppu->framebuffer[ppu->current_scanline][x] =
            (bgp >> (color_index * 2)) & 0x03;  // get the color from BGP
    }
}

static void render_sprites_in_scanline(PPU *ppu) {
    /* TODO: implement sprite rendering (phase 2).
     1. scan OAM for up to 10 sprites in this scanline
     2. sort by x-coordinate (or OAM index)
     3. fetch tile data for each sprite
     4. conmbine with background data considering priority and transparency
     5. apply sprite palette to the sprite data and write to framebuffer
     */

    return;
}

/* function to update the PPU state */
void ppu_step(PPU *ppu, int cycles) {
    uint8_t lcdc = mmu_read(ppu->mmu, LCDC);

    // if LCD is disabled
    if (!(lcdc & 0x01)) {
        if (ppu->current_scanline != 0 || ppu->mode != PPU_MODE_VBLANK) {
            // when LCD is disabled, LY is reset to 0 and PPU enters VBLANK-like state
            ppu->current_scanline = 0;
            ppu->scanline_cycles  = 0;
            mmu_write(ppu->mmu, LY, 0);
            ppu->mode = PPU_MODE_VBLANK;
        }
        return;  // if LCD is off, PPU is mostly idle
    }

    // if LCD is enabled
    ppu->scanline_cycles += cycles;

    switch (ppu->mode) {
        case PPU_MODE_OAM_SEARCH:  // mode 2
            if (ppu->scanline_cycles >= CYCLES_OAM_SCAN) {
                ppu->scanline_cycles -= CYCLES_OAM_SCAN;
                change_mode(ppu, PPU_MODE_DRAWING);
                // TODO: perform OAM search here
            }
            break;

        case PPU_MODE_DRAWING:  // mode 3
            // drawing cycles are actually variable, and should be calculated based on sprite data
            // TODO: change PPU to use variable drawing cycles instead of an average number
            if (ppu->scanline_cycles >= CYCLES_DRAWING_AVG) {
                ppu->scanline_cycles -= CYCLES_DRAWING_AVG;

                if (ppu->current_scanline < LCD_HEIGHT) {
                    render_background_in_scanline(ppu);
                    // render_window_in_scanline(ppu); (TODO)
                    // render_sprites_in_scanline(ppu); (TODO)
                }
                change_mode(ppu, PPU_MODE_HBLANK);
            }
            break;

        case PPU_MODE_HBLANK:  // mode 0
            // uses remaining cycles
            if (ppu->scanline_cycles >=
                (CYCLES_PER_SCANLINE - CYCLES_OAM_SCAN - CYCLES_DRAWING_AVG)) {
                ppu->scanline_cycles -=
                    (CYCLES_PER_SCANLINE - CYCLES_OAM_SCAN - CYCLES_DRAWING_AVG);

                ppu->current_scanline++;
                mmu_write(ppu->mmu, LY, ppu->current_scanline);
                check_lyc_match(ppu);  // check if LYC matches LY

                if (ppu->current_scanline == LCD_HEIGHT) {
                    // if we reach the end of the visible scanlines, enter VBLANK
                    change_mode(ppu, PPU_MODE_VBLANK);
                    request_interrupt(ppu, VBLANK_INTERRUPT);  // request VBLANK interrupt
                    ppu->frame_completed = 1;                  // mark the scanline as completed
                } else {
                    change_mode(ppu, PPU_MODE_OAM_SEARCH);  // go to OAM search mode
                }
            }
            break;

        case PPU_MODE_VBLANK:  // mode 1
            // VBLANK lasts for 10 scanlines, each taking a full scanline time
            if (ppu->scanline_cycles >= CYCLES_VBLANK_SCANLINE) {
                ppu->scanline_cycles -= CYCLES_VBLANK_SCANLINE;
                ppu->current_scanline++;
                mmu_write(ppu->mmu, LY, ppu->current_scanline);
                check_lyc_match(ppu);  // check if LYC matches LY

                if (ppu->current_scanline >= SCANLINES_PER_FRAME) {
                    // if we reach the end of the VBLANK period, reset to the first scanline
                    ppu->current_scanline = 0;
                    mmu_write(ppu->mmu, LY, 0);
                    check_lyc_match(ppu);
                    change_mode(ppu, PPU_MODE_OAM_SEARCH);  // go to OAM search mode
                }
            }
            break;
    }
}

/* function to get the current framebuffer data */
const uint8_t (*ppu_get_framebuffer(PPU *ppu))[LCD_WIDTH] {
    return (const uint8_t (*)[LCD_WIDTH])ppu->framebuffer;
}
