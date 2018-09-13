#include "engine.h"
#include "graphics.h"
#include "maths.h"

int main(int handle, char** params) {
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    if (result) {
        const char *error = SDL_GetError();
        SDL_Log(error);
        return -1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Test SDL!", 100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        
    texture_t texture;
    load_texture_from_file("data/image.png", renderer, &texture);
    
    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        
        SDL_RenderClear(renderer);
            
        draw_texture(renderer, &texture, get_vec2(10, 10));

        SDL_RenderPresent(renderer);
    }
    
    return 0;
}