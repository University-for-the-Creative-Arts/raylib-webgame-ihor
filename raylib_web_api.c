#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <emscripten/emscripten.h>

#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 40
#define BUTTON_SPACING 40

Texture2D catTexture = { 0 };

// JavaScript function to fetch cat image
EM_JS(void, js_fetch_cat_image, (), {
    console.log("Fetching random cat from API...");
    fetch('https://cataas.com/cat?json=true')
        .then(response => response.json())
        .then(data => {
            const catUrl = 'https://cataas.com' + data.url;
            console.log("Cat URL:", catUrl);
            
            // Create image and load it
            const img = new Image();
            img.crossOrigin = 'anonymous';
            img.onload = function() {
                console.log("Image loaded, creating texture...");
                // Create canvas to get image data
                const canvas = document.createElement('canvas');
                const ctx = canvas.getContext('2d');
                canvas.width = img.width;
                canvas.height = img.height;
                ctx.drawImage(img, 0, 0);
                
                // Get image data
                const imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
                
                // Create texture in raylib
                Module._create_texture(canvas.width, canvas.height, imageData.data);
            };
            img.onerror = function() {
                console.error('Failed to load cat image');
            };
            img.src = catUrl;
        })
        .catch(error => {
            console.error('Error fetching cat:', error);
        });
});

// Function called from JavaScript to create texture
EMSCRIPTEN_KEEPALIVE
void create_texture(int width, int height, unsigned char* data) {
    // Unload previous texture
    if (catTexture.id != 0) {
        UnloadTexture(catTexture);
    }
    
    // Create image from data
    Image image = {
        .data = data,
        .width = width,
        .height = height,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1
    };
    
    // Create texture
    catTexture = LoadTextureFromImage(image);
    TraceLog(LOG_INFO, "Texture created: %dx%d", width, height);
}

void loadCatImage() {
    TraceLog(LOG_INFO, "Loading cat image from API...");
    js_fetch_cat_image();
}

void main_loop() {
    // Get mouse position and check for clicks
    Vector2 mousePoint = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    // Calculate button positions
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int centerX = screenWidth / 2;
    const int centerY = screenHeight / 2;
    
    Rectangle catButtonBounds = {
        (float)centerX - BUTTON_WIDTH / 2,
        (float)screenHeight - BUTTON_HEIGHT - 20,
        BUTTON_WIDTH, 
        BUTTON_HEIGHT
    };
    
    // Check button interactions
    if (mouseClicked && CheckCollisionPointRec(mousePoint, catButtonBounds)) {
        TraceLog(LOG_INFO, "NEW CAT Button Pressed!");
        loadCatImage();
    }

    // Drawing
    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw title
        DrawText("Raylib Cat API Viewer - Web", 200, 20, 24, DARKBLUE);
        DrawText("Click 'NEW CAT' to fetch random cats from the internet!", 120, 60, 18, DARKGRAY);
        
        // Draw cat texture or placeholder
        if (catTexture.id != 0) {
            DrawTexture(catTexture, 
                        centerX - catTexture.width / 2, 
                        centerY - catTexture.height / 2, 
                        WHITE);
        } else {
            // Draw placeholder
            DrawRectangle(centerX - 100, centerY - 100, 200, 200, LIGHTGRAY);
            DrawText("No cat loaded", centerX - 60, centerY - 10, 20, DARKGRAY);
            DrawText("Click the button below!", centerX - 100, centerY + 20, 18, GRAY);
        }
        
        // Draw button
        Color buttonColor = CheckCollisionPointRec(mousePoint, catButtonBounds) ? LIGHTGRAY : DARKGRAY;
        DrawRectangleRec(catButtonBounds, buttonColor);
        DrawText("NEW CAT", 
                 (int)catButtonBounds.x + 20, 
                 (int)catButtonBounds.y + 10, 
                 20, 
                 WHITE);
        
    EndDrawing();
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    // Initialization
    InitWindow(screenWidth, screenHeight, "Raylib Cat API Viewer - Web");
    SetTargetFPS(60);
    
    TraceLog(LOG_INFO, "Raylib Web Cat Viewer Started");
    
    // Use Emscripten's main loop for web
    emscripten_set_main_loop(main_loop, 0, 1);
    
    // Cleanup (typically not reached in web)
    UnloadTexture(catTexture);
    CloseWindow();
    
    return 0;
}