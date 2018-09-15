//
// Created by matheus on 12/09/2018.
//

#ifndef SDL_GAME_GRAPHICS_H
#define SDL_GAME_GRAPHICS_H

#include "maths.h"

#define MAX_RENDERERS 64


typedef struct texture {
    SDL_Texture *handle;
    vec2_t size;
} texture_t;

// TODO: Add normalized_pivot
typedef struct texture_renderer {
    texture_t *texture;
    transform_t transform;
    vec2_t normalized_pivot;

    rect_t texture_region;
} texture_renderer_t;

// TODO: Create an animator system

typedef struct camera {
    transform_t transform;
} camera_t;

typedef struct graphics_data {
    camera_t camera;
    texture_renderer_t renderers[MAX_RENDERERS];
    uint active_renderers;
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

void draw_texture_renderer(SDL_Renderer *renderer, const camera_t *camera, const texture_renderer_t *tex_renderer);

texture_renderer_t *create_texture_renderer(graphics_data_t *graphics_data, texture_t *texture);

void draw(SDL_Renderer *renderer, graphics_data_t *graphics_data);

#endif //SDL_GAME_GRAPHICS_H
