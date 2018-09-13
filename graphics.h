//
// Created by matheus on 12/09/2018.
//

#ifndef SDL_GAME_GRAPHICS_H
#define SDL_GAME_GRAPHICS_H

#include "maths.h"

typedef struct texture {
    SDL_Texture *handle;
    ivec2_t size;
} texture_t;

void load_texture_from_file(const char* file_name, SDL_Renderer *renderer, texture_t *texture);
void destroy_texture(texture_t *texture);

void draw_texture(SDL_Renderer *renderer, const texture_t *texture, ivec2_t position);

#endif //SDL_GAME_GRAPHICS_H
