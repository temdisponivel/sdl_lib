//
// Created by matheus on 14/09/2018.
//

#include "physics.h"
#include "graphics.h"
#include <math.h>

void set_draw_color(SDL_Renderer *render, color_t color) {
    SDL_SetRenderDrawColor(render, color.red, color.green, color.blue, color.alpha);
}

collider_t *get_base_collider(physics_data_t *physics_data, int owner, COLLIDER_SHAPE shape) {
    SDL_assert(physics_data->colliders_count < MAX_COLLIDERS);

    collider_t *collider = &physics_data->colliders[physics_data->colliders_count++];

    collider->position = VEC2_ZERO;
    collider->collision_count = 0;
    collider->owner = owner;
    collider->shape = shape;

    return collider;
}

collider_t *get_box_collider(physics_data_t *physics_data, int owner, vec2_t size) {
    collider_t *collider = get_base_collider(physics_data, owner, BOX);
    collider->box_size = size;
    return collider;
}

collider_t *get_circle_collider(physics_data_t *physics_data, int owner, float radius) {
    collider_t *collider = get_base_collider(physics_data, owner, CIRCLE);
    collider->circle_radius = radius;
    return collider;
}

void free_collider(physics_data_t *physics_data, collider_t *collider) {
    SDL_memmove(collider, &physics_data->colliders[physics_data->colliders_count - 1], sizeof(collider_t));
    physics_data->colliders_count--;
}

void update_collider_pos_based_on_renderer(const sprite_renderer_t *sprite_renderer, collider_t *collider) {
    transform_t *transform = &sprite_renderer->transform;
    
    vec2_t position = transform->position;
    vec2_t size = sprite_renderer->sprite.texture_region.size;
    vec2_t scale = transform->scale;
    vec2_t pivot = sprite_renderer->normalized_pivot;
    
    if (collider->shape == BOX) {
        rect_t sprite_rect = calculate_rect_based_on_pivot_and_scale(
                position, 
                size, 
                scale, 
                pivot
        );
        
        collider->position = sprite_rect.position;
        collider->box_size = sprite_rect.size;
    } else {
        
        rect_t sprite_rect = calculate_rect_based_on_pivot_and_scale(
                position,
                size,
                scale,
                pivot
        );

        // Circles always use the center pivot
        vec2_t center_pivot = get_normalized_pivot_point(PIVOT_CENTER);
        
        vec2_t center_of_drawing = denormalize_point(sprite_rect.size, center_pivot);
        center_of_drawing = sum_vec2(center_of_drawing, sprite_rect.position);
        
        collider->position = center_of_drawing;
        collider->circle_radius = sprite_rect.size.x;
    }
}

bool validate_collision(const collider_t *collider_a, const collider_t *collider_b) {
    if (collider_a->shape == collider_b->shape) {
        if (collider_a->shape == CIRCLE) {
            circle_t circle_a = get_circle(collider_a->position, collider_a->circle_radius);
            circle_t circle_b = get_circle(collider_b->position, collider_b->circle_radius);
            return collide_circles(circle_a, circle_b);
        } else {
            rect_t rect_a = get_rect(collider_a->position, collider_a->box_size);
            rect_t rect_b = get_rect(collider_b->position, collider_b->box_size);
            return collide_rects(rect_a, rect_b);
        }
    } else {
        if (collider_a->shape == CIRCLE) {
            circle_t circle_a = get_circle(collider_a->position, collider_a->circle_radius);
            rect_t rect_b = get_rect(collider_b->position, collider_b->box_size);
            return collide_rect_circle(rect_b, circle_a);
        } else {
            rect_t rect_a = get_rect(collider_a->position, collider_a->box_size);
            circle_t circle_b = get_circle(collider_b->position, collider_b->circle_radius);
            return collide_rect_circle(rect_a, circle_b);
        }
    }
}

void update_physics_data(physics_data_t *physics_data) {
    for (int i = 0; i < physics_data->colliders_count; ++i) {
        physics_data->colliders[i].collision_count = 0;
    }

    for (int i = 0; i < physics_data->colliders_count; ++i) {
        collider_t *collider_a = &physics_data->colliders[i];

        for (int j = i + 1; j < physics_data->colliders_count; ++j) {
            collider_t *collider_b = &physics_data->colliders[j];
            bool collided = validate_collision(collider_a, collider_b);

            if (collided) {

                SDL_assert(collider_a->collision_count < MAX_COLLISIONS_PER_COLLIDER);
                SDL_assert(collider_b->collision_count < MAX_COLLISIONS_PER_COLLIDER);

                collision_t *collision_a = &collider_a->collisions[collider_a->collision_count++];
                collision_a->first = collider_a;
                collision_a->second = collider_b;

                collision_t *collision_b = &collider_b->collisions[collider_b->collision_count++];
                collision_b->first = collider_b;
                collision_b->second = collider_a;
            }
        }
    }
}

void draw_collider_debug(SDL_Renderer *renderer, collider_t *collider) {
    color_t color;
    if (collider->collision_count > 0)
        color = COLOR_BLUE;
    else
        color = COLOR_RED;

    if (collider->shape == BOX) {
        rect_t rect = get_rect(collider->position, collider->box_size);
        debug_draw_rect(renderer, rect, color);
    } else {
        debug_draw_circle(renderer, collider->position, collider->circle_radius, color);
    }
}

void draw_physics_debug(SDL_Renderer *renderer, physics_data_t *physics_data) {
    for (int i = 0; i < physics_data->colliders_count; ++i) {
        collider_t *collider = &physics_data->colliders[i];
        draw_collider_debug(renderer, collider);
    }
}