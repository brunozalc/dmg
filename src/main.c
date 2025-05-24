#include <raylib.h>
#include <stdio.h>

#include "cpu.h"
#include "mmu.h"
#include "ppu.h"
#include "rom.h"
#include "timer.h"

// window information
const int DISPLAY_SCALE = 4;
const int HEIGHT_PX     = 144;
const int WIDTH_PX      = 160;

Color dmg_palette[4]    = {
    RAYWHITE,
    LIGHTGRAY,
    DARKGRAY,
    BLACK,
};

// declare the components
CPU cpu;
MMU mmu;
PPU ppu;
Timer timer;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <rom_file>\n", argv[0]);
        return 1;
    }

    const char* rom_file = argv[1];

    // initialize and reset components
    mmu_init(&mmu, &cpu, &timer, &ppu);
    ppu_init(&ppu, &mmu, &cpu);
    timer_init(&timer, &cpu, &mmu);
    cpu_init(&cpu, &mmu, &timer, &ppu);

    load_rom(&mmu, rom_file);

    // raylib init
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WIDTH_PX * DISPLAY_SCALE, HEIGHT_PX * DISPLAY_SCALE, "dmg emulator");
    SetTargetFPS(60);

    Image image       = GenImageColor(WIDTH_PX, HEIGHT_PX, BLANK);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    Color display[WIDTH_PX * HEIGHT_PX];

    while (!WindowShouldClose()) {
        // run the CPU until a frame has been completed
        ppu.frame_completed = 0;

        while (!ppu.frame_completed) {
            cpu_step(&cpu);  // run the CPU. this also ticks all other components
        }

        BeginDrawing();
        ClearBackground(BLACK);

        const uint8_t (*ppu_framebuffer)[LCD_WIDTH] = ppu_get_framebuffer(&ppu);

        for (int y = 0; y < HEIGHT_PX; y++) {
            for (int x = 0; x < WIDTH_PX; x++) {
                display[y * WIDTH_PX + x] = dmg_palette[ppu_framebuffer[y][x] & 0x03];
            }
        }

        UpdateTexture(texture, display);

        DrawTextureEx(texture, (Vector2){0, 0}, 0.0f, DISPLAY_SCALE, WHITE);

        DrawFPS(10, 10);

        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();
    return 0;
}
