#include "engine.h"
#include "graphics.h"
#include "maths.h"
#include "input.h"

int main(int handle, char** params) {
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    if (result) {
        const char *error = SDL_GetError();
        SDL_Log(error);
        return -1;
    }

    graphics_data_t graphics_data = {};
    input_data_t input_data;
    memset(input_data.current_frame_keyboard, 0, sizeof(input_data.current_frame_keyboard));
    
    SDL_Window* window = SDL_CreateWindow("Test SDL!", 100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        
    texture_t texture;
    load_texture_from_file("data/image.png", renderer, &texture);
    texture.size = scale_vec2(texture.size, .3f);
    
    camera_t camera;
    camera.world_position = get_vec2(50, 300);
    graphics_data.camera = camera;

    for (int i = 0; i < 10; ++i) {
        texture_renderer_t *tex_renderer = create_texture_renderer(&graphics_data, &texture);
        tex_renderer->world_position = get_vec2(i * texture.size.x, 0);
    }
    
    while (!input_data.quit_event_called) {
        update_input_data(&input_data);

        for (int i = 0; i < KEY_LAST; ++i) {
            STATE state = input_data.current_frame_keyboard[i];
            if (state != STATE_NORMAL) {
                SDL_Log("%i: %i", i, state);
            }
        }
        
        if (input_data.window_resized) {
            SDL_Log("New size: %i x %i", input_data.window_new_size.x, input_data.window_new_size.y); 
        }
        
        //SDL_Log("Mouse pos: %i x %i | Mouse delta: %i x %i | Mouse scroll: %i", input_data.mouse_pos.x, input_data.mouse_pos.y, input_data.mouse_delta.x, input_data.mouse_delta.y, input_data.mouse_scroll_delta);
        
        SDL_RenderClear(renderer);
        
        draw(renderer, &graphics_data);
        //draw_texture(renderer, &texture, get_vec2(10, 10));

        SDL_RenderPresent(renderer);
    }
    
    return 0;
}