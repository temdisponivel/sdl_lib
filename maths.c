//
// Created by matheus on 12/09/2018.
//

#include "maths.h"

vec2_t get_vec2(float x, float y) {
    vec2_t vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

vec2_t sum_vec2(vec2_t a, vec2_t b) {
    vec2_t result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

vec2_t sub_vec2(vec2_t a, vec2_t b) {
    vec2_t result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

vec2_t scale_vec2(vec2_t vec, float scaler) {
    vec2_t result;
    result.x = vec.x * scaler;
    result.y = vec.y * scaler;
    return result;
}

vec2_t div_vec2(vec2_t vec, float scaler) {
    vec2_t result;
    result.x = vec.x / scaler;
    result.y = vec.y / scaler;
    return result;
}

SDL_Rect get_rect(vec2_t pos, vec2_t size) {
    SDL_Rect rect;
    rect.x = (int) pos.x;
    rect.y = (int) pos.y;
    rect.w = (int) size.width;
    rect.h = (int) size.height;
    return rect;
}
