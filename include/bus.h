#ifndef MEM_HEADER
#define MEM_HEADER

#include <stdint.h>

// declare the RAM as a big 64KB space
extern uint8_t ram[0x10000];

// reset the memory bus, setting all values to 0
void mem_reset(void);

// read a 8bit value from the memory bus
// 8-bit value = ram[addr]
uint8_t mem_read(uint16_t addr);

// read a 16bit value from the memory bus
// 16-bit value = ram[addr] + (ram[addr+1] << 8)
uint16_t mem_read16(uint16_t addr);

// write a 8bit value to the memory bus
// ram[addr] =  8-bit value
void mem_write(uint16_t addr, uint8_t value);

#endif
