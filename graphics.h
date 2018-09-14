//
// Created by matheus on 12/09/2018.
//

#ifndef SDL_GAME_GRAPHICS_H
#define SDL_GAME_GRAPHICS_H

#include "maths.h"

#define MAX_RENDERERS 64


typedef enum {
    PIVOT_CENTER = 1 << 0,
    PIVOT_BOTTOM = 1 << 1,
    PIVOT_TOP = 1 << 2,
    PIVOT_LEFT = 1 << 3,
    PIVOT_RIGHT = 1 << 4,
    
    PIVOT_CENTER_LEFT = PIVOT_CENTER | PIVOT_LEFT,
    PIVOT_CENTER_RIGHT = PIVOT_CENTER | PIVOT_RIGHT,
    PIVOT_CENTER_BOTTOM = PIVOT_CENTER | PIVOT_BOTTOM,
    PIVOT_CENTER_TOP = PIVOT_CENTER | PIVOT_TOP,
    
    PIVOT_BOTTOM_LEFT = PIVOT_BOTTOM | PIVOT_LEFT,
    PIVOT_BOTTOM_RIGHT = PIVOT_BOTTOM | PIVOT_RIGHT,
    
    PIVOT_TOP_LEFT = PIVOT_TOP | PIVOT_LEFT,
    PIVOT_TOP_RIGHT = PIVOT_TOP | PIVOT_RIGHT,
    
} TEXTURE_PIVOT;

typedef struct texture {
    SDL_Texture *handle;
    vec2_t size;
} texture_t;

// TODO: Add pivot
typedef struct texture_renderer {
    texture_t *texture;
    vec2_t world_position;
    vec2_t pivot;
    
    // TODO: Use this texture region on drawing
    vec2_t texture_region;
} texture_renderer_t;

// TODO: Create an animator system

typedef struct camera {
    vec2_t world_position;
} camera_t;

typedef struct graphics_data {
    camera_t camera;
    texture_renderer_t renderers[MAX_RENDERERS];
    uint active_renderers;
} graphics_data_t; 

void load_texture_from_file(const char* file_name, SDL_Renderer *renderer, texture_t *texture);
void destroy_texture(texture_t *texture);

void draw_texture(SDL_Renderer *renderer, const texture_t *texture, vec2_t world_position);

vec2_t get_pivot_point(vec2_t region, TEXTURE_PIVOT pivot);

texture_renderer_t *create_texture_renderer(graphics_data_t *graphics_data, texture_t *texture);

void draw(SDL_Renderer *renderer, graphics_data_t *graphics_data);

#endif //SDL_GAME_GRAPHICS_H
