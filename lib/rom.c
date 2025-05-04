#include "rom.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bus.h"

static const char *cartridge_type_str(uint8_t t) {
    switch (t) {
        case 0x00:
            return "ROM ONLY";
        case 0x01:
            return "MBC1";
        case 0x02:
            return "MBC1+RAM";
        case 0x03:
            return "MBC1+RAM+BATTERY";
        case 0x08:
            return "ROM+RAM";
        case 0x09:
            return "ROM+RAM+BATTERY";
        case 0x0F:
            return "MBC3+TIMER+BATTERY";
        case 0x11:
            return "MBC3";
        case 0x13:
            return "MBC3+RAM+BATTERY";
        case 0x19:
            return "MBC5";
        default:
            return "UNKNOWN";
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
void log_header(void) {
    const uint8_t *header = ram + 0x0100;  // header starts at 0x0100
    char title[17]        = {0};

    // header + 0x34 -> title
    memcpy(title, header + 0x34, 16);

    uint8_t cart_type = header[0x47];
    uint8_t rom_size  = header[0x48];
    uint8_t ram_size  = header[0x49];

    printf("\n=== header ===\n");
    printf("title       : %.16s\n", title);
    printf("cartridge   : 0x%02X (%s)\n", cart_type,
           cartridge_type_str(cart_type));
    printf("ROM size    : %s\n", rom_size_str(rom_size));
    printf("RAM size    : %s\n", ram_size_str(ram_size));
    printf("region      : %s\n", header[0x4A] ? "West" : "Japan");
    printf("CGB flag    : 0x%02X\n", header[0x43]);
    printf("SGB flag    : 0x%02X\n", header[0x46]);
    printf("================\n\n");
}

void load_rom(const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    assert(file && "ROM not found?");

    // without banking yet. games go up to ram[0x8000]
    size_t read = fread(ram, 1, 0x8000, file);
    fclose(file);
    assert(read);  // making sure we read something

    printf("rom loaded: %zu bytes from %s\n", read, filepath);
    log_header();
}
