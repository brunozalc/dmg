#ifndef BUS_HEADER
#define BUS_HEADER

#include <stdint.h>

// memory bus read a 16bit address and returns a 8bit value
uint8_t bus_read(uint16_t addr);

// likewise, we can write a 8bit value into a 16bit address
void bus_write(uint16_t addr, uint8_t value);

#endif
