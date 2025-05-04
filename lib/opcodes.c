#include "opcodes.h"

#include <string.h>

static void op_00_nop(CPU *cpu) { cpu->cycles += 4; }

const opfn optable[256] = {
    op_00_nop, NULL
    // TODO: implement more opcodes
};
