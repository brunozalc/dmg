/* check https://izik1.github.io/gbops/index.html */

#include "opcodes.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bus.h"
#include "cpu.h"

static void op_00_nop(CPU *cpu) { advance_cycles(cpu, 4); }

static void op_0d_dec_c(CPU *cpu) {
    uint8_t old = cpu->c;
    cpu->c      = --old;

    set_flag(cpu, FLAG_Z, cpu->c == 0);
    set_flag(cpu, FLAG_N, 1);
    set_flag(cpu, FLAG_H,
             (old & 0x0F) == 0x00); /* is the low-nibble = 0000? if yes, the
                                          decrement will have a half borrow */
    advance_cycles(cpu, 4);
}

static void op_0e_ld_c_n(CPU *cpu) {
    cpu->c = mem_read(cpu->pc);
    advance_pc(cpu, 1);
    advance_cycles(cpu, 8);
}

static void op_11_ld_de_nn(CPU *cpu) {
    uint16_t addr = mem_read16(cpu->pc);
    cpu->de       = addr;
    advance_pc(cpu, 2);
    advance_cycles(cpu, 12);
}

static void op_12_ld_de_a(CPU *cpu) {
    mem_write(cpu->de, cpu->a);
    advance_cycles(cpu, 8);
}

static void op_14_inc_d(CPU *cpu) {
    uint8_t old = cpu->d;
    cpu->d      = ++old;

    set_flag(cpu, FLAG_Z, cpu->d == 0);
    set_flag(cpu, FLAG_N, 0);
    set_flag(cpu, FLAG_H,
             (old & 0x0F) == 0x0F); /* is the low-nibble = 1111? if yes, the
                                          increment will have a half carry */
    advance_cycles(cpu, 4);
}

static void op_1c_inc_e(CPU *cpu) {
    uint8_t old = cpu->e;
    cpu->e      = ++old;

    set_flag(cpu, FLAG_Z, cpu->e == 0);
    set_flag(cpu, FLAG_N, 0);
    set_flag(cpu, FLAG_H,
             (old & 0x0F) == 0x0F); /* is the low-nibble = 1111? if yes, the
                                          increment will have a half carry */
    advance_cycles(cpu, 4);
}

static void op_20_jr_nz(CPU *cpu) {
    int8_t offset = (int8_t)mem_read(cpu->pc); /* signed +/-128 */
    advance_pc(cpu, 1);                        /* skip operand */
    if (!get_flag(cpu, FLAG_Z)) {
        cpu->pc += offset;
        advance_cycles(cpu, 12);
    } else {
        advance_cycles(cpu, 8);
    }
}

static void op_21_ld_hl_nn(CPU *cpu) {
    uint16_t addr = mem_read16(cpu->pc);
    cpu->hl       = addr;
    advance_pc(cpu, 2);
    advance_cycles(cpu, 12);
}

static void op_2a_ld_a_hli(CPU *cpu) {
    cpu->a = mem_read(cpu->hl);
    cpu->hl++;
    advance_cycles(cpu, 8);
}

static void op_31_ld_sp_nn(CPU *cpu) {
    uint16_t addr = mem_read16(cpu->pc);
    cpu->sp       = addr;
    advance_pc(cpu, 3);
    advance_cycles(cpu, 12);
}

static void op_47_ld_b_a(CPU *cpu) {
    cpu->b = cpu->a;
    advance_cycles(cpu, 4);
}

static void op_78_ld_a_b(CPU *cpu) {
    cpu->a = cpu->b;
    advance_cycles(cpu, 4);
}

static void op_c3_jp(CPU *cpu) {
    uint16_t addr = mem_read16(cpu->pc);
    cpu->pc       = addr;
    advance_cycles(cpu, 16);
}

static void op_f3_di(CPU *cpu) {
    cpu->ime      = 0;
    cpu->ime_next = 0;
    advance_cycles(cpu, 4);
}

// a table of opfn-type functions
// table lookup has O(1) complexity and looks better than a huge switch
// statement
const opfn optable[256] = {
    [0x00] = op_00_nop,      [0x0D] = op_0d_dec_c,    [0x0E] = op_0e_ld_c_n,
    [0x11] = op_11_ld_de_nn, [0x12] = op_12_ld_de_a,  [0x14] = op_14_inc_d,
    [0x1C] = op_1c_inc_e,    [0x20] = op_20_jr_nz,    [0x21] = op_21_ld_hl_nn,
    [0x2A] = op_2a_ld_a_hli, [0x31] = op_31_ld_sp_nn, [0x47] = op_47_ld_b_a,
    [0x78] = op_78_ld_a_b,   [0xC3] = op_c3_jp,       [0xF3] = op_f3_di,
};
