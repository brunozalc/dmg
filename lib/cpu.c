#include "cpu.h"

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opcodes.h"

// FILE *cpu_og = NULL;

/* function to reset and initialize the CPU
- sets all regular regs to zero
- puts pc and sp at their designated place
*/
void cpu_init(struct CPU *cpu, struct MMU *mmu, struct Timer *timer, struct PPU *ppu) {
    *cpu       = (CPU){0};

    cpu->mmu   = mmu;
    cpu->timer = timer;
    cpu->ppu   = ppu;

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

/* function to tick the emulator components */
void tick(CPU *cpu, int cycles) {
    cpu->cycles += cycles;
    timer_step(cpu->timer, cycles); /* update the timer */
    ppu_step(cpu->ppu, cycles);     /* update the PPU */
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
    tick(cpu, 20);
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
        uint8_t flagged_and_enabled = cpu->ifr & cpu->ier & 0x1F;  // IE & IF

        if (cpu->ime && flagged_and_enabled) {
            /* IME = 1  and an interrupt is ready → leave HALT and service it */
            cpu->halt = 0;
            interrupt_servicing_routine(cpu);
            return;

        } else if (!cpu->ime && flagged_and_enabled) {
            /* IME = 0  and IE&IF ≠ 0 → trigger HALT-bug (skip next PC increment) */
            cpu->halt     = 0;
            cpu->halt_bug = 1;
            /* fall through to normal fetch/execute */

        } else {
            /* IME = 0 and   IE&IF = 0   (or)   no IF bits at all → stay halted */
            tick(cpu, 4);
            return;
        }
    }

    /* acknowledge pending interrupts */
    if (cpu->ime) {
        interrupt_servicing_routine(cpu);
    }

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
