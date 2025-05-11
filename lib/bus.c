#include "bus.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

// declare the ram space
uint8_t ram[0x10000];

void mem_reset(void) { memset(ram, 0, sizeof(ram)); }

uint8_t mem_read(uint16_t addr) {
    /* gameboy-doctor helper
   - if the address read is 0xFF44, return the value of 0x9Ω
    */
    if (addr == 0xFF44) {
        return 0x90;
    }

    return ram[addr];
}

uint16_t mem_read16(uint16_t addr) {
    /* gameboy-doctor helper
   - if the address read is 0xFF44, return the value of 0x9Ω
    */
    if (addr == 0xFF44) {
        return 0x90;
    }

    return (ram[addr + 1] << 8) | ram[addr];
}

void mem_write(uint16_t addr, uint8_t value) {
    ram[addr] = value;

    /* blargg test serial output */
    if (addr == 0xFF02 && (value & 0x80)) { /* start-transfer bit set? */
        putchar(ram[0xFF01]);               /* dump the byte written to SB */
        fflush(stdout);

        ram[0xFF02] &= ~0x80; /* clear bit 7: transfer complete */
    }
}
