#include "mbc.h"

#include <stdio.h>
#include <string.h>

#include "mmu.h"

/* helper function to get ROM size in bytes from header code */
static uint32_t get_rom_size_bytes(uint8_t rom_size_code) {
    if (rom_size_code <= 8) {
        return 0x8000 << rom_size_code;  // 32KB * 2^code
    }
    return 0x8000;  // default to 32KB for unknown codes
}

/* helper function to get RAM size in bytes from header code */
static uint32_t get_ram_size_bytes(uint8_t ram_size_code) {
    switch (ram_size_code) {
        case 0:  return 0;       // no RAM
        case 1:  return 0x800;   // 2KB
        case 2:  return 0x2000;  // 8KB
        case 3:  return 0x8000;  // 32KB
        default: return 0;
    }
}

/* helper function to convert MBC type byte to enum */
static mbc_type_t cartridge_type_to_mbc_type(uint8_t cart_type) {
    switch (cart_type) {
        case 0x00: return MBC_NONE;
        case 0x01: return MBC1;
        case 0x02: return MBC1_RAM;
        case 0x03: return MBC1_RAM_BAT;
        case 0x11: return MBC3;
        case 0x13: return MBC3_RAM_BAT;
        case 0x19: return MBC5;
        default:   return MBC_NONE;
    }
}

void mbc_init(MBC *mbc, uint8_t cartridge_type, uint8_t rom_size_code, uint8_t ram_size_code) {
    memset(mbc, 0, sizeof(MBC));

    mbc->type      = cartridge_type_to_mbc_type(cartridge_type);
    mbc->rom_size  = get_rom_size_bytes(rom_size_code);
    mbc->ram_size  = get_ram_size_bytes(ram_size_code);
    mbc->rom_banks = mbc->rom_size / 0x4000;                              // 16KB per ROM bank
    mbc->ram_banks = (mbc->ram_size > 0) ? (mbc->ram_size / 0x2000) : 0;  // 8KB per RAM bank

    mbc_reset(mbc);

    printf("MBC initialized: type=%d, ROM=%dKB (%d banks), RAM=%dKB (%d banks)\n", mbc->type,
           mbc->rom_size / 1024, mbc->rom_banks, mbc->ram_size / 1024, mbc->ram_banks);
}

void mbc_reset(MBC *mbc) {
    mbc->rom_bank_low  = 1;  // ROM bank 1 is selected by default (bank 0 maps to bank 1)
    mbc->rom_bank_high = 0;
    mbc->ram_enable    = false;
    mbc->ram_bank      = 0;
    mbc->banking_mode  = MBC1_MODE_16_8;  // default mode
}

uint8_t mbc_get_current_rom_bank(MBC *mbc) {
    switch (mbc->type) {
        case MBC1:
        case MBC1_RAM:
        case MBC1_RAM_BAT: {
            uint8_t bank = mbc->rom_bank_low & 0x1F;  // 5 bits
            if (bank == 0)
                bank = 1;  // bank 0 maps to bank 1

            // in mode 1, upper bits affect bank selection for 0x4000-0x7FFF
            if (mbc->banking_mode == MBC1_MODE_4_32) {
                bank |= (mbc->rom_bank_high & 0x03) << 5;  // add upper 2 bits
            }

            return bank % mbc->rom_banks;  // ensure we don't exceed available banks
        }
        case MBC_NONE:
        default:       return 1;  // no banking, always bank 1 for 0x4000-0x7FFF
    }
}

uint8_t mbc_get_current_ram_bank(MBC *mbc) {
    switch (mbc->type) {
        case MBC1_RAM:
        case MBC1_RAM_BAT:
            if (mbc->banking_mode == MBC1_MODE_4_32 && mbc->ram_banks > 1) {
                return (mbc->rom_bank_high & 0x03) % mbc->ram_banks;
            }
            return 0;  // in mode 0, always RAM bank 0
        case MBC_NONE:
        case MBC1:
        default:       return 0;
    }
}

uint8_t mbc_read_rom(MBC *mbc, struct MMU *mmu, uint16_t addr) {
    uint32_t physical_addr;

    if (addr < 0x4000) {
        // Bank 0 area (0x0000-0x3FFF)
        if (mbc->type == MBC1 || mbc->type == MBC1_RAM || mbc->type == MBC1_RAM_BAT) {
            if (mbc->banking_mode == MBC1_MODE_4_32) {
                // In mode 1, this area can be banked with upper bits
                uint8_t bank  = (mbc->rom_bank_high & 0x03) << 5;
                physical_addr = (bank * 0x4000) + addr;
            } else {
                // Mode 0: always bank 0
                physical_addr = addr;
            }
        } else {
            // No MBC: direct access
            physical_addr = addr;
        }
    } else {
        // Switchable bank area (0x4000-0x7FFF)
        uint8_t bank  = mbc_get_current_rom_bank(mbc);
        physical_addr = (bank * 0x4000) + (addr - 0x4000);
    }

    // Bounds check
    if (physical_addr >= mmu->cartridge_rom_size) {
        return 0xFF;  // Return 0xFF for out-of-bounds reads
    }

    return mmu->cartridge_rom[physical_addr];
}

uint8_t mbc_read_ram(MBC *mbc, struct MMU *mmu, uint16_t addr) {
    if (!mbc->ram_enable || mbc->ram_size == 0) {
        return 0xFF;  // RAM disabled or not present
    }

    uint8_t bank           = mbc_get_current_ram_bank(mbc);
    uint32_t physical_addr = (bank * 0x2000) + (addr - 0xA000);

    if (physical_addr >= mmu->cartridge_ram_size) {
        return 0xFF;  // Out of bounds
    }

    return mmu->cartridge_ram[physical_addr];
}

void mbc_write_control(MBC *mbc, uint16_t addr, uint8_t value) {
    switch (mbc->type) {
        case MBC1:
        case MBC1_RAM:
        case MBC1_RAM_BAT:
            if (addr < 0x2000) {
                // RAM Enable (0x0000-0x1FFF)
                mbc->ram_enable = (value & 0x0F) == 0x0A;
            } else if (addr < 0x4000) {
                // ROM Bank Number (0x2000-0x3FFF)
                mbc->rom_bank_low = value & 0x1F;  // 5 bits
                if (mbc->rom_bank_low == 0) {
                    mbc->rom_bank_low = 1;  // Bank 0 maps to bank 1
                }
            } else if (addr < 0x6000) {
                // RAM Bank Number / Upper ROM Bank bits (0x4000-0x5FFF)
                mbc->rom_bank_high = value & 0x03;  // 2 bits
            } else if (addr < 0x8000) {
                // Banking Mode Select (0x6000-0x7FFF)
                mbc->banking_mode = (value & 0x01) ? MBC1_MODE_4_32 : MBC1_MODE_16_8;
            }
            break;
        case MBC_NONE:
        default:
            // No banking, ignore writes
            break;
    }
}

void mbc_write_ram(MBC *mbc, struct MMU *mmu, uint16_t addr, uint8_t value) {
    if (!mbc->ram_enable || mbc->ram_size == 0) {
        return;  // RAM disabled or not present
    }

    uint8_t bank           = mbc_get_current_ram_bank(mbc);
    uint32_t physical_addr = (bank * 0x2000) + (addr - 0xA000);

    if (physical_addr >= mmu->cartridge_ram_size) {
        return;  // Out of bounds
    }

    mmu->cartridge_ram[physical_addr] = value;
}

void mbc_print_state(MBC *mbc) {
    printf("\n=== MBC State ===\n");
    printf("Type: %d\n", mbc->type);
    printf("ROM Bank Low: 0x%02X (%d)\n", mbc->rom_bank_low, mbc->rom_bank_low);
    printf("ROM Bank High: 0x%02X (%d)\n", mbc->rom_bank_high, mbc->rom_bank_high);
    printf("Current ROM Bank: %d\n", mbc_get_current_rom_bank(mbc));
    printf("RAM Enabled: %s\n", mbc->ram_enable ? "Yes" : "No");
    printf("Current RAM Bank: %d\n", mbc_get_current_ram_bank(mbc));
    printf("Banking Mode: %s\n",
           (mbc->banking_mode == MBC1_MODE_16_8) ? "16Mbit ROM/8KB RAM" : "4Mbit ROM/32KB RAM");
    printf("ROM: %dKB (%d banks)\n", mbc->rom_size / 1024, mbc->rom_banks);
    printf("RAM: %dKB (%d banks)\n", mbc->ram_size / 1024, mbc->ram_banks);
    printf("================\n\n");
}
