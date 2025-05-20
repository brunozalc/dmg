#include "cpu.h"

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opcodes.h"

FILE *cpu_log = NULL;

static void log_cpu_state(CPU *cpu) {
    uint8_t b0 = mmu_read(cpu->mmu, cpu->pc);
    uint8_t b1 = mmu_read(cpu->mmu, cpu->pc + 1);
    uint8_t b2 = mmu_read(cpu->mmu, cpu->pc + 2);
    uint8_t b3 = mmu_read(cpu->mmu, cpu->pc + 3);

    fprintf(cpu_log,
            "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X "
            "SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X\n",
            cpu->a, cpu->f, /* <-- F is raw byte */
            cpu->b, cpu->c, cpu->d, cpu->e, cpu->h, cpu->l, cpu->sp, cpu->pc, b0, b1, b2, b3);

    fflush(cpu_log);
}

/* function to entirely reset the CPU
- sets all regular regs to zero
- puts pc and sp at their designated place
*/
void cpu_init(CPU *cpu, MMU *mmu, Timer *timer) {
    *cpu       = (CPU){0};

    cpu->mmu   = mmu;
    cpu->timer = timer;

    /* debug register init */
    cpu->a     = 0x01;
    cpu->b     = 0x00;
    cpu->c     = 0x13;
    cpu->d     = 0x00;
    cpu->e     = 0xD8;
    cpu->f     = 0xB0; /* 0b10110000 */
    cpu->h     = 0x01;
    cpu->l     = 0x4D;

    cpu->pc    = 0x0100;
    cpu->sp    = 0xFFFE;
}

/* function to process an interrupt
- sets the program counter to the address of the interrupt
- pushes the current program counter on the stack
- sets the program counter to the address of the interrupt
*/
static void interrupt_servicing_routine(CPU *cpu) {
    uint8_t pending =
        cpu->ifr & cpu->ier & 0x1F; /* checks the 5 bits for pending interrupts. for example,
                                       for a v-blank interrupt, pending = 0b00000001 */
    if (cpu->ime == 0 || pending == 0)
        return; /* interrupts disabled or no pending interrupts */

    int id = __builtin_ctz(pending); /* get the index of the first set bit. for example, if pending
                                        is 0b00001000, id = 3 */

    cpu->ifr &= ~(1 << id); /* clear the interrupt flag. this acks the interrupt */
    cpu->ime       = 0;     /* disable interrupts */
    cpu->ime_delay = 0;     /* abort any pending EIs */

    uint16_t ret   = cpu->pc; /* save the current program counter */
    cpu->sp--;
    mem_write(cpu->sp, (ret >> 8) & 0xFF); /* push the high byte */
    cpu->sp--;
    mem_write(cpu->sp, ret & 0xFF); /* push the low byte */
    cpu->pc = 0x0040 + (id * 8);    /* set the program counter to the interrupt vector */
    advance_cycles(cpu, 20);
}

/* fetch-decode-execute cycle
- fetch the next instruction from the memory bus (address indicated by pc)
*/
static uint8_t fetch(CPU *cpu) {
    uint8_t opcode = mem_read(cpu->pc);
    if (!cpu->halt_bug)
        cpu->pc++; /* increment the program counter to point to the next
                          instruction (length=1) OR the next operand (length>1) */
    else
        cpu->halt_bug = 0; /* reset the halt bug */

    return opcode;
}

void cpu_step(CPU *cpu) {

    /* handle halt */
    if (cpu->halt == 1) {
        uint8_t pending = cpu->ifr & cpu->ier & 0x1F;

        if (cpu->ime && pending) {
            cpu->halt = 0;
            interrupt_servicing_routine(cpu);
        } else if (!cpu->ime && pending) {
            cpu->halt     = 0;
            cpu->halt_bug = 1; /* supress pc++ once */
        } else {
            advance_cycles(cpu, 4);
            return;
        }
    }

    log_cpu_state(cpu); /* log the previous CPU state */
    /* acknowledge pending interrupts */
    interrupt_servicing_routine(cpu);


    /* fetch the next instruction */
    uint8_t opcode = fetch(cpu);
    decode_and_execute(cpu, opcode);
    
    /* pass the signal to ime from the previous instruction
    see https://gbdev.io/pandocs/Interrupts.html */
    if (cpu->ime_delay) {
        cpu->ime_delay--; /* 2 → 1   (during instr N+1 fetch) */
        if (cpu->ime_delay == 0)
            cpu->ime = 1; /* becomes 1 *after* instr N+1 exec */
    }
}
