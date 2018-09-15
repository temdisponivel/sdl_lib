//
// Created by matheus on 12/09/2018.
//

#ifndef SDL_GAME_MATHS_H
#define SDL_GAME_MATHS_H

#include "defines.h"


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

} PIVOT;

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

// TODO: Add parents
typedef struct transform {
    vec2_t world_pos;
    union {
        float clockwise_angle;
        float angle;
    };
    vec2_t scale;
} transform_t;

typedef struct rect {
    vec2_t position;
    vec2_t size;
} rect_t;

#define VEC2_ZERO get_vec2(0, 0)
#define VEC2_ONE get_vec2(1, 1)
#define VEC2_UP get_vec2(0, 1)
#define VEC2_DOWN get_vec2(0, -1)
#define VEC2_LEFT get_vec2(-1, 0)
#define VEC2_RIGHT get_vec2(1, 0)
#define IDENTITY_TRANS get_trans(VEC2_ZERO, 0, VEC2_ONE)

vec2_t get_vec2(float x, float h);

vec2_t sum_vec2(vec2_t a, vec2_t b);

vec2_t sub_vec2(vec2_t a, vec2_t b);

vec2_t scale_vec2(vec2_t vec, float scaler);

vec2_t div_vec2(vec2_t vec, float scaler);

vec2_t mul_vec2(vec2_t a, vec2_t b);

rect_t get_rect(vec2_t pos, vec2_t size);

SDL_Rect convert_rect(rect_t rect);

vec2_t normalize_rect_point(rect_t rect, vec2_t point);

vec2_t denormalize_rect_point(rect_t rect, vec2_t point);

vec2_t get_rect_pivot(rect_t rect, PIVOT pivot);

vec2_t get_normalized_pivot_point(PIVOT pivot);

SDL_Point convert_vec2(vec2_t vec);

transform_t get_trans(vec2_t pos, float angle, vec2_t scale);

#endif //SDL_GAME_MATHS_H
