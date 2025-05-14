#ifndef MMU_HEADER
#define MMU_HEADER

#include <stdint.h>

struct CPU;
// typedef struct PPU PPU; (TODO)
// typedef struct Timer Timer; (TODO)

// MMU as a struct (new MMU design)
typedef struct MMU {
    struct CPU *cpu;  // pointer to the CPU
    // PPU *ppu;  // pointer to the PPU (TODO)
    // Timer *timer; // pointer to the timer (TODO)

    // memory regions
    // for now, using a single array for all memory (64 KB)
    uint8_t ram[0x10000];  // 64KB of RAM

    // later, we can add more memory regions for VRAM, OAM, etc.
    // uint8_t rom_bank0[0x4000];
    // uint8_t rom_bank_n[0x4000]; // (handled by MBC)
    // uint8_t vram[0x2000];      // 0x8000 - 0x9FFF
    // uint8_t external_ram[0x2000]; // 0xA000 - 0xBFFF (cartridge RAM)
    // uint8_t wram_bank0[0x1000];   // 0xC000 - 0xCFFF
    // uint8_t wram_bank_n[0x1000];  // 0xD000 - 0xDFFF (switchable CGB)
    // uint8_t echo_ram[0x1E00];   // 0xE000 - 0xFDFF (mirror of C000-DDFF)
    // uint8_t oam[0xA0];          // 0xFE00 - 0xFE9F
    // (I/O registers are 0xFF00 - 0xFF7F - handled by dispatch)
    // uint8_t hram[0x7F];         // 0xFF80 - 0xFFF
} MMU;

// initialize and reset the MMU
void mmu_init(MMU *mmu, struct CPU *cpu);
void mmu_reset(MMU *mmu);

// read a 8bit value from the memory bus
// 8-bit value = ram[addr]
uint8_t mmu_read(MMU *mmu, uint16_t addr);

// read a 16bit value from the memory bus
// 16-bit value = ram[addr] + (ram[addr+1] << 8)
uint16_t mmu_read16(MMU *mmu, uint16_t addr);

// write a 8bit value to the memory bus
// ram[addr] =  8-bit value
void mmu_write(MMU *mmu, uint16_t addr, uint8_t value);

// write a 16bit value to the memory bus
// ram[addr] =  16-bit value
void mmu_write16(MMU *mmu, uint16_t addr, uint16_t value);

#endif
