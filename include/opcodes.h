#ifndef OPCODES_HEADER
#define OPCODES_HEADER

#include <stdint.h>

#include "cpu.h"

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

// forward declarations for memory access functions (bus.h)
uint8_t mem_read(uint16_t addr);
uint16_t mem_read16(uint16_t addr);
void mem_write(uint16_t addr, uint8_t val);

// decode and execute the opcode (switch statement)
void decode_and_execute(CPU *cpu, uint8_t op);

// helper to advance the program counter
inline void advance_pc(CPU *cpu, uint8_t n) { cpu->pc += n; }

// helper to advance the cycles
inline void advance_cycles(CPU *cpu, uint8_t n) { cpu->cycles += n; }

// helper to get the high byte of a 16-bit value
inline uint8_t high_byte(uint16_t val) { return (val >> 8) & 0xFF; }

// helper to get the low byte of a 16-bit value
inline uint8_t low_byte(uint16_t val) { return val & 0xFF; }

/* helper to get a flag */
inline int get_flag(CPU *cpu, uint8_t flag) { return (cpu->f & flag) != 0; }

/* helper to set a flag */
inline void set_flag(CPU *cpu, uint8_t flag, int enable) {
    if (enable) {
        cpu->f |= flag; /* if enable = 1, turn the flag bit to 1 */
    } else {
        cpu->f &= ~flag; /* if enable = 0, turn the flag bit to 0 */
    }
}

/*  -----  MACROS ---------------------------------------------------------- */

/* other helpers -------------------------------- */
#define ADV_PC(cpu, n) advance_pc((cpu), (n))
#define ADV_CYCLES(cpu, n) advance_cycles((cpu), (n))

inline void call_u16(CPU *cpu) {
    /* 1. fetch the target address (little‑endian) */
    uint16_t target = mem_read16(cpu->pc);

    /* 2. compute address to return to and skip operand */
    uint16_t ret_addr = cpu->pc + 2;
    advance_pc(cpu, 2);

    /* 3. push ret_addr (high byte first, then low byte) */
    cpu->sp--;
    mem_write(cpu->sp, high_byte(ret_addr)); /* high */
    cpu->sp--;
    mem_write(cpu->sp, low_byte(ret_addr)); /* low  */

    /* 4. jump */
    cpu->pc = target;

    /* 5. timing */
    ADV_CYCLES(cpu, 24);
}

#define CALL_U16(cpu) call_u16(cpu)

/*  x8/alu  ---------------------------------------------------------------- */
#define DEF_INC_R8(OP, REG)                                                   \
    static void op_##OP##_i_##REG(CPU *cpu) {                                 \
        /* 1. r8 <- r8 + 1 */                                                 \
        uint8_t old = (cpu)->REG;                                             \
        (cpu)->REG += 1;                                                      \
        /* 2. set flags */                                                    \
        set_flag(cpu, FLAG_Z, (cpu)->REG == 0);                               \
        set_flag(cpu, FLAG_N, 0);                                             \
        set_flag(cpu, FLAG_H,                                                 \
                 (old & 0x0F) == 0x0F); /* was the low nibble of the original \
                                           value 1111? carry will occur */    \
        ADV_CYCLES(cpu, 4);                                                   \
    }

#define DEF_DEC_R8(OP, REG)                                                   \
    static void op_##OP##_d_##REG(CPU *cpu) {                                 \
        /* 1. r8 <- r8 - 1 */                                                 \
        uint8_t old = (cpu)->REG;                                             \
        (cpu)->REG -= 1;                                                      \
        /* 2. set flags */                                                    \
        set_flag(cpu, FLAG_Z, (cpu)->REG == 0);                               \
        set_flag(cpu, FLAG_N, 1);                                             \
        set_flag(cpu, FLAG_H,                                                 \
                 (old & 0x0F) == 0x00); /* was the low nibble of the original \
                              value 0000? borrow will occur */                \
        ADV_CYCLES(cpu, 4);                                                   \
    }

#define DEF_ADD_A_R8(OP, R8)                                                  \
    static void op_##OP##_add_a_##R8(CPU *cpu) {                              \
        /* 1. r8 <- r8 + r8 */                                                \
        uint16_t result = (cpu)->a + (cpu)->R8;                               \
        /* 2. set flags */                                                    \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                          \
        set_flag(cpu, FLAG_N, 0);                                             \
        set_flag(cpu, FLAG_H, ((cpu)->a & 0x0F) + ((cpu)->R8 & 0x0F) > 0x0F); \
        set_flag(cpu, FLAG_C, result > 0xFF);                                 \
        (cpu)->R8 =                                                           \
            result &                                                          \
            0x00FF; /* make sure we only store ONE byte, the lower one */     \
        ADV_CYCLES(cpu, 4);                                                   \
    }

#define DEF_ADD_A_HLPTR(OP)                                                   \
    static void op_##OP##_add_a_hlptr(CPU *cpu) {                             \
        /* 1. a <- a + (hl) */                                                \
        uint8_t byte_read = mem_read((cpu)->hl);                              \
        uint16_t result = (cpu)->a + byte_read;                               \
        /* 2. set flags */                                                    \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                          \
        set_flag(cpu, FLAG_N, 0);                                             \
        set_flag(cpu, FLAG_H, ((cpu)->a & 0x0F) + (byte_read & 0x0F) > 0x0F); \
        set_flag(cpu, FLAG_C, result > 0xFF);                                 \
        (cpu)->a =                                                            \
            result &                                                          \
            0x00FF; /* make sure we only store ONE byte, the lower one */     \
        ADV_CYCLES(cpu, 8);                                                   \
    }

#define DEF_ADD_A_U8(OP)                                                      \
    static void op_##OP##_add_a_u8(CPU *cpu) {                                \
        /* 1. a <- a + u8 */                                                  \
        uint8_t byte_read = mem_read(cpu->pc);                                \
        uint16_t result = (cpu)->a + byte_read;                               \
        /* 2. set flags */                                                    \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                          \
        set_flag(cpu, FLAG_N, 0);                                             \
        set_flag(cpu, FLAG_H, ((cpu)->a & 0x0F) + (byte_read & 0x0F) > 0x0F); \
        set_flag(cpu, FLAG_C, result > 0xFF);                                 \
        (cpu)->a =                                                            \
            result &                                                          \
            0x00FF; /* make sure we only store ONE byte, the lower one */     \
        ADV_PC(cpu, 1);                                                       \
        ADV_CYCLES(cpu, 8);                                                   \
    }

#define DEF_ADC_A_R8(OP, R8)                                                 \
    static void op_##OP##_adc_a_##R8(CPU *cpu) {                             \
        /* 1. a <- a + r8 + carry */                                         \
        uint16_t result = (cpu)->a + (cpu)->R8 + get_flag(cpu, FLAG_C);      \
        /* 2. set flags */                                                   \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                         \
        set_flag(cpu, FLAG_N, 0);                                            \
        set_flag(                                                            \
            cpu, FLAG_H,                                                     \
            ((cpu)->a & 0x0F) + ((cpu)->R8 & 0x0F) + get_flag(cpu, FLAG_C) > \
                0x0F);                                                       \
        set_flag(cpu, FLAG_C, result > 0xFF);                                \
        (cpu)->a =                                                           \
            result &                                                         \
            0x00FF; /* make sure we only store ONE byte, the lower one */    \
        ADV_CYCLES(cpu, 4);                                                  \
    }

#define DEF_ADC_A_HLPTR(OP)                                                  \
    static void op_##OP##_adc_a_hlptr(CPU *cpu) {                            \
        /* 1. a <- a + (hl) + carry */                                       \
        uint8_t byte_read = mem_read((cpu)->hl);                             \
        uint16_t result = (cpu)->a + byte_read + get_flag(cpu, FLAG_C);      \
        /* 2. set flags */                                                   \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                         \
        set_flag(cpu, FLAG_N, 0);                                            \
        set_flag(                                                            \
            cpu, FLAG_H,                                                     \
            ((cpu)->a & 0x0F) + (byte_read & 0x0F) + get_flag(cpu, FLAG_C) > \
                0x0F);                                                       \
        set_flag(cpu, FLAG_C, result > 0xFF);                                \
        (cpu)->a =                                                           \
            result &                                                         \
            0x00FF; /* make sure we only store ONE byte, the lower one */    \
        ADV_CYCLES(cpu, 8);                                                  \
    }

#define DEF_ADC_A_U8(OP)                                                     \
    static void op_##OP##_adc_a_u8(CPU *cpu) {                               \
        /* 1. a <- a + u8 + carry */                                         \
        uint8_t byte_read = mem_read(cpu->pc);                               \
        uint16_t result = (cpu)->a + byte_read + get_flag(cpu, FLAG_C);      \
        /* 2. set flags */                                                   \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                         \
        set_flag(cpu, FLAG_N, 0);                                            \
        set_flag(                                                            \
            cpu, FLAG_H,                                                     \
            ((cpu)->a & 0x0F) + (byte_read & 0x0F) + get_flag(cpu, FLAG_C) > \
                0x0F);                                                       \
        set_flag(cpu, FLAG_C, result > 0xFF);                                \
        (cpu)->a =                                                           \
            result &                                                         \
            0x00FF; /* make sure we only store ONE byte, the lower one */    \
        ADV_PC(cpu, 1);                                                      \
        ADV_CYCLES(cpu, 8);                                                  \
    }

#define DEF_SUB_A_R8(OP, R8)                                              \
    static void op_##OP##_sub_a_##R8(CPU *cpu) {                          \
        /* 1. a <- a - r8 */                                              \
        uint16_t result = (cpu)->a - (cpu)->R8;                           \
        /* 2. set flags */                                                \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                      \
        set_flag(cpu, FLAG_N, 1);                                         \
        set_flag(cpu, FLAG_H, ((cpu)->a & 0x0F) < ((cpu)->R8 & 0x0F));    \
        set_flag(cpu, FLAG_C, result > 0xFF);                             \
        (cpu)->a =                                                        \
            result &                                                      \
            0x00FF; /* make sure we only store ONE byte, the lower one */ \
        ADV_CYCLES(cpu, 4);                                               \
    }

#define DEF_SUB_A_HLPTR(OP)                                               \
    static void op_##OP##_sub_a_hlptr(CPU *cpu) {                         \
        /* 1. a <- a - (hl) */                                            \
        uint8_t byte_read = mem_read((cpu)->hl);                          \
        uint16_t result = (cpu)->a - byte_read;                           \
        /* 2. set flags */                                                \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                      \
        set_flag(cpu, FLAG_N, 1);                                         \
        set_flag(cpu, FLAG_H, ((cpu)->a & 0x0F) < (byte_read & 0x0F));    \
        set_flag(cpu, FLAG_C, result > 0xFF);                             \
        (cpu)->a =                                                        \
            result &                                                      \
            0x00FF; /* make sure we only store ONE byte, the lower one */ \
        ADV_CYCLES(cpu, 8);                                               \
    }

#define DEF_SUB_A_U8(OP)                                                  \
    static void op_##OP##_sub_a_u8(CPU *cpu) {                            \
        /* 1. a <- a - u8 */                                              \
        uint8_t byte_read = mem_read(cpu->pc);                            \
        uint16_t result = (cpu)->a - byte_read;                           \
        /* 2. set flags */                                                \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                      \
        set_flag(cpu, FLAG_N, 1);                                         \
        set_flag(cpu, FLAG_H, ((cpu)->a & 0x0F) < (byte_read & 0x0F));    \
        set_flag(cpu, FLAG_C, result > 0xFF);                             \
        (cpu)->a =                                                        \
            result &                                                      \
            0x00FF; /* make sure we only store ONE byte, the lower one */ \
        ADV_PC(cpu, 1);                                                   \
        ADV_CYCLES(cpu, 8);                                               \
    }

#define DEF_SBC_A_R8(OP, R8)                                                 \
    static void op_##OP##_sbc_a_##R8(CPU *cpu) {                             \
        /* 1. a <- a - r8 - carry */                                         \
        uint16_t result = (cpu)->a - (cpu)->R8 - get_flag(cpu, FLAG_C);      \
        /* 2. set flags */                                                   \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                         \
        set_flag(cpu, FLAG_N, 1);                                            \
        set_flag(                                                            \
            cpu, FLAG_H,                                                     \
            ((cpu)->a & 0x0F) < ((cpu)->R8 & 0x0F) + get_flag(cpu, FLAG_C)); \
        set_flag(cpu, FLAG_C, result > 0xFF);                                \
        (cpu)->a =                                                           \
            result &                                                         \
            0x00FF; /* make sure we only store ONE byte, the lower one */    \
        ADV_CYCLES(cpu, 4);                                                  \
    }

#define DEF_SBC_A_HLPTR(OP)                                                  \
    static void op_##OP##_sbc_a_hlptr(CPU *cpu) {                            \
        /* 1. a <- a - (hl) - carry */                                       \
        uint8_t byte_read = mem_read((cpu)->hl);                             \
        uint16_t result = (cpu)->a - byte_read - get_flag(cpu, FLAG_C);      \
        /* 2. set flags */                                                   \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                         \
        set_flag(cpu, FLAG_N, 1);                                            \
        set_flag(                                                            \
            cpu, FLAG_H,                                                     \
            ((cpu)->a & 0x0F) < (byte_read & 0x0F) + get_flag(cpu, FLAG_C)); \
        set_flag(cpu, FLAG_C, result > 0xFF);                                \
        (cpu)->a =                                                           \
            result &                                                         \
            0x00FF; /* make sure we only store ONE byte, the lower one */    \
        ADV_CYCLES(cpu, 8);                                                  \
    }

#define DEF_SBC_A_U8(OP)                                                     \
    static void op_##OP##_sbc_a_u8(CPU *cpu) {                               \
        /* 1. a <- a - u8 - carry */                                         \
        uint8_t byte_read = mem_read(cpu->pc);                               \
        uint16_t result = (cpu)->a - byte_read - get_flag(cpu, FLAG_C);      \
        /* 2. set flags */                                                   \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                         \
        set_flag(cpu, FLAG_N, 1);                                            \
        set_flag(                                                            \
            cpu, FLAG_H,                                                     \
            ((cpu)->a & 0x0F) < (byte_read & 0x0F) + get_flag(cpu, FLAG_C)); \
        set_flag(cpu, FLAG_C, result > 0xFF);                                \
        (cpu)->a =                                                           \
            result &                                                         \
            0x00FF; /* make sure we only store ONE byte, the lower one */    \
        ADV_PC(cpu, 1);                                                      \
        ADV_CYCLES(cpu, 8);                                                  \
    }

#define DEF_AND_A_R8(OP, R8)                     \
    static void op_##OP##_and_a_##R8(CPU *cpu) { \
        /* 1. a <- a & r8 */                     \
        (cpu)->a &= (cpu)->R8;                   \
        /* 2. set flags */                       \
        set_flag(cpu, FLAG_Z, (cpu)->a == 0);    \
        set_flag(cpu, FLAG_N, 0);                \
        set_flag(cpu, FLAG_H, 1);                \
        set_flag(cpu, FLAG_C, 0);                \
        ADV_CYCLES(cpu, 4);                      \
    }

#define DEF_AND_A_HLPTR(OP)                       \
    static void op_##OP##_and_a_hlptr(CPU *cpu) { \
        /* 1. a <- a & (hl) */                    \
        (cpu)->a &= mem_read((cpu)->hl);          \
        /* 2. set flags */                        \
        set_flag(cpu, FLAG_Z, (cpu)->a == 0);     \
        set_flag(cpu, FLAG_N, 0);                 \
        set_flag(cpu, FLAG_H, 1);                 \
        set_flag(cpu, FLAG_C, 0);                 \
        ADV_CYCLES(cpu, 8);                       \
    }

#define DEF_AND_A_U8(OP)                       \
    static void op_##OP##_and_a_u8(CPU *cpu) { \
        /* 1. a <- a & u8 */                   \
        (cpu)->a &= mem_read(cpu->pc);         \
        /* 2. set flags */                     \
        set_flag(cpu, FLAG_Z, (cpu)->a == 0);  \
        set_flag(cpu, FLAG_N, 0);              \
        set_flag(cpu, FLAG_H, 1);              \
        set_flag(cpu, FLAG_C, 0);              \
        ADV_PC(cpu, 1);                        \
        ADV_CYCLES(cpu, 8);                    \
    }

#define DEF_XOR_A_R8(OP, R8)                     \
    static void op_##OP##_xor_a_##R8(CPU *cpu) { \
        /* 1. a <- a ^ r8 */                     \
        (cpu)->a ^= (cpu)->R8;                   \
        /* 2. set flags */                       \
        set_flag(cpu, FLAG_Z, (cpu)->a == 0);    \
        set_flag(cpu, FLAG_N, 0);                \
        set_flag(cpu, FLAG_H, 0);                \
        set_flag(cpu, FLAG_C, 0);                \
        ADV_CYCLES(cpu, 4);                      \
    }

#define DEF_XOR_A_HLPTR(OP)                       \
    static void op_##OP##_xor_a_hlptr(CPU *cpu) { \
        /* 1. a <- a ^ (hl) */                    \
        (cpu)->a ^= mem_read((cpu)->hl);          \
        /* 2. set flags */                        \
        set_flag(cpu, FLAG_Z, (cpu)->a == 0);     \
        set_flag(cpu, FLAG_N, 0);                 \
        set_flag(cpu, FLAG_H, 0);                 \
        set_flag(cpu, FLAG_C, 0);                 \
        ADV_CYCLES(cpu, 8);                       \
    }

#define DEF_XOR_A_U8(OP)                       \
    static void op_##OP##_xor_a_u8(CPU *cpu) { \
        /* 1. a <- a ^ u8 */                   \
        (cpu)->a ^= mem_read(cpu->pc);         \
        /* 2. set flags */                     \
        set_flag(cpu, FLAG_Z, (cpu)->a == 0);  \
        set_flag(cpu, FLAG_N, 0);              \
        set_flag(cpu, FLAG_H, 0);              \
        set_flag(cpu, FLAG_C, 0);              \
        ADV_PC(cpu, 1);                        \
        ADV_CYCLES(cpu, 8);                    \
    }

#define DEF_OR_A_R8(OP, R8)                     \
    static void op_##OP##_or_a_##R8(CPU *cpu) { \
        /* 1. a <- a | r8 */                    \
        (cpu)->a |= (cpu)->R8;                  \
        /* 2. set flags */                      \
        set_flag(cpu, FLAG_Z, (cpu)->a == 0);   \
        set_flag(cpu, FLAG_N, 0);               \
        set_flag(cpu, FLAG_H, 0);               \
        set_flag(cpu, FLAG_C, 0);               \
        ADV_CYCLES(cpu, 4);                     \
    }

#define DEF_OR_A_HLPTR(OP)                       \
    static void op_##OP##_or_a_hlptr(CPU *cpu) { \
        /* 1. a <- a | (hl) */                   \
        (cpu)->a |= mem_read((cpu)->hl);         \
        /* 2. set flags */                       \
        set_flag(cpu, FLAG_Z, (cpu)->a == 0);    \
        set_flag(cpu, FLAG_N, 0);                \
        set_flag(cpu, FLAG_H, 0);                \
        set_flag(cpu, FLAG_C, 0);                \
        ADV_CYCLES(cpu, 8);                      \
    }

#define DEF_OR_A_U8(OP)                       \
    static void op_##OP##_or_a_u8(CPU *cpu) { \
        /* 1. a <- a | (pc) */                \
        (cpu)->a |= mem_read((cpu)->pc);      \
        /* 2. set flags */                    \
        set_flag(cpu, FLAG_Z, (cpu)->a == 0); \
        set_flag(cpu, FLAG_N, 0);             \
        set_flag(cpu, FLAG_H, 0);             \
        set_flag(cpu, FLAG_C, 0);             \
        ADV_CYCLES(cpu, 8);                   \
    }

#define DEF_CP_A_R8(OP, R8)                                            \
    static void op_##OP##_cp_a_##R8(CPU *cpu) {                        \
        /* 1. a - r8, but discard the result */                        \
        uint16_t result = (cpu)->a - (cpu)->R8;                        \
        /* 2. set flags */                                             \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                   \
        set_flag(cpu, FLAG_N, 1);                                      \
        set_flag(cpu, FLAG_H, ((cpu)->a & 0x0F) < ((cpu)->R8 & 0x0F)); \
        set_flag(cpu, FLAG_C, result > 0xFF);                          \
        ADV_CYCLES(cpu, 4);                                            \
    }

#define DEF_CP_A_HLPTR(OP)                                             \
    static void op_##OP##_cp_a_hlptr(CPU *cpu) {                       \
        /* 1. a - (hl), but discard the result */                      \
        uint8_t byte_read = mem_read((cpu)->hl);                       \
        uint16_t result = (cpu)->a - byte_read;                        \
        /* 2. set flags */                                             \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                   \
        set_flag(cpu, FLAG_N, 1);                                      \
        set_flag(cpu, FLAG_H, ((cpu)->a & 0x0F) < (byte_read & 0x0F)); \
        set_flag(cpu, FLAG_C, result > 0xFF);                          \
        ADV_CYCLES(cpu, 8);                                            \
    }

#define DEF_CP_A_U8(OP)                                                \
    static void op_##OP##_cp_a_u8(CPU *cpu) {                          \
        /* 1. a - u8, but discard the result */                        \
        uint8_t byte_read = mem_read(cpu->pc);                         \
        uint16_t result = (cpu)->a - byte_read;                        \
        /* 2. set flags */                                             \
        set_flag(cpu, FLAG_Z, (result & 0xFF) == 0);                   \
        set_flag(cpu, FLAG_N, 1);                                      \
        set_flag(cpu, FLAG_H, ((cpu)->a & 0x0F) < (byte_read & 0x0F)); \
        set_flag(cpu, FLAG_C, result > 0xFF);                          \
        ADV_PC(cpu, 1);                                                \
        ADV_CYCLES(cpu, 8);                                            \
    }

/* x16/alu  ----------------------------------- */
#define DEF_INC_R16(OP, REG)                  \
    static void op_##OP##_i_##REG(CPU *cpu) { \
        /* r16 <- r16 + 1 */                  \
        (cpu)->REG += 1;                      \
        ADV_CYCLES(cpu, 8);                   \
    }

#define DEF_DEC_R16(OP, REG)                  \
    static void op_##OP##_d_##REG(CPU *cpu) { \
        /* r16 <- r16 - 1 */                  \
        (cpu)->REG -= 1;                      \
        ADV_CYCLES(cpu, 8);                   \
    }

/*  x8/lsm  ---------------------------------------------- */
#define DEF_LD_R8_U8(OP, REG)                       \
    static void op_##OP##_ld_##REG##_u8(CPU *cpu) { \
        /* r8 <- u8 */                              \
        (cpu)->REG = mem_read((cpu)->pc);           \
        ADV_PC(cpu, 1);                             \
        ADV_CYCLES(cpu, 8);                         \
    }

#define DEF_LD_R8_R8(OP, DST, SRC)                     \
    static void op_##OP##_ld_##DST##_##SRC(CPU *cpu) { \
        /* r8 <- r8 */                                 \
        (cpu)->DST = (cpu)->SRC;                       \
        ADV_CYCLES(cpu, 4);                            \
    }

#define DEF_LD_R16PTR_R8(OP, R16, R8)                    \
    static void op_##OP##_ld_##R16##ptr_##R8(CPU *cpu) { \
        /* (r16) <- r8 */                                \
        mem_write((cpu)->R16, (cpu)->R8);                \
        ADV_CYCLES(cpu, 8);                              \
    }

#define DEF_LD_R8_R16PTR_INC(OP, R8, R16)                    \
    static void op_##OP##_ld_##R8##_##R16##ptr_i(CPU *cpu) { \
        /* 1. r8 <- (r16++) */                               \
        (cpu)->R8 = mem_read((cpu)->R16++);                  \
        ADV_CYCLES(cpu, 8);                                  \
    }

#define DEF_LD_R8_R16PTR_DEC(OP, R8, R16)                    \
    static void op_##OP##_ld_##R8##_##R16##ptr_d(CPU *cpu) { \
        /* 1. r8 <- (r16--) */                               \
        (cpu)->R8 = mem_read((cpu)->R16--);                  \
        ADV_CYCLES(cpu, 8);                                  \
    }

#define DEF_LD_R8_U16PTR(OP, R8)                       \
    static void op_##OP##_ld_##R8##_u16ptr(CPU *cpu) { \
        /* r8 <- (u16) */                              \
        uint16_t addr = mem_read16((cpu)->pc);         \
        (cpu)->R8 = mem_read(addr);                    \
        ADV_PC(cpu, 2);                                \
        ADV_CYCLES(cpu, 16);                           \
    }

#define DEF_LD_U16PTR_R8(OP, SRC)                     \
    static void op_##OP##_ld_u16ptr_##SRC(CPU *cpu) { \
        /* (u16) <- r8 */                             \
        uint16_t addr = mem_read16((cpu)->pc);        \
        mem_write(addr, (cpu)->SRC);                  \
        ADV_PC(cpu, 2);                               \
        ADV_CYCLES(cpu, 16);                          \
    }

#define DEF_LD_A_FF00U8PTR(OP)                             \
    static void op_##OP##_ld_a_ff00u8ptr(CPU *cpu) {       \
        /* 1. a <- (FF00 + u8) */                          \
        (cpu)->a = mem_read(0xFF00 + mem_read((cpu)->pc)); \
        ADV_PC(cpu, 1);                                    \
        ADV_CYCLES(cpu, 12);                               \
    }

#define DEF_LD_FF00U8PTR_A(OP)                        \
    static void op_##OP##_ld_ff00u8ptr_a(CPU *cpu) {  \
        /* (FF00 + u8) <- a */                        \
        uint16_t addr = 0xFF00 + mem_read((cpu)->pc); \
        mem_write(addr, (cpu)->a);                    \
        ADV_PC(cpu, 1);                               \
        ADV_CYCLES(cpu, 12);                          \
    }

#define DEF_LD_R8_R16PTR(OP, R8, R16)                      \
    static void op_##OP##_ld_##R8##_##R16##ptr(CPU *cpu) { \
        /* 1. r8 <- (r16) */                               \
        (cpu)->R8 = mem_read((cpu)->R16);                  \
        ADV_CYCLES(cpu, 8);                                \
    }

#define DEF_LD_R16PTR_U8(OP, R16)                      \
    static void op_##OP##_ld_##R16##ptr_u8(CPU *cpu) { \
        /* (r16) <- u8 */                              \
        mem_write((cpu)->R16, mem_read((cpu)->pc));    \
        ADV_PC(cpu, 1);                                \
        ADV_CYCLES(cpu, 12);                           \
    }

#define DEF_LD_R16PTR_INC_R8(OP, R16, R8)                  \
    static void op_##OP##_ld_##R16##ptr_i_##R8(CPU *cpu) { \
        /* (r16++) <- r8 */                                \
        mem_write((cpu)->R16++, (cpu)->R8);                \
        ADV_CYCLES(cpu, 8);                                \
    }

#define DEF_LD_R16PTR_DEC_R8(OP, R16, R8)                  \
    static void op_##OP##_ld_##R16##ptr_d_##R8(CPU *cpu) { \
        /* (r16--) <- r8 */                                \
        mem_write((cpu)->R16--, (cpu)->R8);                \
        ADV_CYCLES(cpu, 8);                                \
    }

/*  x16/lsm  -------------------------------------- */
#define DEF_LD_R16_U16(OP, R16)                      \
    static void op_##OP##_ld_##R16##_u16(CPU *cpu) { \
        /* r16 <- u16 */                             \
        uint16_t v = mem_read16((cpu)->pc);          \
        (cpu)->R16 = v;                              \
        ADV_PC(cpu, 2);                              \
        ADV_CYCLES(cpu, 12);                         \
    }

#define DEF_PUSH_R16(OP, R16)                        \
    static void op_##OP##_push_##R16(CPU *cpu) {     \
        /* 1. get the high and low bytes of r16 */   \
        uint8_t high = high_byte(cpu->R16);          \
        uint8_t low = low_byte(cpu->R16);            \
        /* 2. first, write the high, then the low */ \
        cpu->sp--;                                   \
        mem_write(cpu->sp, high);                    \
        cpu->sp--;                                   \
        mem_write(cpu->sp, low);                     \
        ADV_CYCLES(cpu, 16);                         \
    }

#define DEF_POP_R16(OP, R16)                    \
    static void op_##OP##_pop_##R16(CPU *cpu) { \
        /* 1. pop the low byte first */         \
        uint8_t low = mem_read(cpu->sp++);      \
        uint8_t high = mem_read(cpu->sp++);     \
        /* 2. combine the two bytes into r16 */ \
        cpu->R16 = (high << 8) | low;           \
        ADV_CYCLES(cpu, 12);                    \
    }

/*  control/branch  --------------------------------------------------- */
#define DEF_JR_U8(OP)                             \
    static void op_##OP##_jr_u8(CPU *cpu) {       \
        /* unconditional jump */                  \
        int8_t off = (int8_t)mem_read((cpu)->pc); \
        ADV_PC(cpu, 1);                           \
        (cpu)->pc += off;                         \
        ADV_CYCLES(cpu, 12);                      \
    }

#define DEF_JR_Z_U8(OP)                           \
    static void op_##OP##_jr_z_u8(CPU *cpu) {     \
        /* jump w/ offset if FLAG_Z = 1 */        \
        int8_t off = (int8_t)mem_read((cpu)->pc); \
        ADV_PC(cpu, 1);                           \
        if (get_flag(cpu, FLAG_Z)) {              \
            (cpu)->pc += off;                     \
            ADV_CYCLES(cpu, 12);                  \
        } else {                                  \
            ADV_CYCLES(cpu, 8);                   \
        }                                         \
    }

#define DEF_JR_NZ_U8(OP)                          \
    static void op_##OP##_jr_nz_u8(CPU *cpu) {    \
        /* jump w/ offset if FLAG_Z = 0 */        \
        int8_t off = (int8_t)mem_read((cpu)->pc); \
        ADV_PC(cpu, 1);                           \
        if (!get_flag(cpu, FLAG_Z)) {             \
            (cpu)->pc += off;                     \
            ADV_CYCLES(cpu, 12);                  \
        } else {                                  \
            ADV_CYCLES(cpu, 8);                   \
        }                                         \
    }

#define DEF_JR_C_U8(OP)                           \
    static void op_##OP##_jr_c_u8(CPU *cpu) {     \
        /* jump w/ offset if FLAG_C = 1 */        \
        int8_t off = (int8_t)mem_read((cpu)->pc); \
        ADV_PC(cpu, 1);                           \
        if (get_flag(cpu, FLAG_C)) {              \
            (cpu)->pc += off;                     \
            ADV_CYCLES(cpu, 12);                  \
        } else {                                  \
            ADV_CYCLES(cpu, 8);                   \
        }                                         \
    }

#define DEF_JR_NC_U8(OP)                          \
    static void op_##OP##_jr_nc_u8(CPU *cpu) {    \
        /* jump w/ offset if FLAG_C = 0 */        \
        int8_t off = (int8_t)mem_read((cpu)->pc); \
        ADV_PC(cpu, 1);                           \
        if (!get_flag(cpu, FLAG_C)) {             \
            (cpu)->pc += off;                     \
            ADV_CYCLES(cpu, 12);                  \
        } else {                                  \
            ADV_CYCLES(cpu, 8);                   \
        }                                         \
    }

#define DEF_JP(OP)                             \
    static void op_##OP##_jp(CPU *cpu) {       \
        /* unconditional jump */               \
        uint16_t addr = mem_read16((cpu)->pc); \
        (cpu)->pc = addr;                      \
        ADV_CYCLES(cpu, 16);                   \
    }

#define DEF_CALL_U16(OP)                       \
    static void op_##OP##_call_u16(CPU *cpu) { \
        /* use the CALL helper */              \
        CALL_U16(cpu);                         \
    }

#define DEF_CALL_NZ_U16(OP)                              \
    static void op_##OP##_call_nz_u16(CPU *cpu) {        \
        /* call if FLAG_Z = 0 */                         \
        if (!get_flag(cpu, FLAG_Z)) {                    \
            CALL_U16(cpu); /* push the return address */ \
        } else {                                         \
            ADV_PC(cpu, 2);                              \
            ADV_CYCLES(cpu, 12);                         \
        }                                                \
    }

#define DEF_CALL_NC_U16(OP)                           \
    static void op_##OP##_call_nc_u16(CPU *cpu) {     \
        /* call if FLAG_C = 0 */                      \
        if (!get_flag(cpu, FLAG_C)) {                 \
            CALL_U16(cpu); /* push the return addr */ \
        } else {                                      \
            ADV_PC(cpu, 2);                           \
            ADV_CYCLES(cpu, 12);                      \
        }                                             \
    }

#define DEF_RET(OP)                                                      \
    static void op_##OP##_ret(CPU *cpu) {                                \
        /* 1. pop the return address (low byte first, then high byte) */ \
        uint8_t low = mem_read(cpu->sp++);                               \
        uint8_t high = mem_read(cpu->sp++);                              \
        cpu->pc = (high << 8) | low;                                     \
        ADV_CYCLES(cpu, 16);                                             \
    }

/*  control/misc --------------------------- */
#define DEF_IMPLIED(OP, NAME, BODY, CYC)     \
    static void op_##OP##_##NAME(CPU *cpu) { \
        BODY;                                \
        ADV_CYCLES(cpu, CYC);                \
    }

#endif
