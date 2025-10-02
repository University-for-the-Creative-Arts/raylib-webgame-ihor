#include "raylib.h"
#include <stdlib.h> // Required for NULL definition and other standard C utilities

// Define button dimensions
#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 40
#define BUTTON_SPACING 40

// --- API Configuration ---
// In a simple C setup, we define constants here.
// NOTE: For security, never share compiled code containing real keys.
#define CAT_API_KEY "YOUR_SECRET_API_KEY_HERE"

// --- Global Variables for Texture and URL ---
Texture2D catTexture = { 0 }; // The texture to draw (GPU data)
char catImageUrl[256] = { 0 }; // Placeholder for the fetched URL string

// --- Function to Load the Cat Image from a URL ---
void loadCatImage() {
    // ----------------------------------------------------------------------
    // NOTE ON API INTEGRATION:
    // To fully integrate The Cat API, you would need:
    // 1. A way to make an HTTP GET request (e.g., using a library like curl) 
    //    and set the header: "x-api-key: " + CAT_API_KEY.
    // 2. A JSON parser to extract the image URL from the API response
    //    (e.g., from "https://api.thecatapi.com/v1/images/search").
    //
    // WARNING: raylib's standard LoadImage() function is used here. 
    // True network loading often requires platform-specific configuration or 
    // the use of an external HTTP library since LoadImage() is primarily for files.
    // ----------------------------------------------------------------------
    
    // --- STATIC PLACEHOLDER URL ---
    // Using a placeholder URL to simulate the successful fetching of an image link.
    TextCopy(catImageUrl, "https://placehold.co/200x200/556B2F/ffffff?text=MEOW"); 

    // 1. Unload the previous texture if it exists
    if (catTexture.id != 0) {
        UnloadTexture(catTexture);
    }

    // 2. Load the image from the URL/path into CPU memory
    Image catImage = LoadImage(catImageUrl); // CORRECTED: Changed LoadImageFromURL to LoadImage
    
    // 3. Check if loading succeeded (Image data pointer should not be NULL)
    if (catImage.data != NULL) {
        // 4. Convert the Image to a Texture and upload to GPU
        catTexture = LoadTextureFromImage(catImage);
        
        // 5. Free CPU memory used by the Image
        UnloadImage(catImage); 
        TraceLog(LOG_INFO, "New cat texture loaded successfully.");
    } else {
        // Fallback for failed network load
        catTexture.id = 0; 
        TraceLog(LOG_WARNING, "Failed to load cat image from URL. Network access required.");
    }
}

int main() {
    const int screenWidth = 960;
    const int screenHeight = 600;
    
    // --- Initialization ---
    InitWindow(screenWidth, screenHeight, "Raylib Cat Viewer");
    
    // INITIAL LOAD: Load the first cat image (or placeholder)
    loadCatImage();
    
    // --- Define Positions ---
    
    // We base the button positions on a fixed center size (if texture fails, it's 1x1, so we use a fixed visual anchor)
    const int centerVisualSize = 200; 
    const int centerX = screenWidth / 2;
    const int centerY = screenHeight / 2;
    
    // 1. Button 1 (Left) Position and Rectangle
    Rectangle leftButtonBounds = {
        (float)centerX - centerVisualSize / 2 - BUTTON_WIDTH - BUTTON_SPACING, // Left of the visual center
        (float)centerY - BUTTON_HEIGHT / 2,  // Y: Vertically centered
        BUTTON_WIDTH, 
        BUTTON_HEIGHT
    };

    // 2. Button 2 (Right / "NEW CAT") Position and Rectangle
    Rectangle rightButtonBounds = {
        (float)centerX + centerVisualSize / 2 + BUTTON_SPACING, // Right of the visual center
        (float)centerY - BUTTON_HEIGHT / 2,      // Y: Vertically centered
        BUTTON_WIDTH, 
        BUTTON_HEIGHT
    };
    
    SetTargetFPS(60);

    // --- Main Game Loop ---
    while (!WindowShouldClose()) {
        
        // Get mouse position and check for a left-click
        Vector2 mousePoint = GetMousePosition();
        bool mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        
        // --- Input and Logic ---
        
        // Check for LEFT Button interaction
        if (CheckCollisionPointRec(mousePoint, leftButtonBounds)) {
            if (mouseClicked) {
                TraceLog(LOG_INFO, "LEFT Button Pressed!");
                // You could use this button to change filter, zoom, etc.
            }
        }

        // Check for RIGHT Button interaction (Load NEW CAT)
        if (CheckCollisionPointRec(mousePoint, rightButtonBounds)) {
            if (mouseClicked) {
                TraceLog(LOG_INFO, "NEW CAT Button Pressed! Attempting to load...");
                loadCatImage();
            }
        }

        // --- Drawing ---
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            // 1. Draw the Cat Texture in the center
            if (catTexture.id != 0) {
                DrawTexture(catTexture, 
                            centerX - catTexture.width / 2, 
                            centerY - catTexture.height / 2, 
                            WHITE);
            } else {
                // Draw error text if texture failed to load
                DrawText("LOADING ERROR", centerX - 70, centerY - 10, 20, RED);
            }
            
            // 2. Draw the Buttons
            
            // LEFT Button
            Color leftButtonColor = CheckCollisionPointRec(mousePoint, leftButtonBounds) ? LIGHTGRAY : DARKGRAY;
            DrawRectangleRec(leftButtonBounds, leftButtonColor);
            DrawText("FILTER", 
                     (int)leftButtonBounds.x + 35, 
                     (int)leftButtonBounds.y + 10, 
                     20, 
                     WHITE);
            
            // RIGHT Button ("NEW CAT")
            Color rightButtonColor = CheckCollisionPointRec(mousePoint, rightButtonBounds) ? LIGHTGRAY : DARKGRAY;
            DrawRectangleRec(rightButtonBounds, rightButtonColor);
            DrawText("NEW CAT", 
                     (int)rightButtonBounds.x + 20, 
                     (int)rightButtonBounds.y + 10, 
                     20, 
                     WHITE);

            // Text at top (modified)
            DrawText("The Cat API Viewer (Network Required)", 
                     screenWidth/2 - MeasureText("The Cat API Viewer (Network Required)", 20)/2, 
                     10, 20, 
                     DARKBLUE);
            
        EndDrawing();
        
    }
    
    // --- De-Initialization ---
    UnloadTexture(catTexture); // Ensure the texture memory is freed
    CloseWindow();
    return 0;
}
