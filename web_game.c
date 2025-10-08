#include "raylib.h"
#include <emscripten/emscripten.h>
#include <stdlib.h>
#include <string.h>

#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 40
#define BUTTON_SPACING 40

Texture2D catTexture = { 0 };
int currentCat = 0;
Color catColors[] = { RED, GREEN, BLUE, PURPLE, ORANGE };

void generateCatTexture() {
    if (catTexture.id != 0) {
        UnloadTexture(catTexture);
    }
    
    Image catImage = GenImageColor(200, 200, catColors[currentCat]);
    const char* catNames[] = { "RED CAT", "GREEN CAT", "BLUE CAT", "PURPLE CAT", "ORANGE CAT" };
    ImageDrawText(&catImage, catNames[currentCat], 30, 90, 20, WHITE);
    
    catTexture = LoadTextureFromImage(catImage);
    UnloadImage(catImage);
    
    currentCat = (currentCat + 1) % 5;
}

void main_loop() {
    Vector2 mousePoint = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    Rectangle leftButton = { 100, 350, BUTTON_WIDTH, BUTTON_HEIGHT };
    Rectangle rightButton = { 500, 350, BUTTON_WIDTH, BUTTON_HEIGHT };
    
    if (mouseClicked) {
        if (CheckCollisionPointRec(mousePoint, leftButton)) {
            currentCat = (currentCat - 1 + 5) % 5;
            generateCatTexture();
        }
        if (CheckCollisionPointRec(mousePoint, rightButton)) {
            generateCatTexture();
        }
    }
    
    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        if (catTexture.id != 0) {
            DrawTexture(catTexture, 300, 125, WHITE);
        }
        
        Color leftColor = CheckCollisionPointRec(mousePoint, leftButton) ? LIGHTGRAY : DARKGRAY;
        Color rightColor = CheckCollisionPointRec(mousePoint, rightButton) ? LIGHTGRAY : DARKGRAY;
        
        DrawRectangleRec(leftButton, leftColor);
        DrawRectangleRec(rightButton, rightColor);
        
        DrawText("PREV", leftButton.x + 35, leftButton.y + 10, 20, WHITE);
        DrawText("NEXT CAT", rightButton.x + 20, rightButton.y + 10, 20, WHITE);
        
        DrawText("Cat Viewer - Web Version", 250, 20, 24, DARKBLUE);
        DrawText("Click buttons to change cat colors!", 220, 60, 20, DARKGRAY);
        
    EndDrawing();
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "Raylib Cat Viewer - Web");
    generateCatTexture();
    SetTargetFPS(60);
    emscripten_set_main_loop(main_loop, 0, 1);
    
    UnloadTexture(catTexture);
    CloseWindow();
    return 0;
}