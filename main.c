#include "engine.h"
#include "graphics.h"
#include "maths.h"
#include "input.h"
#include "time.h"
#include "physics.h"
#include "SDL2/SDL_mixer.h"
#include "audio.h"
#include "SDL2/SDL_ttf.h"
#include "gui.h"
#include "stdio.h"

int main(int handle, char **params) {
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    if (result) {
        const char *error = SDL_GetError();
        SDL_Log(error);
        return -1;
    }

    result = TTF_Init();
    if (result) {
        const char *error = TTF_GetError();
        SDL_Log(error);
        return -2;
    }

    SDL_Window *window = SDL_CreateWindow("Test SDL!", 100, 100, 800, 600,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    graphics_data_t graphics_data = {};
    input_data_t input_data = {};
    time_data_t time_data = {};
    physics_data_t physics_data = {};
    audio_data_t audio_data = {};
    font_t font = {};

    bool inited = init_audio(&audio_data);
    SDL_assert(inited);

    time_data.time_scale = 1;
    time_data.target_frame_rate = 60;

    init_font_from_file(renderer, &font, DEFAULT_FONT_PATH, DEFAULT_FONT_SIZE, ITALIC);

    texture_t texture;
    load_texture_from_file("data/image.png", renderer, &texture);

    texture_t texture_sheet;
    load_texture_from_file("data/sheet.png", renderer, &texture_sheet);
    
    texture_t button_normal_texture;
    texture_t button_normal2_texture;
    texture_t button_clicked_texture;
    load_texture_from_file("data/button_normal.png", renderer, &button_normal_texture);
    load_texture_from_file("data/button_normal.png", renderer, &button_normal2_texture);
    load_texture_from_file("data/button_clicked.png", renderer, &button_clicked_texture);
    
    sprite_t button_normal_sprite = create_sprite(&button_normal_texture);
    sprite_t button_normal2_sprite = create_sprite(&button_normal2_texture);
    sprite_t button_clicked_sprite = create_sprite(&button_clicked_texture);
    
    camera_t camera;
    camera.transform.position = get_vec2(0, 0);
    graphics_data.camera = camera;

#define ENTITY_COUNT 2

    collider_t *colliders[ENTITY_COUNT];
    
    sprite_animation_t first_animation;
    sprite_animation_t second_animation;
    
    for (int i = 0; i < ENTITY_COUNT; ++i) {
        sprite_renderer_t *tex_renderer = get_sprite_renderer(&graphics_data, &texture);
        tex_renderer->transform.position = get_vec2(0, 0);
        tex_renderer->transform.scale = get_vec2(.5f, .5f);
        tex_renderer->normalized_pivot = get_normalized_pivot_point(PIVOT_CENTER);
        
        if (i == 0)
            colliders[i] = get_circle_collider(&physics_data, i, 100);
        else
            colliders[i] = get_box_collider(&physics_data, i, tex_renderer->sprite.texture_region.size);
    }

    collider_t *first_collider = colliders[0];
    collider_t *second_collider = colliders[1];
    
    sprite_renderer_t *first_renderer = &graphics_data.renderers[0];
    sprite_renderer_t *second_renderer = &graphics_data.renderers[1];

    sound_t effect = load_sound_from_file("data/effect.wav", EFFECT);
    sound_t music = load_sound_from_file("data/music.mp3", MUSIC);

    audio_source_t *source = get_audio_source(&audio_data, effect);
    audio_source_t *music_source = get_audio_source(&audio_data, music);

    music_source->volume = 0;
    fade_in_audio_source(music_source, 20);

    play_audio_source(music_source);

    create_sprite_animation_from_sheet(
            &texture_sheet,
            get_vec2(320, 330),
            12,
            30,
            2,
            true,
            &first_animation
    );

    create_sprite_animation_from_sheet(
            &texture_sheet,
            get_vec2(320, 330),
            0,
            11,
            1,
            false,
            &second_animation
    );
    
    label_t mouse_pos_label;
    setup_label(&mouse_pos_label, &font, "", COLOR_BLUE);
    mouse_pos_label.pivot = PIVOT_BOTTOM_RIGHT;
    
    label_t message_label;
    setup_label(&message_label, &font, "Hello, is anybody there?", COLOR_RED);
    
    while (!input_data.quit_event_called) {

        start_frame(&time_data);

        update_input_data(&input_data);
        update_physics_data(&physics_data);
        update_audio_data(&audio_data, &time_data);

        vec2_t to_add = {};
        int speed = 100;

        if (is_key_pressed(&input_data, KEY_a)) {
            to_add = get_vec2(-speed, 0);
        } else if (is_key_pressed(&input_data, KEY_d)) {
            to_add = get_vec2(speed, 0);
        } else if (is_key_pressed(&input_data, KEY_w)) {
            to_add = get_vec2(0, -speed);
        } else if (is_key_pressed(&input_data, KEY_s)) {
            to_add = get_vec2(0, speed);
        }

        source->position = sum_vec2(source->position, to_add);
        second_renderer->transform.position = sum_vec2(second_renderer->transform.position, to_add);
        
        if (is_key_pressed(&input_data, KEY_p)) {
            first_animation.playing = !first_animation.playing;
            
            if (music_source->playing)
                pause_audio_source(music_source);
            else
                resume_audio_source(music_source);
        }

        if (is_key_pressed(&input_data, KEY_r)) {
            reset_audio_source(music_source);
            reset_animation(&first_animation);
        }

        first_renderer->transform.position = input_data.mouse_pos;
        audio_data.listener.position = input_data.mouse_pos;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        update_sprite_animation(&time_data, &first_animation);
        update_sprite_animation(&time_data, &second_animation);
        
        set_sprite_on_renderer(first_renderer, &first_animation);
        set_sprite_on_renderer(second_renderer, &second_animation);

        draw_physics_debug(renderer, &physics_data);

        for (int i = 0; i < ENTITY_COUNT; ++i) {
            sprite_renderer_t tex_render = graphics_data.renderers[i];
            update_collider_pos_based_on_renderer(&tex_render, colliders[i]);

            if (colliders[i]->collision_count > 0) {
                message_label.color = COLOR_GREEN;
                
                if (!source->playing) {
                    play_audio_source(source);
                } 
            } else {
                message_label.color = COLOR_BLUE;
            }
        }

        draw(renderer, &graphics_data);
        
        char mouse_pos_text[128];
        sprintf(mouse_pos_text, "x: %f - y: %f", input_data.mouse_pos.x, input_data.mouse_pos.y);
        set_label_text(&mouse_pos_label, mouse_pos_text);
        
        draw_label(renderer, get_vec2(800, 600), &mouse_pos_label);
        draw_label(renderer, second_renderer->transform.position, &message_label);
        
        bool clicked = draw_click_area_sprites_ex(
                renderer,
                &input_data,
                get_vec2(400, 300),
                get_vec2(200, 100),
                PIVOT_CENTER,
                &button_normal_sprite,
                &button_clicked_sprite,
                &button_clicked_sprite,
                &font,
                "Click me!",
                24,
                COLOR_GREEN,
                COLOR_BLACK,
                PIVOT_TOP
        );

        if (clicked) {
            SDL_Log("Sprite!!");
            play_audio_source(source);
        }

        clicked = draw_click_area_color_ex(
                renderer,
                &input_data,
                get_vec2(400, 400),
                get_vec2(200, 100),
                PIVOT_CENTER,
                COLOR_RED,
                COLOR_BLUE,
                COLOR_GREEN,
                &font,
                "Click me!",
                24,
                COLOR_WHITE,
                COLOR_BLACK,
                PIVOT_CENTER_LEFT
        );

        if (clicked) {
            SDL_Log("Color!!");
        }
        
        clicked = draw_click_area_colored_sprites_ex(
                renderer,
                &input_data,
                get_vec2(400, 500),
                get_vec2(200, 100),
                PIVOT_CENTER,
                &button_normal2_sprite,
                COLOR_RED,
                COLOR_BLUE,
                COLOR_GREEN,
                &font,
                "Click me!",
                24,
                COLOR_WHITE,
                COLOR_BLACK,
                PIVOT_CENTER_BOTTOM
        );

        if (clicked) {
            SDL_Log("Sprite Color!!");
        }
                        
        SDL_RenderPresent(renderer);

        end_frame(&time_data);
    }

    return 0;
}