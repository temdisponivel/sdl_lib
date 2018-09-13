//
// Created by matheus on 12/09/2018.
//

#include "maths.h"


ivec2_t get_vec2(int x, int y) {
    ivec2_t vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

ivec2_t sum_vec2(ivec2_t a, ivec2_t b) {
    ivec2_t result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

ivec2_t sub_vec2(ivec2_t a, ivec2_t b) {
    ivec2_t result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

ivec2_t scale_vec2(ivec2_t vec, float scaler) {
    ivec2_t result;
    result.x = vec.x * scaler;
    result.y = vec.y * scaler;
    return result;
}

ivec2_t div_vec2(ivec2_t vec, float scaler) {
    ivec2_t result;
    result.x = vec.x / scaler;
    result.y = vec.y / scaler;
    return result;
}


SDL_Rect get_rect(ivec2_t pos, ivec2_t size) {
    SDL_Rect rect;
    rect.x = pos.x;
    rect.y = pos.y;
    rect.w = size.width;
    rect.h = size.height;
    return rect;
}
