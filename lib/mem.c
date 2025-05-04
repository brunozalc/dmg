#include "mem.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

// declare the ram space
uint8_t ram[0x10000];

void
mem_reset (void)
{
    memset (ram, 0, sizeof (ram));
}

uint8_t
mem_read (uint16_t addr)
{
    return ram[addr];
}

void
mem_write (uint16_t addr, uint8_t value)
{
    ram[addr] = value;
}
