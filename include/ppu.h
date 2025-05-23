#ifndef PPU_HEADER
#define PPU_HEADER

#include <stdint.h>
#include <stdio.h>

#define LCD_HEIGHT 144
#define LCD_WIDTH 160

/* display colors */
#define COLOR_WHITE 0
#define COLOR_LIGHT_GRAY 1
#define COLOR_DARK_GRAY 2
#define COLOR_BLACK 3

/* I/O registers addresses */
#define LCDC 0xFF40  // LCD control
#define STAT 0xFF41  // LCD status
#define SCY 0xFF42   // scroll Y
#define SCX 0xFF43   // scroll X
#define LY 0xFF44    // current scanline
#define LYC 0xFF45   // LY compare
#define BGP 0xFF47   // background palette

/* CPU and MMU forward declarations */
struct CPU;
struct MMU;

typedef enum {
    PPU_MODE_HBLANK = 0,
    PPU_MODE_VBLANK = 1,
    PPU_MODE_OAM_SEARCH = 2,
    PPU_MODE_DRAWING = 3,
} ppu_mode;

typedef struct PPU {
    struct MMU *mmu;
    struct CPU *cpu;

    /* PPU timing and states */
    int scanline_cycles;       // how many cycles in the current scanline
    uint8_t current_scanline;  // current scanline (LY register, 0-153)
    ppu_mode mode;             // current PPU mode

    /* framebuffer */
    uint8_t framebuffer[LCD_HEIGHT]
                       [LCD_WIDTH];  // framebuffer for the LCD
                                     // each pixel is a 0-3 shade of gray

    /* PPU registers (mirrors to MMU I/O registers) */
    uint8_t lcdc;  // 0xFF40 (LCD control)
    uint8_t stat;  // 0xFF41 (LCD status)
    uint8_t scy;   // 0xFF42 (scroll Y)
    uint8_t scx;   // 0xFF43 (scroll X)
    uint8_t lyc;   // 0xFF45 (LYC, compare register)
    uint8_t bgp;   // 0xFF47 (background palette)

    int frame_completed;  // flag to indicate if the frame (all scanlines) is
                          // completed

} PPU;

void ppu_init(PPU *ppu, struct MMU *mmu, struct CPU *cpu);
void ppu_reset(PPU *ppu);
void ppu_step(PPU *ppu, int cycles);

/* helper to get current framebuffer data and pass it to the main game loop */
const uint8_t (*ppu_get_framebuffer(PPU *ppu))[LCD_WIDTH];

#endif
