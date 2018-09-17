//
// Created by matheus on 12/09/2018.
//

#ifndef SDL_GAME_GRAPHICS_H
#define SDL_GAME_GRAPHICS_H

#include "maths.h"
#include "time.h"

#define MAX_RENDERERS 1024
#define MAX_SPRITES_ON_ANIMATOR 32
#define MAX_LAYERS 16

#define FIRST_LAYER 0;
#define LAST_LAYER MAX_LAYERS - 1

typedef ubyte layer_index_t;

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
    
    // TODO: Should this really be a pointer?!
    transform_t *transform;
    vec2_t normalized_pivot;
    
    int depth_inside_layer;

    layer_index_t layer;

} sprite_renderer_t;

typedef struct camera {
    transform_t transform;
    
    // TODO: Maybe allow this to be changeable?
    vec2_t _half_size;
} camera_t;

typedef struct color {
    ubyte red;
    ubyte green;
    ubyte blue;
    ubyte alpha;
} color_t;

typedef struct drawing_layer {
    sprite_renderer_t *renderers[MAX_RENDERERS];
    int renderer_count;
} drawing_layer_t;

// TODO: Add an texture array that will hold all loaded textures
// create a function to get texture by name and return from the array or load if necessary
typedef struct graphics_data {    
    camera_t camera;
    sprite_renderer_t renderers[MAX_RENDERERS];
    uint renderers_count;
    
    drawing_layer_t layers[MAX_LAYERS];
    
    texture_t white_texture;    
} graphics_data_t;

#define COLOR_CHANNEL_FULL 255
#define COLOR_RED get_color(COLOR_CHANNEL_FULL, 0, 0, COLOR_CHANNEL_FULL)
#define COLOR_GREEN get_color(0, COLOR_CHANNEL_FULL, 0, COLOR_CHANNEL_FULL)
#define COLOR_BLUE get_color(0, 0, COLOR_CHANNEL_FULL, 0)
#define COLOR_BLACK get_color(0, 0, 0, COLOR_CHANNEL_FULL)
#define COLOR_WHITE get_color(COLOR_CHANNEL_FULL, COLOR_CHANNEL_FULL, COLOR_CHANNEL_FULL, COLOR_CHANNEL_FULL)
#define COLOR_GREY get_color(128, 128, 128, COLOR_CHANNEL_FULL)
#define COLOR_TRANSPARENT get_color(0, 0, 0, 0)
#define COLOR_TO_PARAMETERS(color) (color).red, (color).green, (color).blue, (color).alpha

SDL_Surface *create_solid_color_surface(vec2_t size, color_t color);
void create_solid_color_texture(SDL_Renderer *renderer, vec2_t size, color_t color, texture_t *destination);

void init_graphics_data(SDL_Renderer *renderer, graphics_data_t *graphics_data);

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

sprite_renderer_t *get_sprite_renderer_empty(graphics_data_t *graphics_data);

sprite_renderer_t *get_sprite_renderer(graphics_data_t *graphics_data, texture_t *texture);

void free_sprite_renderer(graphics_data_t *graphics_data, sprite_renderer_t *renderer);

rect_t calculate_rect_based_on_pivot_and_scale(vec2_t position, vec2_t size, vec2_t scale, vec2_t normalized_pivot);

vec2_t world_to_camera_pos(vec2_t world_pos, const camera_t *camera);
void update_graphics_data(vec2_t current_resolution, graphics_data_t *graphics_data);
void draw(SDL_Renderer *renderer, graphics_data_t *graphics_data);

sprite_t create_sprite(texture_t *texture);
sprite_t create_sprite_ex(texture_t *texture, rect_t region);

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

SDL_Color convert_color(color_t color);

void debug_draw_circle(SDL_Renderer *renderer, vec2_t center, float radius, color_t color);

void debug_draw_rect(SDL_Renderer *renderer, rect_t rect, color_t color);

void debug_draw_fill_rect(SDL_Renderer *renderer, rect_t rect, color_t color);

#endif //SDL_GAME_GRAPHICS_H
