#include "raylib.h"

int main() {
    const int screenWidth = 960;
    const int screenHeight = 600;
    Texture2D testSprite = LoadTexture("sprite.png");

    InitWindow(screenWidth, screenHeight, "Raylib Starter Template");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            //Draw sprite at the position 
            DrawTexture(testSprite, screenWidth / 2, screenHeight / 2, WHITE);
        EndDrawing();
        
    }
    UnloadTexture(testSprite);
    CloseWindow();
    return 0;
}