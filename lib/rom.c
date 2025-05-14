#include "rom.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mmu.h"

static const char *cartridge_type_str(uint8_t t) {
    switch (t) {
        case 0x00: return "ROM ONLY";
        case 0x01: return "MBC1";
        case 0x02: return "MBC1+RAM";
        case 0x03: return "MBC1+RAM+BATTERY";
        case 0x08: return "ROM+RAM";
        case 0x09: return "ROM+RAM+BATTERY";
        case 0x0F: return "MBC3+TIMER+BATTERY";
        case 0x11: return "MBC3";
        case 0x13: return "MBC3+RAM+BATTERY";
        case 0x19: return "MBC5";
        default:   return "UNKNOWN";
    }
}

static const char *rom_size_str(uint8_t c) {
    static const char *sizes[] = {"32KB", "64KB", "128KB", "256KB", "512KB",
                                  "1MB",  "2MB",  "4MB",   "8MB"};
    return (c <= 8) ? sizes[c] : "UNKNOWN";
}

static const char *ram_size_str(uint8_t c) {
    static const char *sizes[] = {"0KB", "2KB", "8KB", "32KB"};
    return (c <= 3) ? sizes[c] : "UNKNOWN";
}

/* cartridge headers for the DMG start at the address 0x0100 and end at 0x014F
- 0x0134 -> 0x013E: title
- 0x0144 -> 0x0145: new licensee code
- 0x0147: cart type
- 0x0148: cart rom size
- 0x0149: cart ram size
 */
void log_header(MMU *mmu) {
    char title[17] = {0};

    for (int i = 0; i < 16; i++) {
        title[i] = mmu_read(mmu, 0x0134 + i);
    }

    uint8_t cart_type = mmu_read(mmu, 0x0147);
    uint8_t rom_size  = mmu_read(mmu, 0x0148);
    uint8_t ram_size  = mmu_read(mmu, 0x0149);

    printf("\n=== header ===\n");
    printf("title       : %.16s\n", title);
    printf("cartridge   : 0x%02X (%s)\n", cart_type, cartridge_type_str(cart_type));
    printf("ROM size    : %s\n", rom_size_str(rom_size));
    printf("RAM size    : %s\n", ram_size_str(ram_size));
    printf("region      : %s\n", mmu_read(mmu, 0x0149) ? "West" : "Japan");
    printf("CGB flag    : 0x%02X\n", mmu_read(mmu, 0x0143));
    printf("SGB flag    : 0x%02X\n", mmu_read(mmu, 0x0146));
    printf("================\n\n");
}

void load_rom(MMU *mmu, const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    assert(file && "ROM not found?");

    // without banking yet. games go up to ram[0x8000]
    size_t read = fread(mmu->ram, 1, 0x8000, file);
    fclose(file);
    assert(read);  // making sure we read something

    printf("rom loaded: %zu bytes from %s\n", read, filepath);
    log_header(mmu);
}
