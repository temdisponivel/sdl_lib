#include "engine.h"
#include "graphics.h"
#include "maths.h"
#include "input.h"
#include "time.h"

int main(int handle, char** params) {
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    if (result) {
        const char *error = SDL_GetError();
        SDL_Log(error);
        return -1;
    }

    graphics_data_t graphics_data = {};
    input_data_t input_data = {};
    time_data_t time_data = {};
    
    time_data.time_scale = 1;
    
    SDL_Window* window = SDL_CreateWindow("Test SDL!", 100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        
    texture_t texture;
    load_texture_from_file("data/image.png", renderer, &texture);
    
    texture_t texture_sheet;
    load_texture_from_file("data/sheet.png", renderer, &texture_sheet);
    
    camera_t camera;
    camera.transform.world_pos = get_vec2(0, 0);
    graphics_data.camera = camera;

    for (int i = 0; i < 2; ++i) {
        sprite_renderer_t *tex_renderer = create_sprite_renderer(&graphics_data, &texture);
        tex_renderer->transform.world_pos = get_vec2(0, 0);
        tex_renderer->normalized_pivot = get_normalized_pivot_point(PIVOT_TOP_LEFT);
    }
    
    sprite_animation_t first_animation;
    create_sprite_animation_from_sheet(
            &texture_sheet,
            get_vec2(320, 330),
            12,
            30,
            2,
            true,
            &first_animation
    );

    sprite_animation_t second_animation;
    create_sprite_animation_from_sheet(
            &texture_sheet,
            get_vec2(320, 330),
            0,
            11,
            1,
            false,
            &second_animation
    );
    
    time_data.target_frame_rate = 60;
    
    while (!input_data.quit_event_called) {
        
        start_frame(&time_data);
        
        update_input_data(&input_data);
        
        {
            vec2_t to_add = {};
            int speed = 1;

#define key_function is_key_holded

            //SDL_TriggerBreakpoint();
            //bool right = is_modifier_on(&input_data, MODIFIER_ALT);
            
            if (key_function(&input_data, KEY_a) && is_modifier_on(&input_data, MODIFIER_ALT)) {
                to_add = get_vec2(-speed, 0);
            } else if (key_function(&input_data, KEY_d) && input_data.current_modifiers == MODIFIER_CTRL) {
                to_add = get_vec2(speed, 0);
            } else if (key_function(&input_data, KEY_w) && input_data.current_modifiers == (MODIFIER_ALT & MODIFIER_CTRL)) {
                to_add = get_vec2(0, -speed);
            } else if (key_function(&input_data, KEY_s) && input_data.current_modifiers == (MODIFIER_ALT & MODIFIER_SHIFT)) {
                to_add = get_vec2(0, speed);
            }

            graphics_data.renderers[0].transform.world_pos = sum_vec2(graphics_data.renderers[0].transform.world_pos, to_add);
        }
        
        {
            vec2_t to_add = {};
            int speed = 1;

            speed += get_click_count(&input_data, BUTTON_LEFT);

            #define button_function is_button_released
            if (button_function(&input_data, BUTTON_LEFT)) {
                to_add = get_vec2(-speed, 0);
            } else if (button_function(&input_data, BUTTON_CENTER)) {
                to_add = get_vec2(speed, 0);
            } else if (button_function(&input_data, BUTTON_RIGHT)) {
                to_add = get_vec2(0, speed);
            }

            graphics_data.renderers[1].transform.world_pos = sum_vec2(graphics_data.renderers[1].transform.world_pos, to_add);
        }

        graphics_data.renderers[0].transform.world_pos = input_data.mouse_pos;

        if (input_data.window_resized) {
            SDL_Log("New size: %i x %i", input_data.window_new_size.x, input_data.window_new_size.y); 
        }
        
        //SDL_Log("Mouse pos: %i x %i | Mouse delta: %i x %i | Mouse scroll: %i", input_data.mouse_pos.x, input_data.mouse_pos.y, input_data.mouse_delta.x, input_data.mouse_delta.y, input_data.mouse_scroll_delta);
        
        SDL_RenderClear(renderer);
        
        update_sprite_animation(&time_data, &first_animation);
        set_sprite_on_renderer(&graphics_data.renderers[0], &first_animation);

        update_sprite_animation(&time_data, &second_animation);
        set_sprite_on_renderer(&graphics_data.renderers[1], &second_animation);
        
        draw(renderer, &graphics_data);
        //draw_texture(renderer, &texture, get_vec2(10, 10));

        SDL_RenderPresent(renderer);

        end_frame(&time_data);
        
        //SDL_Log("FPS: %f", 1.f / time_data.dt);
    }
    
    return 0;
}