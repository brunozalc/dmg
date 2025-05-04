#ifndef CPU_HEADER
#define CPU_HEADER

#include <stdint.h>

typedef struct {
    // zero flag
    uint8_t z : 1;

    // subtract flag
    uint8_t n : 1;

    // half carry flag
    uint8_t h : 1;

    // carry flag
    uint8_t c : 1;

    // unused bits
    uint8_t unused : 4;
} Flags;

typedef struct {
    // a: accumulator; f: flags
    union {
        struct {
            uint8_t f, a;
        };
        uint16_t af;
    };

    union {
        struct {
            uint8_t c, b;
        };
        uint16_t bc;
    };

    union {
        struct {
            uint8_t e, d;
        };
        uint16_t de;
    };

    union {
        struct {
            uint8_t l, h;
        };
        uint16_t hl;
    };

    // stack pointer and program counter are both 16 bits
    uint16_t sp, pc;

    // cpu cycle counter
    uint64_t cycles;

    // flags
    Flags flags;

    // last opcode executed (debugging)
    uint8_t last_opcode;

} CPU;

void cpu_reset(CPU *cpu);
void cpu_step(CPU *cpu);

#endif
