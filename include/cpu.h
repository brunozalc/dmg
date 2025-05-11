#ifndef CPU_HEADER
#define CPU_HEADER

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *cpu_log;

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

    // interrupt enable register
    int ime;     /* 0 or 1, current state (READ ONLY) */
    int set_ime; /* 0 or 1, value to copy after one instruction */

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
    int halt; /* 0 or 1, current state */

    // last opcode executed (debugging)
    uint8_t last_opcode;

} CPU;

void cpu_reset(CPU *cpu);
void cpu_step(CPU *cpu);

/* function to log an error
- prints information about the CPU state when the error occurred
*/
void log_cpu_error(CPU *cpu, const char *format, ...);

#endif
