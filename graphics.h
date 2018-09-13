//
// Created by matheus on 12/09/2018.
//

#ifndef SDL_GAME_GRAPHICS_H
#define SDL_GAME_GRAPHICS_H

#include "maths.h"

#define MAX_RENDERERS 64

typedef struct texture {
    SDL_Texture *handle;
    ivec2_t size;
} texture_t;

typedef struct texture_renderer {
    texture_t *texture;
    ivec2_t world_position;
    ivec2_t texture_region;
} texture_renderer_t;

typedef struct camera {
    ivec2_t world_position;
} camera_t;

typedef struct graphics_data {
    camera_t camera;
    texture_renderer_t renderers[MAX_RENDERERS];
    uint active_renderers;
} graphics_data_t; 

void load_texture_from_file(const char* file_name, SDL_Renderer *renderer, texture_t *texture);
void destroy_texture(texture_t *texture);

void draw_texture(SDL_Renderer *renderer, const texture_t *texture, ivec2_t position);

texture_renderer_t *create_texture_renderer(graphics_data_t *graphics_data, texture_t *texture);

void draw(SDL_Renderer *renderer, graphics_data_t *graphics_data);

#endif //SDL_GAME_GRAPHICS_H
