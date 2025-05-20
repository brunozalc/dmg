#include "mmu.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"

void mmu_init(MMU *mmu, struct CPU *cpu, struct Timer *timer) {
    mmu->cpu   = cpu;
    mmu->timer = timer;
    mmu_reset(mmu);
}

void mmu_reset(MMU *mmu) {
    memset(mmu->rom, 0, sizeof(mmu->rom));
    memset(mmu->vram, 0, sizeof(mmu->vram));
    memset(mmu->eram, 0, sizeof(mmu->eram));
    memset(mmu->wram, 0, sizeof(mmu->wram));
    memset(mmu->oam, 0, sizeof(mmu->oam));
    memset(mmu->io, 0, sizeof(mmu->io));
    memset(mmu->hram, 0, sizeof(mmu->hram));
}

uint8_t mmu_read(MMU *mmu, uint16_t addr) {
    if (addr < 0x8000) {
        return mmu->rom[addr]; /* read from ROM */
    } else if (addr < 0xA000) {
        return mmu->vram[addr - 0x8000]; /* read from VRAM */
    } else if (addr < 0xC000) {
        return mmu->eram[addr - 0xA000]; /* read from ERAM */
    } else if (addr < 0xE000) {
        return mmu->wram[addr - 0xC000]; /* read from WRAM */
    } else if (addr < 0xFE00) {
        return mmu->wram[addr - 0xE000]; /* read from WRAM */
    } else if (addr < 0xFEA0) {
        return mmu->oam[addr - 0xFE00]; /* read from OAM */
    } else if (addr < 0xFF00) {
        return 0xFF; /* prohibited area */
    } else if (addr < 0xFF80) {
        switch (addr) {
            case 0xFF04: return mmu->timer->div >> 8;          /* DIV register */
            case 0xFF05: return mmu->timer->tima;              /* TIMA register */
            case 0xFF06: return mmu->timer->tma;               /* TMA register */
            case 0xFF07: return mmu->timer->tac;               /* TAC register */
            case 0xFF0F: return (mmu->cpu->ifr & 0x1F) | 0xE0; /* IFR register */
            case 0xFF44: return 0x90;                          /* gameboy-doctor helper */
            case 0xFF4D:                                       /* undocumented read */
            case 0xFF56: return 0xFF;
            default:     return mmu->io[addr - 0xFF00]; /* read from other IO registers */
        }
    } else if (addr < 0xFFFF) {
        return mmu->hram[addr - 0xFF80]; /* read from HRAM */
    } else {
        return (mmu->cpu->ier & 0x1F) | 0xE0; /* IER register */
    }
}

uint16_t mmu_read16(MMU *mmu, uint16_t addr) {
    uint8_t low  = mmu_read(mmu, addr);     /* read the low byte */
    uint8_t high = mmu_read(mmu, addr + 1); /* read the high byte */
    return (high << 8) | low;               /* combine the two bytes */
}

void mmu_write(MMU *mmu, uint16_t addr, uint8_t value) {
    if (addr < 0x8000) {
        /* no bank switching, rom is read-only */
        return;
    } else if (addr < 0xA000) {
        mmu->vram[addr - 0x8000] = value; /* write to VRAM */
        return;
    } else if (addr < 0xC000) {
        mmu->eram[addr - 0xA000] = value; /* write to ERAM */
        return;
    } else if (addr < 0xE000) {
        mmu->wram[addr - 0xC000] = value; /* write to WRAM */
        return;
    } else if (addr < 0xFE00) {
        mmu->wram[addr - 0xE000] = value; /* write to WRAM */
        return;
    } else if (addr < 0xFEA0) {
        mmu->oam[addr - 0xFE00] = value; /* write to OAM */
        return;
    } else if (addr < 0xFF00) {
        return; /* prohibited area */
    } else if (addr < 0xFF80) {
        switch (addr) {
            case 0xFF02:                    /* blargg test serial output */
                mmu->io[0x02] = value;      /* write to SC register */
                if (value & 0x80) {         /* start-transfer bit set? */
                    putchar(mmu->io[0x01]); /* dump the byte written to SB */
                    fflush(stdout);
                }
                mmu->io[0x02] &= ~0x80; /* clear bit 7: transfer complete */
                break;
            case 0xFF04: timer_write_div(mmu->timer); break;         /* reset the DIV register */
            case 0xFF05: timer_write_tima(mmu->timer, value); break; /* TIMA register */
            case 0xFF06: timer_write_tma(mmu->timer, value); break;  /* TMA register */
            case 0xFF07: timer_write_tac(mmu->timer, value); break;  /* TAC register */
            case 0xFF0F: mmu->cpu->ifr = value & 0x1F; break;        /* IFR register */
            default:     mmu->io[addr - 0xFF00] = value; break; /* write to other IO registers */
        }
        return;
    } else if (addr < 0xFFFF) {
        mmu->hram[addr - 0xFF80] = value; /* write to HRAM */
        return;
    } else {                          /* addr == 0xFFFF */
        mmu->cpu->ier = value & 0x1F; /* write to IER register */
    }
}

void mmu_write16(MMU *mmu, uint16_t addr, uint16_t value) {
    mmu_write(mmu, addr, value & 0xFF);     /* write the low byte */
    mmu_write(mmu, addr + 1, (value >> 8)); /* write the high byte */
}
