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
// TODO: Make all transforms be reference by pointers
// so that when we change a transform in one component, all
// others will see the change
typedef struct transform {
    vec2_t position;
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

typedef struct circle {
    vec2_t position;
    float radius;
} circle_t;

typedef struct interpolation {
    float start;
    float target;
    
    float duration;
    float time;
} interpolation_t;

#define VEC2_ZERO get_vec2(0, 0)
#define VEC2_ONE get_vec2(1, 1)
#define VEC2_UP get_vec2(0, 1)
#define VEC2_DOWN get_vec2(0, -1)
#define VEC2_LEFT get_vec2(-1, 0)
#define VEC2_RIGHT get_vec2(1, 0)
#define IDENTITY_TRANS get_trans(VEC2_ZERO, 0, VEC2_ONE)

#define PI 3.14159265359
#define TAU 2 * PI

#define SQUARE(x) ((x) * (x))
#define LERP(x, y, delta) (((x) * (1 - (delta))) + ((y) * (delta)))
#define RADIANS(degrees) (((degrees) / 180.f) * PI)
#define DEGREES(radians)  ((180.f / PI) * (radians))  

vec2_t get_vec2(float x, float h);

vec2_t max_vec2(vec2_t a, vec2_t b);

vec2_t min_vec2(vec2_t a, vec2_t b);

vec2_t sum_vec2(vec2_t a, vec2_t b);

vec2_t sub_vec2(vec2_t a, vec2_t b);

vec2_t scale_vec2(vec2_t vec, float scaler);

vec2_t div_vec2(vec2_t vec, float scaler);

vec2_t mul_vec2(vec2_t a, vec2_t b);

float sqrd_magnitude_vec2(vec2_t vec);

float magnitude_vec2(vec2_t a);

float sqrd_distance_vec2(vec2_t a, vec2_t b);

float distance_vec2(vec2_t a, vec2_t b);

rect_t get_rect(vec2_t pos, vec2_t size);

SDL_Rect convert_rect(rect_t rect);

vec2_t normalize_rect_point(rect_t rect, vec2_t point);

bool is_point_inside_rect(rect_t rect, vec2_t point);

vec2_t denormalize_point(vec2_t region, vec2_t point);

vec2_t get_normalized_pivot_point(PIVOT pivot);

SDL_Point convert_vec2(vec2_t vec);

transform_t get_trans(vec2_t pos, float angle, vec2_t scale);

circle_t get_circle(vec2_t pos, float radius);

bool collide_rects(rect_t box_a, rect_t box_b);

bool collide_circles(circle_t circle_a, circle_t circle_b);

bool collide_rect_circle(rect_t box, circle_t circle);

float get_current_interpolation_value(const interpolation_t *interpolation);

interpolation_t blank_interpolation();

#endif //SDL_GAME_MATHS_H
