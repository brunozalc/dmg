#include <raylib.h>

#include "bus.h"
#include "cpu.h"

int main(void) {
    const int DISPLAY_SCALE = 4;
    const int HEIGHT_PX = 160;
    const int WIDTH_PX = 144;

    InitWindow(WIDTH_PX * DISPLAY_SCALE, HEIGHT_PX * DISPLAY_SCALE,
               "sm83 emulator");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Hello, Game Boy!", 10, 10, 20, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
