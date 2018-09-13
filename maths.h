//
// Created by matheus on 12/09/2018.
//

#ifndef SDL_GAME_MATHS_H
#define SDL_GAME_MATHS_H

#include "defines.h"

typedef struct ivec2 {
    union {
        int x;
        int width;
    };
    
    union {
        int y;
        int height;
    };
} ivec2_t;

ivec2_t get_vec2(int x, int h);

ivec2_t sum_vec2(ivec2_t a, ivec2_t b);

ivec2_t sub_vec2(ivec2_t a, ivec2_t b);

ivec2_t scale_vec2(ivec2_t vec, float scaler);

ivec2_t div_vec2(ivec2_t vec, float scaler);

SDL_Rect get_rect(ivec2_t pos, ivec2_t size);

#endif //SDL_GAME_MATHS_H
