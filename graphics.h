//
// Created by matheus on 12/09/2018.
//

#ifndef SDL_GAME_GRAPHICS_H
#define SDL_GAME_GRAPHICS_H

#include "maths.h"
#include "time.h"

#define MAX_RENDERERS 64
#define MAX_SPRITES_ON_ANIMATOR 32

typedef struct texture {
    SDL_Texture *handle;
    vec2_t size;
} texture_t;

typedef struct sprite {
    texture_t *texture;
    rect_t texture_region;
} sprite_t;

typedef struct sprite_animation {
    sprite_t sprites[MAX_SPRITES_ON_ANIMATOR];
    int frame_count;
    
    float cycle_duration;
    float current_time;
    
    bool loop;
    bool playing;
} sprite_animation_t;

typedef struct sprite_renderer {
    sprite_t sprite;
    
    transform_t transform;
    vec2_t normalized_pivot;

} sprite_renderer_t;

typedef struct camera {
    transform_t transform;
} camera_t;

typedef struct color {
    byte red;
    byte green;
    byte blue;
    byte alpha;
} color_t;

#define COLOR_CHANNEL_FULL 255
#define COLOR_RED get_color(COLOR_CHANNEL_FULL, 0, 0, COLOR_CHANNEL_FULL)
#define COLOR_GREEN get_color(0, COLOR_CHANNEL_FULL, 0, COLOR_CHANNEL_FULL)
#define COLOR_BLUE get_color(0, 0, COLOR_CHANNEL_FULL, 0)
#define COLOR_BLACK get_color(0, 0, 0, COLOR_CHANNEL_FULL)
#define COLOR_WHITE get_color(COLOR_CHANNEL_FULL, COLOR_CHANNEL_FULL, COLOR_CHANNEL_FULL, COLOR_CHANNEL_FULL)
#define COLOR_TRANSPARENT get_color(0, 0, 0, 0)

// TODO: Add an texture array that will hold all loaded textures
// create a function to get texture by name and return from the array or load if necessary
typedef struct graphics_data {    
    camera_t camera;
    sprite_renderer_t renderers[MAX_RENDERERS];
    uint renderers_count;
} graphics_data_t;

void load_texture_from_file(const char *file_name, SDL_Renderer *renderer, texture_t *texture);

void destroy_texture(texture_t *texture);

void draw_texture_ex(
        SDL_Renderer *renderer,
        rect_t screen_region,
        float angle,
        rect_t texture_region,
        vec2_t pivot,
        const texture_t *texture
);

void draw_sprite_renderer(SDL_Renderer *renderer, const camera_t *camera, const sprite_renderer_t *tex_renderer);

sprite_renderer_t *get_sprite_renderer(graphics_data_t *graphics_data, texture_t *texture);

void free_sprite_renderer(graphics_data_t *graphics_data, sprite_renderer_t *renderer);

void draw(SDL_Renderer *renderer, graphics_data_t *graphics_data);

sprite_t create_sprite(texture_t *texture, rect_t region);

// SPRITE ANIMATION

void create_sprite_animation_from_sheet(
        texture_t *sprite_sheet,
        vec2_t sprite_size,
        int start_frame,
        int frame_count,
        int cycle_duration,
        bool loop,
        sprite_animation_t *destination
);

int get_sprite_animation_frame_index(const sprite_animation_t *animation);

void stop_animation(sprite_animation_t *animation);

void reset_animation(sprite_animation_t *animation);

void update_sprite_animation(const time_data_t *time_data, sprite_animation_t *animation);

void set_sprite_on_renderer(sprite_renderer_t *renderer, const sprite_animation_t *animation);

color_t get_color(byte red, byte green, byte blue, byte alpha);

#endif //SDL_GAME_GRAPHICS_H
