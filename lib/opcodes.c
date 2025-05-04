/* check https://izik1.github.io/gbops/index.html */

#include "opcodes.h"

#include <string.h>

#include "bus.h"
#include "cpu.h"

static void op_00_nop(CPU *cpu) { advance_cycles(cpu, 4); }

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

static void op_21_ld_hl_nn(CPU *cpu) {
    uint16_t addr = mem_read16(cpu->pc);
    cpu->hl       = addr;
    advance_pc(cpu, 2);
    advance_cycles(cpu, 12);
}

static void op_47_ld_b_a(CPU *cpu) {
    cpu->b = cpu->a;
    advance_cycles(cpu, 4);
}

static void op_c3_jp(CPU *cpu) {
    uint16_t addr = mem_read16(cpu->pc);
    cpu->pc       = addr;
    advance_cycles(cpu, 16);
}

// a table of opfn-type functions
// table lookup has O(1) complexity and looks better than a huge switch
// statement
const opfn optable[256] = {
    [0x00] = op_00_nop,      [0x0E] = op_0e_ld_c_n, [0x11] = op_11_ld_de_nn,
    [0x21] = op_21_ld_hl_nn, [0x47] = op_47_ld_b_a, [0xC3] = op_c3_jp,
    // TODO: implement more opcodes
};
