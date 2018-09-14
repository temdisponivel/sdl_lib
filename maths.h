//
// Created by matheus on 12/09/2018.
//

#ifndef SDL_GAME_MATHS_H
#define SDL_GAME_MATHS_H

#include "defines.h"

typedef struct vec2 {
    union {
        float x;
        float width;
    };

    union {
        float y;
        float height;
    };
} vec2_t;

vec2_t get_vec2(float x, float h);

vec2_t sum_vec2(vec2_t a, vec2_t b);

vec2_t sub_vec2(vec2_t a, vec2_t b);

vec2_t scale_vec2(vec2_t vec, float scaler);

vec2_t div_vec2(vec2_t vec, float scaler);

SDL_Rect get_rect(vec2_t pos, vec2_t size);

#endif //SDL_GAME_MATHS_H
