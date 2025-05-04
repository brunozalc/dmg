#ifndef MEM_HEADER
#define MEM_HEADER

#include <stdint.h>

// declare the RAM as a big 64KB space
extern uint8_t ram[0x10000];

// memory bus reads a 16bit address from CPU and returns a 8bit value
uint8_t mem_read(uint16_t addr);

// likewise, we can write a 8bit value into a 16bit address
void mem_write(uint16_t addr, uint8_t value);

// resets the ram space to zero
void mem_reset(void);

#endif
