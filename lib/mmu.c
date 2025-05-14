#include "mmu.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"

void mmu_init(MMU *mmu, struct CPU *cpu) {
    mmu->cpu = cpu;
    mmu_reset(mmu);
}

void mmu_reset(MMU *mmu) { memset(mmu->ram, 0, sizeof(mmu->ram)); }

uint8_t mmu_read(MMU *mmu, uint16_t addr) {
    /* gameboy-doctor helper
   - if the address read is 0xFF44, return the value of 0x90
    */
    if (addr == 0xFF44) {
        return 0x90;
    }

    /* ier and if MMIO handling */
    if (addr == 0xFF0F)
        return (mmu->cpu->ifr & 0x1F) | 0xE0;
    if (addr == 0xFFFF)
        return (mmu->cpu->ier & 0x1F) | 0xE0;

    return mmu->ram[addr];
}

uint16_t mmu_read16(MMU *mmu, uint16_t addr) {
    /* gameboy-doctor helper
   - if the address read is 0xFF44, return the value of 0x9Ω
    */
    if (addr == 0xFF44) {
        return 0x90;
    }

    return (mmu->ram[addr + 1] << 8) | mmu->ram[addr];
}

void mmu_write(MMU *mmu, uint16_t addr, uint8_t value) {
    mmu->ram[addr] = value;

    /* blargg test serial output */
    if (addr == 0xFF02 && (value & 0x80)) { /* start-transfer bit set? */
        putchar(mmu->ram[0xFF01]);          /* dump the byte written to SB */
        fflush(stdout);

        mmu->ram[0xFF02] &= ~0x80; /* clear bit 7: transfer complete */
    }

    /* ier and ifr MMIO handling */
    if (addr == 0xFFFF) {
        mmu->cpu->ier = value & 0x1F; /* only the lower 5 bits are used */
    } else if (addr == 0xFF0F) {
        mmu->cpu->ifr = value & 0x1F; /* only the lower 5 bits are used */
    }
}

void mmu_write16(MMU *mmu, uint16_t addr, uint16_t value) {
    mmu->ram[addr]     = value & 0xFF;
    mmu->ram[addr + 1] = (value >> 8) & 0xFF;
}
