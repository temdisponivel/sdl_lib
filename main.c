#include "engine.h"
#include "graphics.h"
#include "maths.h"
#include "input.h"
#include "time.h"
#include "physics.h"
#include "SDL2/SDL_mixer.h"
#include "audio.h"

rect_t get_sprite_screen_region(sprite_renderer_t *tex_renderer) {
    vec2_t world_pos = tex_renderer->transform.world_pos;

    vec2_t screen_size = mul_vec2(tex_renderer->sprite.texture_region.size, tex_renderer->transform.scale);

    vec2_t pivot = denormalize_rect_point(get_rect(VEC2_ZERO, screen_size), tex_renderer->normalized_pivot);
    world_pos = sub_vec2(world_pos, pivot);

    rect_t screen_region = get_rect(world_pos, screen_size);
    return screen_region;
}

circle_t get_sprite_screen_circle(sprite_renderer_t *tex_renderer) {
    rect_t screen_region = get_sprite_screen_region(tex_renderer);
    circle_t circle;
    circle.position = screen_region.position;
    
    vec2_t squared = mul_vec2(screen_region.size, screen_region.size);
    circle.radius = screen_region.size.x;// SDL_sqrtf(squared.x + squared.y);
    return circle;
}

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
    physics_data_t physics_data = {};
    audio_data_t audio_data =  {};
    
    bool inited = init_audio(&audio_data);
    SDL_assert(inited);
    
    sound_t effect = load_sound_from_file("data/effect.wav", EFFECT);
    sound_t music = load_sound_from_file("data/music.mp3", MUSIC);
    
    audio_source_t *source = get_audio_source(&audio_data, effect);
    audio_source_t *music_source = get_audio_source(&audio_data, music);
    
    music_source->volume = 0;
    fade_in_audio_source(music_source, 20);
    
    //source->loop = true;
    //source->loop = false;
    //source->volume = 0;
    //fade_in_audio_source(source, 25);
    //source->volume = .5;
    
    play_audio_source(music_source);
    
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

#define ENTITY_COUNT 2
    
    collider_t *colliders[ENTITY_COUNT];

    for (int i = 0; i < ENTITY_COUNT; ++i) {
        sprite_renderer_t *tex_renderer = get_sprite_renderer(&graphics_data, &texture);
        tex_renderer->transform.world_pos = get_vec2(0, 0);
        tex_renderer->transform.scale = get_vec2(.5f, .5f);
        tex_renderer->normalized_pivot = get_normalized_pivot_point(PIVOT_CENTER);
        
        //colliders[i] = get_box_collider(&physics_data, i, get_vec2(320, 165));
        colliders[i] = get_circle_collider(&physics_data, i, 320);
    }
    
    //graphics_data.renderers[1].transform.world_pos = get_vec2(400, 300);
    
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
            int speed = 100;

#define key_function is_key_holded

            //SDL_TriggerBreakpoint();
            //bool right = is_modifier_on(&input_data, MODIFIER_ALT);
            
            if (key_function(&input_data, KEY_a)) {
                to_add = get_vec2(-speed, 0);
            } else if (key_function(&input_data, KEY_d)) {
                to_add = get_vec2(speed, 0);
            } else if (key_function(&input_data, KEY_w)) {
                to_add = get_vec2(0, -speed);
            } else if (key_function(&input_data, KEY_s)) {
                to_add = get_vec2(0, speed);
            }

            source->position = sum_vec2(source->position, to_add);
            graphics_data.renderers[1].transform.world_pos = sum_vec2(graphics_data.renderers[1].transform.world_pos, to_add);
        }
        
        {
            vec2_t to_add = {};
            int speed = 1;

            speed += get_click_count(&input_data, BUTTON_LEFT);

            #define button_function is_button_holded
            if (button_function(&input_data, BUTTON_LEFT)) {
                graphics_data.renderers[0].transform.angle++;
                to_add = get_vec2(-speed, 0);
            } else if (button_function(&input_data, BUTTON_CENTER)) {
                to_add = get_vec2(speed, 0);
            } else if (button_function(&input_data, BUTTON_RIGHT)) {
                to_add = get_vec2(0, speed);
            }

            graphics_data.renderers[1].transform.world_pos = sum_vec2(graphics_data.renderers[1].transform.world_pos, to_add);
        }
        
        if (is_key_pressed(&input_data, KEY_p)) {
            if (music_source->playing)  
                pause_audio_source(music_source);
            else
                resume_audio_source(music_source);
        }
        
        if (is_key_pressed(&input_data, KEY_r)) {
            reset_audio_source(music_source);
        }
        
        if (is_key_pressed(&input_data, KEY_r)) {
            reset_animation(&first_animation);
        }

        if (is_key_pressed(&input_data, KEY_p)) {
            first_animation.playing = !first_animation.playing;
        }
        
        if (is_key_pressed(&input_data, KEY_BACKSPACE)) {
            free_sprite_renderer(&graphics_data, graphics_data.renderers);
        }

        graphics_data.renderers[0].transform.world_pos = input_data.mouse_pos;
        audio_data.listener.position = input_data.mouse_pos;

        if (input_data.window_resized) {
            SDL_Log("New size: %i x %i", input_data.window_new_size.x, input_data.window_new_size.y); 
        }
        
        //SDL_Log("Mouse pos: %i x %i | Mouse delta: %i x %i | Mouse scroll: %i", input_data.mouse_pos.x, input_data.mouse_pos.y, input_data.mouse_delta.x, input_data.mouse_delta.y, input_data.mouse_scroll_delta);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        update_sprite_animation(&time_data, &first_animation);
        set_sprite_on_renderer(&graphics_data.renderers[0], &first_animation);

        update_sprite_animation(&time_data, &second_animation);
        set_sprite_on_renderer(&graphics_data.renderers[1], &second_animation);

        for (int i = 0; i < ENTITY_COUNT; ++i) {
            sprite_renderer_t tex_render = graphics_data.renderers[i]; 
            rect_t tex_rect = calculate_rect(
                    tex_render.transform.world_pos, 
                    tex_render.sprite.texture_region.size, 
                    tex_render.transform.scale, 
                    tex_render.normalized_pivot
            );
            
            colliders[i]->position = tex_rect.position;
            //colliders[i]->box_size = tex_rect.size;
            colliders[i]->circle_radius = tex_rect.size.y;
            
            if (colliders[i]->collision_count > 0)  {
                if (!source->playing)
                    play_audio_source(source);
            }
        }
        
        update_physics_data(&physics_data);
        
        draw(renderer, &graphics_data);
        //draw_texture(renderer, &texture, get_vec2(10, 10));
        draw_physics_debug(renderer, &physics_data);
        update_audio_data(&audio_data, &time_data);
        
        SDL_RenderPresent(renderer);

        end_frame(&time_data);
        
        //SDL_Log("FPS: %f", 1.f / time_data.dt);
    }
    
    return 0;
}