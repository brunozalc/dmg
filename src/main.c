#include <raylib.h>
#include <stdio.h>

#include "bus.h"
#include "cpu.h"
#include "rom.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <rom_file>\n", argv[0]);
        return 1;
    }

    const char* rom_file    = argv[1];

    const int DISPLAY_SCALE = 4;
    const int HEIGHT_PX     = 144;
    const int WIDTH_PX      = 160;

    // initialize the CPU and reset all states
    CPU cpu;
    cpu_reset(&cpu);

    // initialize the memory bus and reset all states
    mem_reset();

    load_rom(rom_file);

    // raylib init
    SetTraceLogLevel(5);
    InitWindow(WIDTH_PX * DISPLAY_SCALE, HEIGHT_PX * DISPLAY_SCALE,
               "sm83 emulator");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        for (int i = 0; i < 10000; ++i) {
            cpu_step(&cpu);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Hello, Game Boy!", 10, 10, 20, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
