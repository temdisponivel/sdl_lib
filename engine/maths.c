//
// Created by matheus on 12/09/2018.
//

#include <math.h>
#include "maths.h"

vec2_t get_vec2(float x, float y) {
    vec2_t vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

vec2_t max_vec2(vec2_t a, vec2_t b) {
    vec2_t result;
    result.x = fmaxf(a.x, b.x);
    result.y = fmaxf(a.y, b.y);
    return result;
}


vec2_t min_vec2(vec2_t a, vec2_t b) {
    vec2_t result;
    result.x = fminf(a.x, b.x);
    result.y = fminf(a.y, b.y);
    return result;
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

vec2_t mul_vec2(vec2_t a, vec2_t b) {
    vec2_t result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
}

float sqrd_magnitude_vec2(vec2_t vec) {
    return (vec.x * vec.x + vec.y * vec.y);
}

float magnitude_vec2(vec2_t a) {
    float sqrd_mag = sqrd_magnitude_vec2(a);
    return SDL_sqrtf(sqrd_mag);
}

float sqrd_distance_vec2(vec2_t a, vec2_t b) {
    vec2_t diff = sub_vec2(a, b);
    return sqrd_magnitude_vec2(diff);
}

float distance_vec2(vec2_t a, vec2_t b) {
    float sqrd_dist = sqrd_distance_vec2(a, b);
    return SDL_sqrtf(sqrd_dist);
}

rect_t get_rect(vec2_t pos, vec2_t size) {
    rect_t rect;
    rect.position.x = pos.x;
    rect.position.y = pos.y;
    rect.size.width = size.width;
    rect.size.height = size.height;
    return rect;
}

SDL_Rect convert_rect(rect_t rect) {
    SDL_Rect result;
    result.x = (int) rect.position.x;
    result.y = (int) rect.position.y;
    result.w = (int) rect.size.width;
    result.h = (int) rect.size.height;
    return result;
}

vec2_t normalize_rect_point(rect_t rect, vec2_t point) {
    float x = point.x - rect.position.x;
    float y = point.y - rect.position.y;
    vec2_t normalized = get_vec2(x / rect.size.x, y / rect.size.y);
    return normalized;
}

bool is_point_inside_rect(rect_t rect, vec2_t point) {
    vec2_t normalized = normalize_rect_point(rect, point);
    if (normalized.x < 0 || normalized.x > 1)
        return false;
    else if (normalized.y < 0 || normalized.y > 1)
        return false;
    return true;
}

vec2_t denormalize_point(vec2_t region, vec2_t point) {
    vec2_t result;
    result.x = (region.width * point.x);
    result.y = (region.height * point.y);
    return result;
}

vec2_t denormalize_point_inside_rect(rect_t region, vec2_t point) {
    vec2_t result;
    result.x = region.position.x + (region.size.width * point.x);
    result.y = region.position.y + (region.size.height * point.y);
    return result;
}

vec2_t get_normalized_pivot_point(PIVOT pivot) {
    float x = 0;
    float y = 0;

    if ((pivot & PIVOT_CENTER) != 0) {
        x = .5f;
        y = .5f;
    }

    if ((pivot & PIVOT_BOTTOM) != 0) {
        y = 1;
    }

    if ((pivot & PIVOT_TOP) != 0) {
        y = 0;
    }

    if ((pivot & PIVOT_LEFT) != 0) {
        x = 0;
    }

    if ((pivot & PIVOT_RIGHT) != 0) {
        x = 1;
    }

    return get_vec2(x, y);
}

SDL_Point convert_vec2(vec2_t vec) {
    SDL_Point point;
    point.x = (int) vec.x;
    point.y = (int) vec.y;
    return point;
}

transform_t get_trans(vec2_t pos, float angle, vec2_t scale) {
    transform_t trans;
    trans.position = pos;
    trans.angle = angle;
    trans.scale = scale;
    return trans;
}

circle_t get_circle(vec2_t pos, float radius) {
    circle_t circle;
    circle.position = pos;
    circle.radius = radius;
    return circle;
}

bool collide_rects(rect_t box_a, rect_t box_b) {
    vec2_t normalized_b_to_a = normalize_rect_point(box_a, box_b.position);
    if (normalized_b_to_a.x < -1 || normalized_b_to_a.x > 1) {
        return false;
    } else if (normalized_b_to_a.y < -1 || normalized_b_to_a.y > 1) {
        return false;
    } else {
        return true;
    }
}

bool collide_circles(circle_t circle_a, circle_t circle_b) {
    float distance = sqrd_distance_vec2(circle_a.position, circle_b.position);
    float sqrd_radius = SQUARE(circle_a.radius + circle_b.radius);
    return (distance <= sqrd_radius);
}

bool collide_rect_circle(rect_t box, circle_t circle) {
    float width = circle.radius * 2;
    vec2_t size = get_vec2(width, width);
    vec2_t half_size = div_vec2(size, 2.f);
    vec2_t position = sub_vec2(circle.position, half_size);
    rect_t circle_rect = get_rect(position, size);
    
    return collide_rects(box, circle_rect);
}

float get_current_interpolation_value(const interpolation_t *interpolation) {
    float delta = interpolation->time / interpolation->duration;
    return LERP(interpolation->start, interpolation->target, delta);
}

interpolation_t blank_interpolation() {
    interpolation_t result = {};
    return result;
}