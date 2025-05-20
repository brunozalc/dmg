#ifndef CPU_HEADER
#define CPU_HEADER

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mmu.h"
#include "timer.h"

extern FILE *cpu_log;

struct MMU;
struct Timer;

typedef struct CPU {
    struct MMU *mmu;     /* pointer to the MMU */
    struct Timer *timer; /* pointer to the timer */

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

    // interrupt enable register
    int ime;       /* 0 or 1, current state (READ ONLY) */
    int ime_delay; /* two cycle ime delay  */

    /* 0xFF0F, interrupt flag register
    - bit 0: v-blank
    - bit 1: LCD
    - bit 2: timer
    - bit 3: serial
    - bit 4: joypad
    any bits set to 1 are interrupt requests
    */
    uint8_t ifr;

    /* 0xFFFF, interrupt enable register
    - bit 0: v-blank
    - bit 1: LCD
    - bit 2: timer
    - bit 3: serial
    - bit 4: joypad
    any bits set to 1 are enabled interrupts
    */
    uint8_t ier;

    /* HALT flag */
    int halt;     /* 0 or 1, current state */
    int halt_bug; /* 0 or 1, double read bug */

    // last opcode executed (debugging)
    uint8_t last_opcode;

} CPU;

void cpu_init(CPU *cpu, MMU *mmu, Timer *timer);
void cpu_step(CPU *cpu);

/* function to log CPU errors with useful information */
// static inline void log_cpu_error(CPU *cpu, const char *format, ...) {
//     va_list args;
//     va_start(args, format);

//     fprintf(stderr, "\n=== error ===\n");
//     fprintf(stderr, "pc: 0x%04X\n", cpu->pc);
//     fprintf(stderr, "opcode: 0x%02X\n", cpu->last_opcode);
//     fprintf(stderr, "error: ");
//     vfprintf(stderr, format, args);
//     fprintf(stderr, "\n");

//     // print CPU state
//     fprintf(stderr, "\nCPU state:\n");
//     fprintf(stderr, "A: 0x%02X  F: 0x%02X\n", cpu->a, cpu->f);
//     fprintf(stderr, "B: 0x%02X  C: 0x%02X\n", cpu->b, cpu->c);
//     fprintf(stderr, "D: 0x%02X  E: 0x%02X\n", cpu->d, cpu->e);
//     fprintf(stderr, "H: 0x%02X  L: 0x%02X\n", cpu->h, cpu->l);
//     fprintf(stderr, "sp: 0x%04X\n", cpu->sp);
//     fprintf(stderr, "cycles: %llu\n", cpu->cycles);
//     fprintf(stderr, "===================\n\n");

//     va_end(args);
//     exit(1);
// }

#endif
