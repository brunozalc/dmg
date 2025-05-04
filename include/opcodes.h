#ifndef OPCODES_HEADER
#define OPCODES_HEADER

#include "cpu.h"

// opfn is a function pointer that takes a CPU pointer as an argument
// this means that we can create an array of opfns, simplifying the
// opcode table
typedef void (*opfn)(CPU*);

// opcode table
// this is a 256 element array of opfn functions
extern const opfn optable[256];

#endif
