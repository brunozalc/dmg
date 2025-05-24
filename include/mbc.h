#ifndef MBC_HEADER
#define MBC_HEADER

#include <stdbool.h>
#include <stdint.h>

/* MBC types based on cartridge header */
typedef enum {
    MBC_NONE = 0x00,     // ROM ONLY
    MBC1 = 0x01,         // MBC1
    MBC1_RAM = 0x02,     // MBC1+RAM
    MBC1_RAM_BAT = 0x03, // MBC1+RAM+BATTERY
    MBC3 = 0x11,         // MBC3
    MBC3_RAM_BAT = 0x13, // MBC3+RAM+BATTERY
    MBC5 = 0x19,         // MBC5
} mbc_type_t;

/* MBC1 banking modes */
typedef enum {
    MBC1_MODE_16_8 = 0,  // 16Mbit ROM/8KByte RAM mode (default)
    MBC1_MODE_4_32 = 1,  // 4Mbit ROM/32KByte RAM mode
} mbc1_mode_t;

/* Forward declaration */
struct MMU;

/* MBC state structure */
typedef struct MBC {
    mbc_type_t type;
    
    /* ROM banking */
    uint8_t rom_bank_low;    // ROM bank register (5 bits for MBC1)
    uint8_t rom_bank_high;   // Upper ROM bank bits / RAM bank (2 bits for MBC1)
    
    /* RAM banking */
    bool ram_enable;         // RAM enable flag
    uint8_t ram_bank;        // Current RAM bank
    
    /* MBC1 specific */
    mbc1_mode_t banking_mode; // MBC1 banking mode
    
    /* ROM/RAM sizes */
    uint32_t rom_size;       // Total ROM size in bytes
    uint32_t ram_size;       // Total RAM size in bytes
    uint8_t rom_banks;       // Number of ROM banks
    uint8_t ram_banks;       // Number of RAM banks
    
} MBC;

/* MBC functions */
void mbc_init(MBC *mbc, uint8_t cartridge_type, uint8_t rom_size_code, uint8_t ram_size_code);
void mbc_reset(MBC *mbc);

/* MBC read/write handlers */
uint8_t mbc_read_rom(MBC *mbc, struct MMU *mmu, uint16_t addr);
uint8_t mbc_read_ram(MBC *mbc, struct MMU *mmu, uint16_t addr);
void mbc_write_control(MBC *mbc, uint16_t addr, uint8_t value);
void mbc_write_ram(MBC *mbc, struct MMU *mmu, uint16_t addr, uint8_t value);

/* Helper functions */
uint8_t mbc_get_current_rom_bank(MBC *mbc);
uint8_t mbc_get_current_ram_bank(MBC *mbc);

/* Debug functions */
void mbc_print_state(MBC *mbc);

#endif
