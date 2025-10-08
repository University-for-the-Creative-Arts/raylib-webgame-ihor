#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <emscripten/emscripten.h>

// Define button dimensions
#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 40
#define BUTTON_SPACING 40

// Global Variables
Texture2D catTexture = { 0 };
char catImageUrl[512] = { 0 };

// JavaScript integration functions
EM_JS(void, js_fetch_cat_image, (), {
    // JavaScript code to fetch cat image from API
    fetch('https://cataas.com/cat?json=true')
        .then(response => response.json())
        .then(data => {
            const catUrl = 'https://cataas.com' + data.url;
            // Pass the URL back to C
            Module._js_receive_cat_url(catUrl);
        })
        .catch(error => {
            console.error('Error fetching cat:', error);
            Module._js_receive_cat_url('error');
        });
});

EM_JS(void, js_load_image_from_url, (const char* url), {
    // Load image from URL and create texture
    const imageUrl = UTF8ToString(url);
    const img = new Image();
    img.crossOrigin = 'anonymous';
    img.onload = function() {
        // Create texture from loaded image
        const canvas = document.createElement('canvas');
        const ctx = canvas.getContext('2d');
        canvas.width = img.width;
        canvas.height = img.height;
        ctx.drawImage(img, 0, 0);
        
        // Get image data and pass to C
        const imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
        Module._js_receive_cat_image(canvas.width, canvas.height, imageData.data);
    };
    img.onerror = function() {
        Module._js_receive_cat_image(0, 0, null);
    };
    img.src = imageUrl;
});

// Callback functions called from JavaScript
EMSCRIPTEN_KEEPALIVE
void js_receive_cat_url(const char* url) {
    if (strcmp(url, "error") == 0) {
        TraceLog(LOG_WARNING, "Failed to fetch cat URL");
        return;
    }
    
    // Store the URL and load the image
    TextCopy(catImageUrl, url);
    TraceLog(LOG_INFO, "Received cat URL: %s", url);
    js_load_image_from_url(url);
}

EMSCRIPTEN_KEEPALIVE  
void js_receive_cat_image(int width, int height, unsigned char* imageData) {
    if (width == 0 || height == 0 || imageData == NULL) {
        TraceLog(LOG_ERROR, "Failed to load cat image");
        return;
    }
    
    // Unload previous texture
    if (catTexture.id != 0) {
        UnloadTexture(catTexture);
    }
    
    // Create image from raw data
    Image catImage = {
        .data = imageData,
        .width = width,
        .height = height,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1
    };
    
    // Convert to texture
    catTexture = LoadTextureFromImage(catImage);
    TraceLog(LOG_INFO, "Cat texture loaded: %dx%d", width, height);
    
    // Note: We don't free imageData here as it's managed by JavaScript
}

// Function to load cat image
void loadCatImage() {
    TraceLog(LOG_INFO, "Fetching new cat image...");
    
    // Unload previous texture
    if (catTexture.id != 0) {
        UnloadTexture(catTexture);
        catTexture.id = 0;
    }
    
    // Fetch new cat image via JavaScript
    js_fetch_cat_image();
}

// Main game loop function
void main_loop() {
    // Get mouse position and check for clicks
    Vector2 mousePoint = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    // Calculate button positions
    const int screenWidth = 960;
    const int screenHeight = 600;
    const int centerVisualSize = 200; 
    const int centerX = screenWidth / 2;
    const int centerY = screenHeight / 2;
    
    Rectangle leftButtonBounds = {
        (float)centerX - centerVisualSize / 2 - BUTTON_WIDTH - BUTTON_SPACING,
        (float)centerY - BUTTON_HEIGHT / 2,
        BUTTON_WIDTH, 
        BUTTON_HEIGHT
    };

    Rectangle rightButtonBounds = {
        (float)centerX + centerVisualSize / 2 + BUTTON_SPACING,
        (float)centerY - BUTTON_HEIGHT / 2,
        BUTTON_WIDTH, 
        BUTTON_HEIGHT
    };
    
    // Check button interactions
    if (mouseClicked) {
        if (CheckCollisionPointRec(mousePoint, leftButtonBounds)) {
            TraceLog(LOG_INFO, "FILTER Button Pressed!");
            // Filter functionality can be added here
        }
        
        if (CheckCollisionPointRec(mousePoint, rightButtonBounds)) {
            TraceLog(LOG_INFO, "NEW CAT Button Pressed!");
            loadCatImage();
        }
    }

    // Drawing
    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw cat texture
        if (catTexture.id != 0) {
            DrawTexture(catTexture, 
                        centerX - catTexture.width / 2, 
                        centerY - catTexture.height / 2, 
                        WHITE);
        } else {
            // Loading message
            DrawText("Click NEW CAT to load a cat!", centerX - 140, centerY - 10, 20, DARKGRAY);
        }
        
        // Draw buttons
        Color leftButtonColor = CheckCollisionPointRec(mousePoint, leftButtonBounds) ? LIGHTGRAY : DARKGRAY;
        DrawRectangleRec(leftButtonBounds, leftButtonColor);
        DrawText("FILTER", 
                 (int)leftButtonBounds.x + 35, 
                 (int)leftButtonBounds.y + 10, 
                 20, 
                 WHITE);
        
        Color rightButtonColor = CheckCollisionPointRec(mousePoint, rightButtonBounds) ? LIGHTGRAY : DARKGRAY;
        DrawRectangleRec(rightButtonBounds, rightButtonColor);
        DrawText("NEW CAT", 
                 (int)rightButtonBounds.x + 20, 
                 (int)rightButtonBounds.y + 10, 
                 20, 
                 WHITE);

        // Title
        DrawText("The Cat API Viewer - Web Version", 
                 screenWidth/2 - MeasureText("The Cat API Viewer - Web Version", 20)/2, 
                 10, 20, DARKBLUE);
        
        // Status
        if (catTexture.id != 0) {
            DrawText("Real cat loaded from API!", 10, screenHeight - 30, 20, DARKGREEN);
        }
        
    EndDrawing();
}

int main() {
    const int screenWidth = 960;
    const int screenHeight = 600;
    
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