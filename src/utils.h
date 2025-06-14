#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <raylib.h>
#include <stdint.h>

#define BOOT_ROM_PATH "./include/boot/bootix_dmg.bin"
#define ALT_BOOT_ROM_PATH "./include/boot/dmg_boot.bin"

// window information
const int DISPLAY_SCALE = 4;
const int HEIGHT_PX = 144;
const int WIDTH_PX = 160;

const uint32_t FRAMES_PER_RTC_TICK = 60;

Color dmg_palette[4] = {
    RAYWHITE,
    LIGHTGRAY,
    DARKGRAY,
    BLACK,
};

#endif
