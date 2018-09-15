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

vec2_t mul_vec2(vec2_t a, vec2_t b) {
    vec2_t result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
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

vec2_t normalized_circle_point(circle_t circle, vec2_t point) {
    float x = point.x - circle.position.x;
    float y = point.y - circle.position.y;
    vec2_t normalized = get_vec2(x / circle.radius, y / circle.radius);
    return normalized;
}

vec2_t denormalize_rect_point(rect_t rect, vec2_t point) {
    vec2_t result;
    result.x = rect.position.x + (rect.size.x * point.x);
    result.y = rect.position.y + (rect.size.y * point.y);
    return result;
}

vec2_t get_rect_pivot(rect_t rect, PIVOT pivot) {
    vec2_t normalized_pivot = get_normalized_pivot_point(pivot);
    vec2_t denormalized_pivot = denormalize_rect_point(rect, normalized_pivot);
    return denormalized_pivot;
}

rect_t calculate_rect(vec2_t world_pos, vec2_t size, vec2_t scale, vec2_t normalized_pivot) {
    vec2_t screen_size = mul_vec2(size, scale);

    vec2_t pivot = denormalize_rect_point(get_rect(VEC2_ZERO, screen_size), normalized_pivot);
    world_pos = sub_vec2(world_pos, pivot);

    rect_t screen_region = get_rect(world_pos, screen_size);
    return screen_region;
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
    trans.world_pos = pos;
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
    vec2_t normalized_b_to_a = normalized_circle_point(circle_a, circle_b.position);
    if (normalized_b_to_a.x < -1 || normalized_b_to_a.x > 1) {
        return false;
    } else if (normalized_b_to_a.y < -1 || normalized_b_to_a.y > 1) {
        return false;
    } else {
        return true;
    }
}

bool collide_rect_circle(rect_t box, circle_t circle) {
    rect_t circle_rect = get_rect(circle.position, get_vec2(circle.radius, circle.radius));
    return collide_rects(box, circle_rect);
}