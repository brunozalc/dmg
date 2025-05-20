#ifndef MMU_HEADER
#define MMU_HEADER

#include <stdint.h>

struct CPU;
struct Timer;
// struct PPU PPU; (TODO)

typedef struct MMU {
    struct CPU *cpu;      // pointer to the CPU
    struct Timer *timer;  // pointer to the timer
    // PPU *ppu;  // pointer to the PPU (TODO)

    /* cartridge rom banks */
    uint8_t rom[0x8000];  // 0000h - 7FFFh (without bank switching)
    // uint8_t rom_bank0[0x4000];  // 16KB 0000h - 3FFFh (fixed)
    // uint8_t rom_bankx[0x4000];  // 16KB 4000h - 7FFFh (switchable)

    /* video ram */
    uint8_t vram[0x2000];  // 8000h - 9FFFh

    /* external (cartdrige) ram */
    uint8_t eram[0x2000];  // A000h - BFFFh

    /* work ram */
    uint8_t wram[0x2000];  // C000h - DFFFh

    // echo ram is prohibited (according to nintendo)

    /* oam (object attribute memory) */
    uint8_t oam[0x00A0];  // FE00h - FE9Fh

    /* i/o registers */
    uint8_t io[0x0080];  // FF00h - FF7Fh

    /* high ram */
    uint8_t hram[0x007F];  // FF80h - FFFFh

    // MBC *mbc;  // memory bank controller (TODO)

} MMU;

// initialize and reset the MMU
void mmu_init(MMU *mmu, struct CPU *cpu, struct Timer *timer);
void mmu_reset(MMU *mmu);

// read a 8bit value from the memory bus
uint8_t mmu_read(MMU *mmu, uint16_t addr);

// read a 16bit value from the memory bus
uint16_t mmu_read16(MMU *mmu, uint16_t addr);

// write a 8bit value to the memory bus
void mmu_write(MMU *mmu, uint16_t addr, uint8_t value);

// write a 16bit value to the memory bus
void mmu_write16(MMU *mmu, uint16_t addr, uint16_t value);

#endif
