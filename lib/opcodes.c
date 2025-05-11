/* check https://izik1.github.io/gbops/index.html */

#include "opcodes.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* ----  x8/alu ---- */
DEF_INC_R8(0x04, b)   /* INC B */
DEF_DEC_R8(0x05, b)   /* DEC B */
DEF_INC_R8(0x0c, c)   /* INC C */
DEF_DEC_R8(0x0d, c)   /* DEC C */
DEF_INC_R8(0x14, d)   /* INC D */
DEF_DEC_R8(0x15, d)   /* DEC D */
DEF_INC_R8(0x1c, e)   /* INC E */
DEF_DEC_R8(0x1d, e)   /* DEC E */
DEF_INC_R8(0x24, h)   /* INC H */
DEF_DEC_R8(0x25, h)   /* DEC H */
DEF_INC_R8(0x2c, l)   /* INC L */
DEF_DEC_R8(0x2d, l)   /* DEC L */
DEF_INC_R16(0x34, hl) /* INC HL */
DEF_DEC_R16(0x35, hl) /* DEC HL */
DEF_INC_R8(0x3c, a)   /* INC A */
DEF_DEC_R8(0x3d, a)   /* DEC A */

/* add */
DEF_ADD_A_R8(0x80, b) /* ADD A, B */
DEF_ADD_A_R8(0x81, c) /* ADD A, C */
DEF_ADD_A_R8(0x82, d) /* ADD A, D */
DEF_ADD_A_R8(0x83, e) /* ADD A, E */
DEF_ADD_A_R8(0x84, h) /* ADD A, H */
DEF_ADD_A_R8(0x85, l) /* ADD A, L */
DEF_ADD_A_HLPTR(0x86) /* ADD A, (HL) */
DEF_ADD_A_R8(0x87, a) /* ADD A, A */
DEF_ADC_A_R8(0x88, b) /* ADC A, B */
DEF_ADC_A_R8(0x89, c) /* ADC A, C */
DEF_ADC_A_R8(0x8a, d) /* ADC A, D */
DEF_ADC_A_R8(0x8b, e) /* ADC A, E */
DEF_ADC_A_R8(0x8c, h) /* ADC A, H */
DEF_ADC_A_R8(0x8d, l) /* ADC A, L */
DEF_ADC_A_HLPTR(0x8e) /* ADC A, (HL) */
DEF_ADC_A_R8(0x8f, a) /* ADC A, A */

/* sub */
DEF_SUB_A_R8(0x90, b) /* SUB A, B */
DEF_SUB_A_R8(0x91, c) /* SUB A, C */
DEF_SUB_A_R8(0x92, d) /* SUB A, D */
DEF_SUB_A_R8(0x93, e) /* SUB A, E */
DEF_SUB_A_R8(0x94, h) /* SUB A, H */
DEF_SUB_A_R8(0x95, l) /* SUB A, L */
DEF_SUB_A_HLPTR(0x96) /* SUB A, (HL) */
DEF_SUB_A_R8(0x97, a) /* SUB A, A */
DEF_SBC_A_R8(0x98, b) /* SBC A, B */
DEF_SBC_A_R8(0x99, c) /* SBC A, C */
DEF_SBC_A_R8(0x9a, d) /* SBC A, D */
DEF_SBC_A_R8(0x9b, e) /* SBC A, E */
DEF_SBC_A_R8(0x9c, h) /* SBC A, H */
DEF_SBC_A_R8(0x9d, l) /* SBC A, L */
DEF_SBC_A_HLPTR(0x9e) /* SBC A, (HL) */
DEF_SBC_A_R8(0x9f, a) /* SBC A, A */

/* and & xor */
DEF_AND_A_R8(0xa0, b) /* AND A, B */
DEF_AND_A_R8(0xa1, c) /* AND A, C */
DEF_AND_A_R8(0xa2, d) /* AND A, D */
DEF_AND_A_R8(0xa3, e) /* AND A, E */
DEF_AND_A_R8(0xa4, h) /* AND A, H */
DEF_AND_A_R8(0xa5, l) /* AND A, L */
DEF_AND_A_HLPTR(0xa6) /* AND A, (HL) */
DEF_AND_A_R8(0xa7, a) /* AND A, A */
DEF_XOR_A_R8(0xa8, b) /* XOR A, B */
DEF_XOR_A_R8(0xa9, c) /* XOR A, C */
DEF_XOR_A_R8(0xaa, d) /* XOR A, D */
DEF_XOR_A_R8(0xab, e) /* XOR A, E */
DEF_XOR_A_R8(0xac, h) /* XOR A, H */
DEF_XOR_A_R8(0xad, l) /* XOR A, L */
DEF_XOR_A_HLPTR(0xae) /* XOR A, (HL) */
DEF_XOR_A_R8(0xaf, a) /* XOR A, A */

/* or & cp */
DEF_OR_A_R8(0xb0, b) /* OR A, B */
DEF_OR_A_R8(0xb1, c) /* OR A, C */
DEF_OR_A_R8(0xb2, d) /* OR A, D */
DEF_OR_A_R8(0xb3, e) /* OR A, E */
DEF_OR_A_R8(0xb4, h) /* OR A, H */
DEF_OR_A_R8(0xb5, l) /* OR A, L */
DEF_OR_A_HLPTR(0xb6) /* OR A, (HL) */
DEF_OR_A_R8(0xb7, a) /* OR A, A */
DEF_CP_A_R8(0xb8, b) /* CP A, B */
DEF_CP_A_R8(0xb9, c) /* CP A, C */
DEF_CP_A_R8(0xba, d) /* CP A, D */
DEF_CP_A_R8(0xbb, e) /* CP A, E */
DEF_CP_A_R8(0xbc, h) /* CP A, H */
DEF_CP_A_R8(0xbd, l) /* CP A, L */
DEF_CP_A_HLPTR(0xbe) /* CP A, (HL) */
DEF_CP_A_R8(0xbf, a) /* CP A, A */

/* operations with immediate */
DEF_ADD_A_U8(0xc6) /* ADD A, u8 */
DEF_ADC_A_U8(0xce) /* ADC A, u8 */
DEF_SUB_A_U8(0xd6) /* SUB A, u8 */
DEF_SBC_A_U8(0xde) /* SBC A, u8 */
DEF_AND_A_U8(0xe6) /* AND A, u8 */
DEF_XOR_A_U8(0xee) /* XOR A, u8 */
DEF_OR_A_U8(0xf6)  /* OR A, u8 */
DEF_CP_A_U8(0xfe)  /* CP A, u8 */

/* ---- x16/alu ---- */
DEF_INC_R16(0x03, bc) /* INC BC */
DEF_INC_R16(0x13, de) /* INC DE */
DEF_INC_R16(0x23, hl) /* INC HL */
DEF_INC_R16(0x33, sp) /* INC SP */
DEF_DEC_R16(0x0b, bc) /* DEC BC */
DEF_DEC_R16(0x1b, de) /* DEC DE */
DEF_DEC_R16(0x2b, hl) /* DEC HL */
DEF_DEC_R16(0x3b, sp) /* DEC SP */

/* ----  x8/lsm ---- */
DEF_LD_R16PTR_R8(0x02, bc, a)     /* LD (BC), A */
DEF_LD_R8_U8(0x06, b)             /* LD B, u8 */
DEF_LD_R8_R16PTR(0x0a, a, bc)     /* LD A, (BC) */
DEF_LD_R8_U8(0x0e, c)             /* LD C, u8 */
DEF_LD_R16PTR_R8(0x12, de, a)     /* LD (DE), A */
DEF_LD_R8_U8(0x16, d)             /* LD D, u8 */
DEF_LD_R8_R16PTR(0x1a, a, de)     /* LD A, (DE) */
DEF_LD_R8_U8(0x1e, e)             /* LD E, u8 */
DEF_LD_R16PTR_INC_R8(0x22, hl, a) /* LD (HL+), A */
DEF_LD_R8_U8(0x26, h)             /* LD H, u8 */
DEF_LD_R8_R16PTR_INC(0x2a, a, hl) /* LD A, (HL+) */
DEF_LD_R8_U8(0x2e, l)             /* LD L, u8 */
DEF_LD_R16PTR_DEC_R8(0x32, hl, a) /* LD (HL-), A */
DEF_LD_R16PTR_U8(0x36, hl)        /* LD (HL), u8 */
DEF_LD_R8_R16PTR_DEC(0x3a, a, hl) /* LD A, (HL-) */
DEF_LD_R8_U8(0x3e, a)             /* LD A, u8 */

DEF_LD_R8_R8(0x40, b, b)      /* LD B, B */
DEF_LD_R8_R8(0x41, b, c)      /* LD B, C */
DEF_LD_R8_R8(0x42, b, d)      /* LD B, D */
DEF_LD_R8_R8(0x43, b, e)      /* LD B, E */
DEF_LD_R8_R8(0x44, b, h)      /* LD B, H */
DEF_LD_R8_R8(0x45, b, l)      /* LD B, L */
DEF_LD_R8_R16PTR(0x46, b, hl) /* LD B, (HL) */
DEF_LD_R8_R8(0x47, b, a)      /* LD B, A */
DEF_LD_R8_R8(0x48, c, b)      /* LD C, B */
DEF_LD_R8_R8(0x49, c, c)      /* LD C, C */
DEF_LD_R8_R8(0x4a, c, d)      /* LD C, D */
DEF_LD_R8_R8(0x4b, c, e)      /* LD C, E */
DEF_LD_R8_R8(0x4c, c, h)      /* LD C, H */
DEF_LD_R8_R8(0x4d, c, l)      /* LD C, L */
DEF_LD_R8_R16PTR(0x4e, c, hl) /* LD C, (HL) */
DEF_LD_R8_R8(0x4f, c, a)      /* LD C, A */

DEF_LD_R8_R8(0x50, d, b)      /* LD D, B */
DEF_LD_R8_R8(0x51, d, c)      /* LD D, C */
DEF_LD_R8_R8(0x52, d, d)      /* LD D, D */
DEF_LD_R8_R8(0x53, d, e)      /* LD D, E */
DEF_LD_R8_R8(0x54, d, h)      /* LD D, H */
DEF_LD_R8_R8(0x55, d, l)      /* LD D, L */
DEF_LD_R8_R16PTR(0x56, d, hl) /* LD D, (HL) */
DEF_LD_R8_R8(0x57, d, a)      /* LD D, A */
DEF_LD_R8_R8(0x58, e, b)      /* LD E, B */
DEF_LD_R8_R8(0x59, e, c)      /* LD E, C */
DEF_LD_R8_R8(0x5a, e, d)      /* LD E, D */
DEF_LD_R8_R8(0x5b, e, e)      /* LD E, E */
DEF_LD_R8_R8(0x5c, e, h)      /* LD E, H */
DEF_LD_R8_R8(0x5d, e, l)      /* LD E, L */
DEF_LD_R8_R16PTR(0x5e, e, hl) /* LD E, (HL) */
DEF_LD_R8_R8(0x5f, e, a)      /* LD E, A */

DEF_LD_R8_R8(0x60, h, b)      /* LD H, B */
DEF_LD_R8_R8(0x61, h, c)      /* LD H, C */
DEF_LD_R8_R8(0x62, h, d)      /* LD H, D */
DEF_LD_R8_R8(0x63, h, e)      /* LD H, E */
DEF_LD_R8_R8(0x64, h, h)      /* LD H, H */
DEF_LD_R8_R8(0x65, h, l)      /* LD H, L */
DEF_LD_R8_R16PTR(0x66, h, hl) /* LD H, (HL) */
DEF_LD_R8_R8(0x67, h, a)      /* LD H, A */
DEF_LD_R8_R8(0x68, l, b)      /* LD L, B */
DEF_LD_R8_R8(0x69, l, c)      /* LD L, C */
DEF_LD_R8_R8(0x6a, l, d)      /* LD L, D */
DEF_LD_R8_R8(0x6b, l, e)      /* LD L, E */
DEF_LD_R8_R8(0x6c, l, h)      /* LD L, H */
DEF_LD_R8_R8(0x6d, l, l)      /* LD L, L */
DEF_LD_R8_R16PTR(0x6e, l, hl) /* LD L, (HL) */
DEF_LD_R8_R8(0x6f, l, a)      /* LD L, A */

DEF_LD_R16PTR_R8(0x70, hl, b) /* LD (HL), B */
DEF_LD_R16PTR_R8(0x71, hl, c) /* LD (HL), C */
DEF_LD_R16PTR_R8(0x72, hl, d) /* LD (HL), D */
DEF_LD_R16PTR_R8(0x73, hl, e) /* LD (HL), E */
DEF_LD_R16PTR_R8(0x74, hl, h) /* LD (HL), H */
DEF_LD_R16PTR_R8(0x75, hl, l) /* LD (HL), L */
DEF_LD_R16PTR_R8(0x77, hl, a) /* LD (HL), A */

DEF_LD_R8_R8(0x78, a, b)      /* LD A, B */
DEF_LD_R8_R8(0x79, a, c)      /* LD A, C */
DEF_LD_R8_R8(0x7a, a, d)      /* LD A, D */
DEF_LD_R8_R8(0x7b, a, e)      /* LD A, E */
DEF_LD_R8_R8(0x7c, a, h)      /* LD A, H */
DEF_LD_R8_R8(0x7d, a, l)      /* LD A, L */
DEF_LD_R8_R16PTR(0x7e, a, hl) /* LD A, (HL) */
DEF_LD_R8_R8(0x7f, a, a)      /* LD A, A */
DEF_LD_FF00U8PTR_A(0xe0)      /* LD (FF00 + u8), A */
DEF_LD_U16PTR_R8(0xea, a)     /* LD (u16), A */
DEF_LD_R8_U16PTR(0xfa, a)     /* LD A, (u16) */
DEF_LD_A_FF00U8PTR(0xf0)      /* LD A, (FF00 + u8) */

/* ----  x16/lsm ---- */
DEF_LD_R16_U16(0x01, bc) /* LD BC, u16 */
DEF_LD_R16_U16(0x11, de) /* LD DE, u16 */
DEF_LD_R16_U16(0x21, hl) /* LD HL, u16 */
DEF_LD_R16_U16(0x31, sp) /* LD SP, u16 */
DEF_POP_R16(0xc1, bc)    /* POP BC */
DEF_POP_R16(0xd1, de)    /* POP DE */
DEF_POP_R16(0xe1, hl)    /* POP HL */
DEF_POP_R16(0xf1, af)    /* POP AF */
DEF_PUSH_R16(0xc5, bc)   /* PUSH BC */
DEF_PUSH_R16(0xd5, de)   /* PUSH DE */
DEF_PUSH_R16(0xe5, hl)   /* PUSH HL */
DEF_PUSH_R16(0xf5, af)   /* PUSH AF */

/* ----  control/branch ---- */
DEF_JR_U8(0x18)       /* JR (u8) */
DEF_JR_NZ_U8(0x20)    /* JR NZ, (u8) -> jump if z = 0 */
DEF_JR_Z_U8(0x28)     /* JR Z, (u8) -> jump if z = 1 */
DEF_JR_NC_U8(0x30)    /* JR NC, (u8) -> jump if c = 0 */
DEF_JR_C_U8(0x38)     /* JR C, (u8) -> jump if c = 1 */
DEF_JP(0xc3)          /* JP (u16) */
DEF_CALL_NZ_U16(0xc4) /* CALL NZ, (u16) -> call if z = 0 */
DEF_CALL_NC_U16(0xd4) /* CALL NC, (u16) -> call if c = 0 */
DEF_CALL_U16(0xcd)    /* CALL (u16) */
DEF_RET(0xc9)         /* RET */

/* ---- x8/rsb (cb-prefixed) ---- */
DEF_RLC_R8(0x00, b) /* RLC B */
DEF_RLC_R8(0x01, c) /* RLC C */
DEF_RLC_R8(0x02, d) /* RLC D */
DEF_RLC_R8(0x03, e) /* RLC E */
DEF_RLC_R8(0x04, h) /* RLC H */
DEF_RLC_R8(0x05, l) /* RLC L */
DEF_RLC_HLPTR(0x06) /* RLC (HL) */
DEF_RLC_R8(0x07, a) /* RLC A */

DEF_RRC_R8(0x08, b) /* RRC B */
DEF_RRC_R8(0x09, c) /* RRC C */
DEF_RRC_R8(0x0a, d) /* RRC D */
DEF_RRC_R8(0x0b, e) /* RRC E */
DEF_RRC_R8(0x0c, h) /* RRC H */
DEF_RRC_R8(0x0d, l) /* RRC L */
DEF_RRC_HLPTR(0x0e) /* RRC (HL) */
DEF_RRC_R8(0x0f, a) /* RRC A */

DEF_RL_R8(0x10, b) /* RL B */
DEF_RL_R8(0x11, c) /* RL C */
DEF_RL_R8(0x12, d) /* RL D */
DEF_RL_R8(0x13, e) /* RL E */
DEF_RL_R8(0x14, h) /* RL H */
DEF_RL_R8(0x15, l) /* RL L */
DEF_RL_HLPTR(0x16) /* RL (HL) */
DEF_RL_R8(0x17, a) /* RL A */

DEF_RR_R8(0x18, b) /* RR B */
DEF_RR_R8(0x19, c) /* RR C */
DEF_RR_R8(0x1a, d) /* RR D */
DEF_RR_R8(0x1b, e) /* RR E */
DEF_RR_R8(0x1c, h) /* RR H */
DEF_RR_R8(0x1d, l) /* RR L */
DEF_RR_HLPTR(0x1e) /* RR (HL) */
DEF_RR_R8(0x1f, a) /* RR A */

DEF_SLA_R8(0x20, b) /* SLA B */
DEF_SLA_R8(0x21, c) /* SLA C */
DEF_SLA_R8(0x22, d) /* SLA D */
DEF_SLA_R8(0x23, e) /* SLA E */
DEF_SLA_R8(0x24, h) /* SLA H */
DEF_SLA_R8(0x25, l) /* SLA L */
DEF_SLA_HLPTR(0x26) /* SLA (HL) */
DEF_SLA_R8(0x27, a) /* SLA A */

DEF_SRA_R8(0x28, b) /* SRA B */
DEF_SRA_R8(0x29, c) /* SRA C */
DEF_SRA_R8(0x2a, d) /* SRA D */
DEF_SRA_R8(0x2b, e) /* SRA E */
DEF_SRA_R8(0x2c, h) /* SRA H */
DEF_SRA_R8(0x2d, l) /* SRA L */
DEF_SRA_HLPTR(0x2e) /* SRA (HL) */
DEF_SRA_R8(0x2f, a) /* SRA A */

DEF_SWAP_R8(0x30, b) /* SWAP B */
DEF_SWAP_R8(0x31, c) /* SWAP C */
DEF_SWAP_R8(0x32, d) /* SWAP D */
DEF_SWAP_R8(0x33, e) /* SWAP E */
DEF_SWAP_R8(0x34, h) /* SWAP H */
DEF_SWAP_R8(0x35, l) /* SWAP L */
DEF_SWAP_HLPTR(0x36) /* SWAP (HL) */
DEF_SWAP_R8(0x37, a) /* SWAP A */

DEF_SRL_R8(0x38, b) /* SRL B */
DEF_SRL_R8(0x39, c) /* SRL C */
DEF_SRL_R8(0x3a, d) /* SRL D */
DEF_SRL_R8(0x3b, e) /* SRL E */
DEF_SRL_R8(0x3c, h) /* SRL H */
DEF_SRL_R8(0x3d, l) /* SRL L */
DEF_SRL_HLPTR(0x3e) /* SRL (HL) */
DEF_SRL_R8(0x3f, a) /* SRL A */

DEF_BIT_U3_R8(0x40, 0, b) /* BIT 0, B */
DEF_BIT_U3_R8(0x41, 0, c) /* BIT 0, C */
DEF_BIT_U3_R8(0x42, 0, d) /* BIT 0, D */
DEF_BIT_U3_R8(0x43, 0, e) /* BIT 0, E */
DEF_BIT_U3_R8(0x44, 0, h) /* BIT 0, H */
DEF_BIT_U3_R8(0x45, 0, l) /* BIT 0, L */
DEF_BIT_U3_HLPTR(0x46, 0) /* BIT 0, (HL) */
DEF_BIT_U3_R8(0x47, 0, a) /* BIT 0, A */

DEF_BIT_U3_R8(0x48, 1, b) /* BIT 1, B */
DEF_BIT_U3_R8(0x49, 1, c) /* BIT 1, C */
DEF_BIT_U3_R8(0x4a, 1, d) /* BIT 1, D */
DEF_BIT_U3_R8(0x4b, 1, e) /* BIT 1, E */
DEF_BIT_U3_R8(0x4c, 1, h) /* BIT 1, H */
DEF_BIT_U3_R8(0x4d, 1, l) /* BIT 1, L */
DEF_BIT_U3_HLPTR(0x4e, 1) /* BIT 1, (HL) */
DEF_BIT_U3_R8(0x4f, 1, a) /* BIT 1, A */
DEF_BIT_U3_R8(0x50, 2, b) /* BIT 2, B */
DEF_BIT_U3_R8(0x51, 2, c) /* BIT 2, C */
DEF_BIT_U3_R8(0x52, 2, d) /* BIT 2, D */

/* ----  control/misc ---- */
DEF_IMPLIED(0x00, nop, /* nothing */, 4)
DEF_IMPLIED(0xf3, di, cpu->ime = cpu->ime_next = 0;, 4)

void x8_alu_handler(CPU *cpu, uint8_t op) {
    switch (op) {
        case 0x04: op_0x04_i_b(cpu); break;
        case 0x05: op_0x05_d_b(cpu); break;
        case 0x0C: op_0x0c_i_c(cpu); break;
        case 0x0D: op_0x0d_d_c(cpu); break;
        case 0x14: op_0x14_i_d(cpu); break;
        case 0x15: op_0x15_d_d(cpu); break;
        case 0x1C: op_0x1c_i_e(cpu); break;
        case 0x1D: op_0x1d_d_e(cpu); break;
        case 0x24: op_0x24_i_h(cpu); break;
        case 0x25: op_0x25_d_h(cpu); break;
        case 0x2C: op_0x2c_i_l(cpu); break;
        case 0x2D: op_0x2d_d_l(cpu); break;
        case 0x34: op_0x34_i_hl(cpu); break;
        case 0x35: op_0x35_d_hl(cpu); break;
        case 0x3C: op_0x3c_i_a(cpu); break;
        case 0x3D: op_0x3d_d_a(cpu); break;
        case 0x80: op_0x80_add_a_b(cpu); break;
        case 0x81: op_0x81_add_a_c(cpu); break;
        case 0x82: op_0x82_add_a_d(cpu); break;
        case 0x83: op_0x83_add_a_e(cpu); break;
        case 0x84: op_0x84_add_a_h(cpu); break;
        case 0x85: op_0x85_add_a_l(cpu); break;
        case 0x86: op_0x86_add_a_hlptr(cpu); break;
        case 0x87: op_0x87_add_a_a(cpu); break;
        case 0x88: op_0x88_adc_a_b(cpu); break;
        case 0x89: op_0x89_adc_a_c(cpu); break;
        case 0x8A: op_0x8a_adc_a_d(cpu); break;
        case 0x8B: op_0x8b_adc_a_e(cpu); break;
        case 0x8C: op_0x8c_adc_a_h(cpu); break;
        case 0x8D: op_0x8d_adc_a_l(cpu); break;
        case 0x8E: op_0x8e_adc_a_hlptr(cpu); break;
        case 0x8F: op_0x8f_adc_a_a(cpu); break;
        case 0x90: op_0x90_sub_a_b(cpu); break;
        case 0x91: op_0x91_sub_a_c(cpu); break;
        case 0x92: op_0x92_sub_a_d(cpu); break;
        case 0x93: op_0x93_sub_a_e(cpu); break;
        case 0x94: op_0x94_sub_a_h(cpu); break;
        case 0x95: op_0x95_sub_a_l(cpu); break;
        case 0x96: op_0x96_sub_a_hlptr(cpu); break;
        case 0x97: op_0x97_sub_a_a(cpu); break;
        case 0x98: op_0x98_sbc_a_b(cpu); break;
        case 0x99: op_0x99_sbc_a_c(cpu); break;
        case 0x9A: op_0x9a_sbc_a_d(cpu); break;
        case 0x9B: op_0x9b_sbc_a_e(cpu); break;
        case 0x9C: op_0x9c_sbc_a_h(cpu); break;
        case 0x9D: op_0x9d_sbc_a_l(cpu); break;
        case 0x9E: op_0x9e_sbc_a_hlptr(cpu); break;
        case 0x9F: op_0x9f_sbc_a_a(cpu); break;
        case 0xA0: op_0xa0_and_a_b(cpu); break;
        case 0xA1: op_0xa1_and_a_c(cpu); break;
        case 0xA2: op_0xa2_and_a_d(cpu); break;
        case 0xA3: op_0xa3_and_a_e(cpu); break;
        case 0xA4: op_0xa4_and_a_h(cpu); break;
        case 0xA5: op_0xa5_and_a_l(cpu); break;
        case 0xA6: op_0xa6_and_a_hlptr(cpu); break;
        case 0xA7: op_0xa7_and_a_a(cpu); break;
        case 0xA8: op_0xa8_xor_a_b(cpu); break;
        case 0xA9: op_0xa9_xor_a_c(cpu); break;
        case 0xAA: op_0xaa_xor_a_d(cpu); break;
        case 0xAB: op_0xab_xor_a_e(cpu); break;
        case 0xAC: op_0xac_xor_a_h(cpu); break;
        case 0xAD: op_0xad_xor_a_l(cpu); break;
        case 0xAE: op_0xae_xor_a_hlptr(cpu); break;
        case 0xAF: op_0xaf_xor_a_a(cpu); break;
        case 0xB0: op_0xb0_or_a_b(cpu); break;
        case 0xB1: op_0xb1_or_a_c(cpu); break;
        case 0xB2: op_0xb2_or_a_d(cpu); break;
        case 0xB3: op_0xb3_or_a_e(cpu); break;
        case 0xB4: op_0xb4_or_a_h(cpu); break;
        case 0xB5: op_0xb5_or_a_l(cpu); break;
        case 0xB6: op_0xb6_or_a_hlptr(cpu); break;
        case 0xB7: op_0xb7_or_a_a(cpu); break;
        case 0xB8: op_0xb8_cp_a_b(cpu); break;
        case 0xB9: op_0xb9_cp_a_c(cpu); break;
        case 0xBA: op_0xba_cp_a_d(cpu); break;
        case 0xBB: op_0xbb_cp_a_e(cpu); break;
        case 0xBC: op_0xbc_cp_a_h(cpu); break;
        case 0xBD: op_0xbd_cp_a_l(cpu); break;
        case 0xBE: op_0xbe_cp_a_hlptr(cpu); break;
        case 0xBF: op_0xbf_cp_a_a(cpu); break;
        case 0xC6: op_0xc6_add_a_u8(cpu); break;
        case 0xCE: op_0xce_adc_a_u8(cpu); break;
        case 0xD6: op_0xd6_sub_a_u8(cpu); break;
        case 0xDE: op_0xde_sbc_a_u8(cpu); break;
        case 0xE6: op_0xe6_and_a_u8(cpu); break;
        case 0xEE: op_0xee_xor_a_u8(cpu); break;
        case 0xF6: op_0xf6_or_a_u8(cpu); break;
        case 0xFE: op_0xfe_cp_a_u8(cpu); break;
    }
}

void x16_alu_handler(CPU *cpu, uint8_t op) {
    switch (op) {
        case 0x03: op_0x03_i_bc(cpu); break;
        case 0x13: op_0x13_i_de(cpu); break;
        case 0x23: op_0x23_i_hl(cpu); break;
        case 0x33: op_0x33_i_sp(cpu); break;
        case 0x0B: op_0x0b_d_bc(cpu); break;
        case 0x1B: op_0x1b_d_de(cpu); break;
        case 0x2B: op_0x2b_d_hl(cpu); break;
        case 0x3B: op_0x3b_d_sp(cpu); break;
    }
}

void x8_lsm_handler(CPU *cpu, uint8_t op) {
    switch (op) {
        case 0x02: op_0x02_ld_bcptr_a(cpu); break;
        case 0x06: op_0x06_ld_b_u8(cpu); break;
        case 0x0A: op_0x0a_ld_a_bcptr(cpu); break;
        case 0x0E: op_0x0e_ld_c_u8(cpu); break;
        case 0x12: op_0x12_ld_deptr_a(cpu); break;
        case 0x16: op_0x16_ld_d_u8(cpu); break;
        case 0x1A: op_0x1a_ld_a_deptr(cpu); break;
        case 0x1E: op_0x1e_ld_e_u8(cpu); break;
        case 0x22: op_0x22_ld_hlptr_i_a(cpu); break;
        case 0x26: op_0x26_ld_h_u8(cpu); break;
        case 0x2A: op_0x2a_ld_a_hlptr_i(cpu); break;
        case 0x2E: op_0x2e_ld_l_u8(cpu); break;
        case 0x32: op_0x32_ld_hlptr_d_a(cpu); break;
        case 0x36: op_0x36_ld_hlptr_u8(cpu); break;
        case 0x3A: op_0x3a_ld_a_hlptr_d(cpu); break;
        case 0x3E: op_0x3e_ld_a_u8(cpu); break;
        case 0x40: op_0x40_ld_b_b(cpu); break;
        case 0x41: op_0x41_ld_b_c(cpu); break;
        case 0x42: op_0x42_ld_b_d(cpu); break;
        case 0x43: op_0x43_ld_b_e(cpu); break;
        case 0x44: op_0x44_ld_b_h(cpu); break;
        case 0x45: op_0x45_ld_b_l(cpu); break;
        case 0x46: op_0x46_ld_b_hlptr(cpu); break;
        case 0x47: op_0x47_ld_b_a(cpu); break;
        case 0x48: op_0x48_ld_c_b(cpu); break;
        case 0x49: op_0x49_ld_c_c(cpu); break;
        case 0x4A: op_0x4a_ld_c_d(cpu); break;
        case 0x4B: op_0x4b_ld_c_e(cpu); break;
        case 0x4C: op_0x4c_ld_c_h(cpu); break;
        case 0x4D: op_0x4d_ld_c_l(cpu); break;
        case 0x4E: op_0x4e_ld_c_hlptr(cpu); break;
        case 0x4F: op_0x4f_ld_c_a(cpu); break;
        case 0x50: op_0x50_ld_d_b(cpu); break;
        case 0x51: op_0x51_ld_d_c(cpu); break;
        case 0x52: op_0x52_ld_d_d(cpu); break;
        case 0x53: op_0x53_ld_d_e(cpu); break;
        case 0x54: op_0x54_ld_d_h(cpu); break;
        case 0x55: op_0x55_ld_d_l(cpu); break;
        case 0x56: op_0x56_ld_d_hlptr(cpu); break;
        case 0x57: op_0x57_ld_d_a(cpu); break;
        case 0x58: op_0x58_ld_e_b(cpu); break;
        case 0x59: op_0x59_ld_e_c(cpu); break;
        case 0x5A: op_0x5a_ld_e_d(cpu); break;
        case 0x5B: op_0x5b_ld_e_e(cpu); break;
        case 0x5C: op_0x5c_ld_e_h(cpu); break;
        case 0x5D: op_0x5d_ld_e_l(cpu); break;
        case 0x5E: op_0x5e_ld_e_hlptr(cpu); break;
        case 0x5F: op_0x5f_ld_e_a(cpu); break;
        case 0x60: op_0x60_ld_h_b(cpu); break;
        case 0x61: op_0x61_ld_h_c(cpu); break;
        case 0x62: op_0x62_ld_h_d(cpu); break;
        case 0x63: op_0x63_ld_h_e(cpu); break;
        case 0x64: op_0x64_ld_h_h(cpu); break;
        case 0x65: op_0x65_ld_h_l(cpu); break;
        case 0x66: op_0x66_ld_h_hlptr(cpu); break;
        case 0x67: op_0x67_ld_h_a(cpu); break;
        case 0x68: op_0x68_ld_l_b(cpu); break;
        case 0x69: op_0x69_ld_l_c(cpu); break;
        case 0x6A: op_0x6a_ld_l_d(cpu); break;
        case 0x6B: op_0x6b_ld_l_e(cpu); break;
        case 0x6C: op_0x6c_ld_l_h(cpu); break;
        case 0x6D: op_0x6d_ld_l_l(cpu); break;
        case 0x6E: op_0x6e_ld_l_hlptr(cpu); break;
        case 0x6F: op_0x6f_ld_l_a(cpu); break;
        case 0x70: op_0x70_ld_hlptr_b(cpu); break;
        case 0x71: op_0x71_ld_hlptr_c(cpu); break;
        case 0x72: op_0x72_ld_hlptr_d(cpu); break;
        case 0x73: op_0x73_ld_hlptr_e(cpu); break;
        case 0x74: op_0x74_ld_hlptr_h(cpu); break;
        case 0x75: op_0x75_ld_hlptr_l(cpu); break;
        case 0x77: op_0x77_ld_hlptr_a(cpu); break;
        case 0x78: op_0x78_ld_a_b(cpu); break;
        case 0x79: op_0x79_ld_a_c(cpu); break;
        case 0x7A: op_0x7a_ld_a_d(cpu); break;
        case 0x7B: op_0x7b_ld_a_e(cpu); break;
        case 0x7C: op_0x7c_ld_a_h(cpu); break;
        case 0x7D: op_0x7d_ld_a_l(cpu); break;
        case 0x7E: op_0x7e_ld_a_hlptr(cpu); break;
        case 0x7F: op_0x7f_ld_a_a(cpu); break;
        case 0xE0: op_0xe0_ld_ff00u8ptr_a(cpu); break;
        case 0xEA: op_0xea_ld_u16ptr_a(cpu); break;
        case 0xF0: op_0xf0_ld_a_ff00u8ptr(cpu); break;
        case 0xFA: op_0xfa_ld_a_u16ptr(cpu); break;
    }
}

void x16_lsm_handler(CPU *cpu, uint8_t op) {
    switch (op) {
        case 0x01: op_0x01_ld_bc_u16(cpu); break;
        case 0x11: op_0x11_ld_de_u16(cpu); break;
        case 0x21: op_0x21_ld_hl_u16(cpu); break;
        case 0x31: op_0x31_ld_sp_u16(cpu); break;
        case 0xC1: op_0xc1_pop_bc(cpu); break;
        case 0xD1: op_0xd1_pop_de(cpu); break;
        case 0xE1: op_0xe1_pop_hl(cpu); break;
        case 0xF1: op_0xf1_pop_af(cpu); break;
        case 0xC5: op_0xc5_push_bc(cpu); break;
        case 0xD5: op_0xd5_push_de(cpu); break;
        case 0xE5: op_0xe5_push_hl(cpu); break;
        case 0xF5: op_0xf5_push_af(cpu); break;
    }
}

void ctrl_br_handler(CPU *cpu, uint8_t op) {
    switch (op) {
        case 0x18: op_0x18_jr_u8(cpu); break;
        case 0x20: op_0x20_jr_nz_u8(cpu); break;
        case 0x28: op_0x28_jr_z_u8(cpu); break;
        case 0x30: op_0x30_jr_nc_u8(cpu); break;
        case 0x38: op_0x38_jr_c_u8(cpu); break;
        case 0xC3: op_0xc3_jp(cpu); break;
        case 0xC4: op_0xc4_call_nz_u16(cpu); break;
        case 0xCD: op_0xcd_call_u16(cpu); break;
        case 0xD4: op_0xd4_call_nc_u16(cpu); break;
        case 0xC9: op_0xc9_ret(cpu); break;
    }
}

void x8_rsb_handler(CPU *cpu, uint8_t op) {
    switch (op) {
        case 0x00: op_0x00_rlc_b(cpu); break;
        case 0x01: op_0x01_rlc_c(cpu); break;
        case 0x02: op_0x02_rlc_d(cpu); break;
        case 0x03: op_0x03_rlc_e(cpu); break;
        case 0x04: op_0x04_rlc_h(cpu); break;
        case 0x05: op_0x05_rlc_l(cpu); break;
        case 0x06: op_0x06_rlc_hlptr(cpu); break;
        case 0x07: op_0x07_rlc_a(cpu); break;
        case 0x08: op_0x08_rrc_b(cpu); break;
        case 0x09: op_0x09_rrc_c(cpu); break;
        case 0x0A: op_0x0a_rrc_d(cpu); break;
        case 0x0B: op_0x0b_rrc_e(cpu); break;
        case 0x0C: op_0x0c_rrc_h(cpu); break;
        case 0x0D: op_0x0d_rrc_l(cpu); break;
        case 0x0E: op_0x0e_rrc_hlptr(cpu); break;
        case 0x0F: op_0x0f_rrc_a(cpu); break;
        case 0x10: op_0x10_rl_b(cpu); break;
        case 0x11: op_0x11_rl_c(cpu); break;
        case 0x12: op_0x12_rl_d(cpu); break;
        case 0x13: op_0x13_rl_e(cpu); break;
        case 0x14: op_0x14_rl_h(cpu); break;
        case 0x15: op_0x15_rl_l(cpu); break;
        case 0x16: op_0x16_rl_hlptr(cpu); break;
        case 0x17: op_0x17_rl_a(cpu); break;
        case 0x18: op_0x18_rr_b(cpu); break;
        case 0x19: op_0x19_rr_c(cpu); break;
        case 0x1A: op_0x1a_rr_d(cpu); break;
        case 0x1B: op_0x1b_rr_e(cpu); break;
        case 0x1C: op_0x1c_rr_h(cpu); break;
        case 0x1D: op_0x1d_rr_l(cpu); break;
        case 0x1E: op_0x1e_rr_hlptr(cpu); break;
        case 0x1F: op_0x1f_rr_a(cpu); break;
        case 0x20: op_0x20_sla_b(cpu); break;
        case 0x21: op_0x21_sla_c(cpu); break;
        case 0x22: op_0x22_sla_d(cpu); break;
        case 0x23: op_0x23_sla_e(cpu); break;
        case 0x24: op_0x24_sla_h(cpu); break;
        case 0x25: op_0x25_sla_l(cpu); break;
        case 0x26: op_0x26_sla_hlptr(cpu); break;
        case 0x27: op_0x27_sla_a(cpu); break;
        case 0x28: op_0x28_sra_b(cpu); break;
        case 0x29: op_0x29_sra_c(cpu); break;
        case 0x2A: op_0x2a_sra_d(cpu); break;
        case 0x2B: op_0x2b_sra_e(cpu); break;
        case 0x2C: op_0x2c_sra_h(cpu); break;
        case 0x2D: op_0x2d_sra_l(cpu); break;
        case 0x2E: op_0x2e_sra_hlptr(cpu); break;
        case 0x2F: op_0x2f_sra_a(cpu); break;
        case 0x30: op_0x30_swap_b(cpu); break;
        case 0x31: op_0x31_swap_c(cpu); break;
        case 0x32: op_0x32_swap_d(cpu); break;
        case 0x33: op_0x33_swap_e(cpu); break;
        case 0x34: op_0x34_swap_h(cpu); break;
        case 0x35: op_0x35_swap_l(cpu); break;
        case 0x36: op_0x36_swap_hlptr(cpu); break;
        case 0x37: op_0x37_swap_a(cpu); break;
        case 0x38: op_0x38_srl_b(cpu); break;
        case 0x39: op_0x39_srl_c(cpu); break;
        case 0x3A: op_0x3a_srl_d(cpu); break;
        case 0x3B: op_0x3b_srl_e(cpu); break;
        case 0x3C: op_0x3c_srl_h(cpu); break;
        case 0x3D: op_0x3d_srl_l(cpu); break;
        case 0x3E: op_0x3e_srl_hlptr(cpu); break;
        case 0x3F: op_0x3f_srl_a(cpu); break;
        default:   log_cpu_error(cpu, "unimplemented CB opcode: 0x%02X", op);
    }
}

void ctrl_misc_handler(CPU *cpu, uint8_t op) {
    switch (op) {
        case 0x00: op_0x00_nop(cpu); break;
        case 0xF3: op_0xf3_di(cpu); break;
    }
}

void decode_and_execute(CPU *cpu, uint8_t op) {
    switch (op) {
        /* ---- x8/alu ---- */
        x8_alu_handler(cpu, op);

        /* ---- x16/alu ---- */
        x16_alu_handler(cpu, op);

        /* ---- x8/lsm ---- */
        x8_lsm_handler(cpu, op);

        /* ---- x16/lsm ---- */
        x16_lsm_handler(cpu, op);

        /* ---- ctrl/br ---- */
        ctrl_br_handler(cpu, op);

        /* ---- ctrl/misc ---- */
        ctrl_misc_handler(cpu, op);

        /* ---- cb-prefixed ---- */
        case 0xCB: {
            uint8_t cb_op =
                mem_read(cpu->pc++); /* fetch the CB‐opcode (the second byte) and advance PC */

            x8_rsb_handler(cpu, cb_op);
            break;
        }

        /* ---- unimplemented / illegal ---- */
        default: log_cpu_error(cpu, "unimplemented opcode: 0x%02X", op); break;
    }
}
