#include <emscripten/emscripten.h>
#include <stdio.h>

#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 450

// JavaScript integration
EM_JS(void, js_setup_canvas, (), {
    // Setup canvas and 2D context
    Module.canvas = document.getElementById('canvas');
    Module.ctx = Module.canvas.getContext('2d');
});

EM_JS(void, js_clear_screen, (int color), {
    // Clear screen with color
    const r = (color >> 16) & 0xFF;
    const g = (color >> 8) & 0xFF;
    const b = color & 0xFF;
    Module.ctx.fillStyle = `rgb(${r}, ${g}, ${b})`;
    Module.ctx.fillRect(0, 0, Module.canvas.width, Module.canvas.height);
});

EM_JS(void, js_draw_rectangle, (int x, int y, int width, int height, int color), {
    // Draw rectangle
    const r = (color >> 16) & 0xFF;
    const g = (color >> 8) & 0xFF;
    const b = color & 0xFF;
    Module.ctx.fillStyle = `rgb(${r}, ${g}, ${b})`;
    Module.ctx.fillRect(x, y, width, height);
});

EM_JS(void, js_draw_text, (const char* text, int x, int y, int size, int color), {
    // Draw text
    const textStr = UTF8ToString(text);
    const r = (color >> 16) & 0xFF;
    const g = (color >> 8) & 0xFF;
    const b = color & 0xFF;
    Module.ctx.fillStyle = `rgb(${r}, ${g}, ${b})`;
    Module.ctx.font = `${size}px Arial`;
    Module.ctx.fillText(textStr, x, y);
});

EM_JS(void, js_fetch_cat_image, (), {
    // Fetch random cat image
    console.log("Fetching cat from API...");
    fetch('https://cataas.com/cat?width=300&height=300&json=true')
        .then(response => response.json())
        .then(data => {
            const catUrl = 'https://cataas.com' + data.url;
            console.log("Loaded cat:", catUrl);
            
            const img = new Image();
            img.crossOrigin = 'anonymous';
            img.onload = function() {
                // Draw the cat image
                Module.ctx.drawImage(img, 250, 100, 300, 300);
                Module.ctx.strokeStyle = '#333';
                Module.ctx.lineWidth = 2;
                Module.ctx.strokeRect(250, 100, 300, 300);
                
                // Draw success message
                Module.ctx.fillStyle = '#006600';
                Module.ctx.font = '18px Arial';
                Module.ctx.fillText('Real cat loaded from API!', 300, 430);
            };
            img.onerror = function() {
                console.error('Failed to load cat image');
                Module.ctx.fillStyle = '#cc0000';
                Module.ctx.font = '18px Arial';
                Module.ctx.fillText('Failed to load cat image', 300, 430);
            };
            img.src = catUrl;
        })
        .catch(error => {
            console.error('API error:', error);
            Module.ctx.fillStyle = '#cc0000';
            Module.ctx.font = '18px Arial';
            Module.ctx.fillText('Network error loading cat', 300, 430);
        });
});

// Colors (RGB format)
#define COLOR_RAYWHITE  0xf5f5f5
#define COLOR_DARKGRAY  0x505050
#define COLOR_LIGHTGRAY 0xd0d0d0
#define COLOR_DARKBLUE  0x0066cc
#define COLOR_WHITE     0xffffff
#define COLOR_BLACK     0x000000
#define COLOR_GREEN     0x00a800
#define COLOR_RED       0xcc0000

int main() {
    // Setup canvas
    js_setup_canvas();
    
    // Draw initial screen
    js_clear_screen(COLOR_RAYWHITE);
    js_draw_text("Raylib-Style Cat API Viewer", 200, 30, 24, COLOR_DARKBLUE);
    js_draw_text("Click the button to load random cats from the internet!", 150, 60, 16, COLOR_BLACK);
    
    // Draw placeholder
    js_draw_rectangle(250, 100, 300, 300, COLOR_LIGHTGRAY);
    js_draw_text("No cat loaded yet", 320, 250, 20, COLOR_DARKGRAY);
    js_draw_text("Click the button below!", 300, 280, 16, COLOR_BLACK);
    
    // Draw button
    js_draw_rectangle(340, 420, 120, 40, COLOR_DARKGRAY);
    js_draw_text("NEW CAT", 360, 445, 20, COLOR_WHITE);
    
    // Draw button border
    js_draw_rectangle(338, 418, 124, 44, COLOR_BLACK);
    
    printf("Cat API Viewer started!\n");
    
    return 0;
}

// Function to be called from JavaScript when button is clicked
EMSCRIPTEN_KEEPALIVE
void load_new_cat() {
    printf("Loading new cat...\n");
    js_fetch_cat_image();
}