#include <stdint.h>

typedef struct {
    // exposing registers as unions so we can access them both regularly and as
    // pairs

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

} CPU;
