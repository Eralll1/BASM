#include "../outer/raylib.h"

int main() {
    // --- 1. Initialization ---
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "My First Raylib Game");
    SetTargetFPS(60); // Set a comfortable frame rate

    // --- 2. Main Game Loop ---
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        // --- Update Logic (Input, Physics, AI, etc.) ---
        // ...

        // --- Drawing ---
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawCircle(100, 100, 40, GOLD);
            // All drawing functions go here (e.g., DrawText, DrawRectangle)
        EndDrawing();
    }

    // --- 3. De-Initialization ---
    CloseWindow(); // Close window and unload OpenGL context

    return 0;
}