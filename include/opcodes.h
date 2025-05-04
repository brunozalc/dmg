#ifndef OPCODES_HEADER
#define OPCODES_HEADER

#include <stdint.h>

#include "cpu.h"

// opfn is a function pointer that takes a CPU struct as an argument
// this means that we can create an array of opfn-type functions, simplifying
// the opcode table
typedef void (*opfn)(CPU*);

// opcode table
// this is a 256 element array of opfn functions
extern const opfn optable[256];

// helper to advance the program counter
static void inline advance_pc(CPU* cpu, uint8_t n) { cpu->pc += n; }

// helper to advance the cycles
static void inline advance_cycles(CPU* cpu, uint8_t n) { cpu->cycles += n; }

#endif
