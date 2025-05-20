#include <raylib.h>
#include <stdio.h>

#include "cpu.h"
#include "mmu.h"
#include "rom.h"
#include "timer.h"

// window information
const int DISPLAY_SCALE = 4;
const int HEIGHT_PX     = 144;
const int WIDTH_PX      = 160;

// declare the components
CPU cpu;
MMU mmu;
Timer timer;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <rom_file>\n", argv[0]);
        return 1;
    }

    cpu_log = fopen("cpu.log", "w");
    if (!cpu_log) {
        perror("cpu.log");
        exit(1);
    }

    const char* rom_file = argv[1];

    // initialize and reset components
    cpu_init(&cpu, &mmu, &timer);
    mmu_init(&mmu, &cpu, &timer);
    timer_init(&timer, &cpu, &mmu);

    load_rom(&mmu, rom_file);

    // raylib init
    SetTraceLogLevel(5);
    InitWindow(WIDTH_PX * DISPLAY_SCALE, HEIGHT_PX * DISPLAY_SCALE, "dmg emulator");
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
    fclose(cpu_log);
    return 0;
}
