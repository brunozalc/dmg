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
static inline void advance_pc(CPU* cpu, uint8_t n) { cpu->pc += n; }

// helper to advance the cycles
static inline void advance_cycles(CPU* cpu, uint8_t n) { cpu->cycles += n; }

// flags mask
#define FLAG_Z                                               \
    0x80 /* zero flag: if the result of an operation is zero \
            the mask 0x80 is 1000 0000 in binary */
#define FLAG_N                                                     \
    0x40 /* subtract flag: if the last operation was a subtraction \
            the mask 0x40 is 0100 0000 in binary */
#define FLAG_H                                                    \
    0x20 /* half carry flag: indicates carry for the lower 4 bits \
    the mask 0x20 is 0010 0000 in binary */
#define FLAG_C                                                              \
    0x10 /* carry flag: when the result of an 8-bit addition is higher than \
    $FF the mask 0x10 is 0001 0000 in binary */

/* helper to get a flag */
static inline int get_flag(CPU* cpu, uint8_t flag) {
    return (cpu->f & flag) != 0;
}

/* helper to set a flag */
static inline void set_flag(CPU* cpu, uint8_t flag, int enable) {
    if (enable) {
        cpu->f |= flag; /* if enable = 1, turn the flag bit to 1 */
    } else {
        cpu->f &= ~flag; /* if enable = 0, turn the flag bit to 0 */
    }
}

#endif
