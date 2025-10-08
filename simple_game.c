
#include <SDL2/SDL.h>
#include <stdio.h>
#include <emscripten/emscripten.h>

#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 40

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Color currentColor = {255, 0, 0, 255};
SDL_Color colors[] = {
    {255, 0, 0, 255},     // Red
    {0, 255, 0, 255},     // Green  
    {0, 0, 255, 255},     // Blue
    {255, 0, 255, 255},   // Purple
    {255, 165, 0, 255}    // Orange
};
int currentCat = 0;

void main_loop() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            emscripten_cancel_main_loop();
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            
            SDL_Rect leftButton = {100, 350, BUTTON_WIDTH, BUTTON_HEIGHT};
            SDL_Rect rightButton = {500, 350, BUTTON_WIDTH, BUTTON_HEIGHT};
            
            if (mouseX >= leftButton.x && mouseX <= leftButton.x + leftButton.w &&
                mouseY >= leftButton.y && mouseY <= leftButton.y + leftButton.h) {
                currentCat = (currentCat - 1 + 5) % 5;
                currentColor = colors[currentCat];
            }
            if (mouseX >= rightButton.x && mouseX <= rightButton.x + rightButton.w &&
                mouseY >= rightButton.y && mouseY <= rightButton.y + rightButton.h) {
                currentCat = (currentCat + 1) % 5;
                currentColor = colors[currentCat];
            }
        }
    }
    
    SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
    SDL_RenderClear(renderer);
    
    SDL_Rect catRect = {300, 125, 200, 200};
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_RenderFillRect(renderer, &catRect);
    
    SDL_Rect leftButton = {100, 350, BUTTON_WIDTH, BUTTON_HEIGHT};
    SDL_Rect rightButton = {500, 350, BUTTON_WIDTH, BUTTON_HEIGHT};
    
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &leftButton);
    SDL_RenderFillRect(renderer, &rightButton);
    
    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Cat Viewer - Web Version", 
                             SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                             800, 450, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    emscripten_set_main_loop(main_loop, 0, 1);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}