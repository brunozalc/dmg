#include "timer.h"

#include "cpu.h"
#include "mmu.h"

/* helpers */
/* map TAC bits 1:0 to DIV bit numbers */
static inline uint8_t selected_div_bit(const Timer *t) {
    /* 00 -> 9, 01 -> 3, 10 -> 5, 11 -> 7 */
    static const uint8_t div_bit_map[4] = {9, 3, 5, 7};
    return div_bit_map[t->tac & 0x03];
}

static inline void request_interrupt(Timer *t) {
    /* request timer interrupt */
    t->cpu->ifr |= 0x04; /* set bit 2 of IF register */
}

void timer_init(Timer *timer, struct CPU *cpu, struct MMU *mmu) {
    timer->cpu = cpu;
    timer->mmu = mmu;

    timer_reset(timer);
}

void timer_reset(Timer *timer) {
    timer->div            = 0;
    timer->tima           = 0;
    timer->tma            = 0;
    timer->tac            = 0xF8;

    timer->prev_div_bit   = 0;
    timer->overflow_phase = 0xFF; /* idle */
}

/* called from advance_cycles in opcodes.h
cycles = 4, 8, 12, 16... */
void timer_step(Timer *timer, uint8_t cycles) {
    while (cycles--) {
        /* 1. increment DIV, our system counter */
        timer->div++;

        /* 2. falling edge detector (only if TAC is enabled -> bit 2) */
        if (timer->tac & 0x04) {
            uint8_t bit = (timer->div >> selected_div_bit(timer)) &
                          0x01;  // get the selected bit (mux selector)
            uint8_t falling_edge = (timer->prev_div_bit == 1) && bit == 0;  // falling edge detector
            timer->prev_div_bit  = bit;  // update the previous bit

            if (falling_edge && timer->overflow_phase == 0xFF) {
                if (timer->tima == 0xFF) {
                    timer->tima           = 0x00;
                    timer->overflow_phase = 0x00;  // overflow phase
                } else {
                    timer->tima++;
                }
            }
        }

        /* 3. handle overflow */
        if (timer->overflow_phase != 0xFF) {
            switch (timer->overflow_phase) {
                case 1: timer->tima = timer->tma; break;     /* load tma into tima */
                case 2: request_interrupt(timer); break;     /* request interrupt */
                case 3: timer->overflow_phase = 0xFF; break; /* done */
            }
            timer->overflow_phase++;
        }
    }
}

void timer_write_div(Timer *t) {
    t->div          = 0;
    t->prev_div_bit = 0;
}

void timer_write_tima(Timer *t, uint8_t value) {
    if (t->overflow_phase < 2) {
        /* abort overflow */
        t->overflow_phase = 0xFF;
        t->tima           = value;
    } else if (t->overflow_phase == 2) {
        t->tima = value;
    } else {
        t->tima = value;
    }
}

void timer_write_tma(Timer *t, uint8_t value) { t->tma = value; }

void timer_write_tac(Timer *t, uint8_t value) {
    t->tac          = value | 0xF8;

    t->prev_div_bit = (t->div >> selected_div_bit(t)) & 0x01; /* update the previous bit */
}
