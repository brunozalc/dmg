#include "cpu.h"

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bus.h"
#include "opcodes.h"

/* function to entirely reset the CPU
- sets all regular regs to zero
- puts pc and sp at their designated place
*/
void cpu_reset(CPU *cpu) {
    *cpu    = (CPU){0};
    cpu->pc = 0x0100;
    cpu->sp = 0xFFFE;
}

/* function to log an error
- prints information about the CPU state when the error occurred
*/
void log_cpu_error(CPU *cpu, const char *format, ...) {
    va_list args;
    va_start(args, format);

    fprintf(stderr, "\n=== error ===\n");
    fprintf(stderr, "pc: 0x%04X\n",
            cpu->pc); 
    fprintf(stderr, "opcode: 0x%02X\n", cpu->last_opcode);
    fprintf(stderr, "error: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    // print CPU state
    fprintf(stderr, "\nCPU state:\n");
    fprintf(stderr, "A: 0x%02X  F: 0x%02X\n", cpu->a, cpu->f);
    fprintf(stderr, "B: 0x%02X  C: 0x%02X\n", cpu->b, cpu->c);
    fprintf(stderr, "D: 0x%02X  E: 0x%02X\n", cpu->d, cpu->e);
    fprintf(stderr, "H: 0x%02X  L: 0x%02X\n", cpu->h, cpu->l);
    fprintf(stderr, "sp: 0x%04X\n", cpu->sp);
    fprintf(stderr, "cycles: %llu\n", cpu->cycles);
    fprintf(stderr, "===================\n\n");

    va_end(args);
    exit(1);
}

/* fetch-decode-execute cycle
- fetch the next instruction from the memory bus (address indicated by pc)
*/
static uint8_t fetch(CPU *cpu) {
    uint8_t opcode = mem_read(cpu->pc++); /* increment the program counter to point to the next
                                          instruction (length=1) OR the next operand (length>1) */

    /* simple execution log */
    if (opcode != cpu->last_opcode)
        fprintf(stderr, "pc: %04X opcode: %02X\n", cpu->pc - 1, opcode);

    return cpu->last_opcode = opcode;
}

void cpu_step(CPU *cpu) {
    uint8_t opcode = fetch(cpu);
    decode_and_execute(cpu, opcode);

    /* pass the signal to ime after one instruction has passed.
    see https://gbdev.io/pandocs/Interrupts.html */
    if (cpu->ime_next) {
        cpu->ime      = 1;
        cpu->ime_next = 0;
    }
}
