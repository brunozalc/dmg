#ifndef MMU_HEADER
#define MMU_HEADER

#include <stdbool.h>
#include <stdint.h>

#include "mbc.h"

/* I/O registers addresses */
#define DIV 0xFF04   // DIV register
#define TIMA 0xFF05  // TIMA register
#define TMA 0xFF06   // TMA register
#define TAC 0xFF07   // TAC register
#define IF 0xFF0F    // IF register
#define LCDC 0xFF40  // LCD control
#define STAT 0xFF41  // LCD status
#define SCY 0xFF42   // scroll Y
#define SCX 0xFF43   // scroll X
#define LY 0xFF44    // current scanline
#define LYC 0xFF45   // LY compare
#define BGP 0xFF47   // background palette
#define IE 0xFFFF    // IE register

struct CPU;
struct Timer;
struct PPU;

typedef struct MMU {
    struct CPU *cpu;      // pointer to the CPU
    struct Timer *timer;  // pointer to the timer
    struct PPU *ppu;      // pointer to the PPU

    /* cartridge data */
    uint8_t *cartridge_rom;     // dynamically allocated ROM data
    uint32_t cartridge_rom_size; // actual ROM size in bytes
    uint8_t *cartridge_ram;     // dynamically allocated external RAM
    uint32_t cartridge_ram_size; // actual RAM size in bytes
    MBC mbc;                    // memory bank controller
    
    /* legacy ROM for compatibility (now points to cartridge_rom) */
    uint8_t rom[0x8000];  // 0000h - 7FFFh (first 32KB for backwards compatibility)
    uint8_t rom_bank;     // current rom bank (for MBC1 and MBC2)
    bool ram_enable;      // ram enabled (for MBC1 and MBC2)

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

} MMU;

// initialize and reset the MMU
void mmu_init(MMU *mmu, struct CPU *cpu, struct Timer *timer, struct PPU *ppu);
void mmu_reset(MMU *mmu);

// free the memory allocated for the banking controller
void mmu_cleanup(MMU *mmu);

// read a 8bit value from the memory bus
uint8_t mmu_read(MMU *mmu, uint16_t addr);

// read a 16bit value from the memory bus
uint16_t mmu_read16(MMU *mmu, uint16_t addr);

// write a 8bit value to the memory bus
void mmu_write(MMU *mmu, uint16_t addr, uint8_t value);

// write a 16bit value to the memory bus
void mmu_write16(MMU *mmu, uint16_t addr, uint16_t value);

#endif
