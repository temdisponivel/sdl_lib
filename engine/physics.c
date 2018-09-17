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
    collider->current_frame_collisions.collision_count = 0;
    collider->last_frame_collisions.collision_count = 0;
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
    transform_t *transform = sprite_renderer->transform;

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

        vec2_t center_pivot = get_normalized_pivot_point(PIVOT_CENTER);

        vec2_t center_of_drawing = denormalize_point(sprite_rect.size, center_pivot);
        center_of_drawing = sum_vec2(center_of_drawing, sprite_rect.position);

        collider->position = center_of_drawing;
        collider->circle_radius = sprite_rect.size.width / 2.f;
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

void add_collision(collider_t *collider, collider_t *other) {
    if (collider->current_frame_collisions.collision_count < MAX_COLLISIONS_PER_COLLIDER) {

        collision_t *collision_a = &collider->current_frame_collisions.collisions[collider->current_frame_collisions.collision_count++];
        collision_a->self = collider;
        collision_a->other = other;

    } else {
        SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "The collider owned by '%i' has more collisions than we can handle!", collider->owner);
    }
}

int find_collision_index(collision_list_t *list, collider_t *other) {
    for (int i = 0; i < list->collision_count; ++i) {
        if (list->collisions[i].other == other) {
            return i;
        }
    }

    return -1;
}

void update_physics_data(physics_data_t *physics_data) {
    for (int i = 0; i < physics_data->colliders_count; ++i) {
        physics_data->colliders[i].last_frame_collisions = physics_data->colliders[i].current_frame_collisions;
        physics_data->colliders[i].current_frame_collisions.collision_count = 0;
    }

    for (int i = 0; i < physics_data->colliders_count; ++i) {
        collider_t *collider_a = &physics_data->colliders[i];

        for (int j = i + 1; j < physics_data->colliders_count; ++j) {
            collider_t *collider_b = &physics_data->colliders[j];
            bool collided = validate_collision(collider_a, collider_b);

            if (collided) {
                add_collision(collider_a, collider_b);
                add_collision(collider_b, collider_a);
            }
        }
    }

    for (int i = 0; i < physics_data->colliders_count; ++i) {
        collider_t *collider = &physics_data->colliders[i];
        collider->collision_enter_count = 0;
        collider->collision_stay_count = 0;
        collider->collision_exit_count = 0;

        for (int j = 0; j < collider->last_frame_collisions.collision_count; ++j) {
            collision_t *last_frame = &collider->last_frame_collisions.collisions[j];

            int index = find_collision_index(&collider->current_frame_collisions, last_frame->other);
            if (index == -1) {
                collider->collision_exit[collider->collision_exit_count++] = last_frame;
            } else {
                collider->collision_stay[collider->collision_stay_count++] = &collider->current_frame_collisions.collisions[index];
            }
        }

        for (int j = 0; j < collider->current_frame_collisions.collision_count; ++j) {
            collision_t *current_frame = &collider->current_frame_collisions.collisions[j];

            int index = find_collision_index(&collider->last_frame_collisions, current_frame->other);
            if (index == -1) {
                collider->collision_enter[collider->collision_enter_count++] = current_frame;
            }
        }
    }
}

void draw_collider_debug(SDL_Renderer *renderer, const camera_t *camera, collider_t *collider) {
    color_t color;
    if (collider->current_frame_collisions.collision_count > 0)
        color = COLOR_BLUE;
    else
        color = COLOR_RED;

    if (collider->shape == BOX) {
        rect_t rect = get_rect(collider->position, collider->box_size);
        rect.position = world_to_camera_pos(rect.position, camera);
        debug_draw_rect(renderer, rect, color);
    } else {
        vec2_t position = world_to_camera_pos(collider->position, camera);
        debug_draw_circle(renderer, position, collider->circle_radius, color);
    }
}

void draw_physics_debug(SDL_Renderer *renderer, const camera_t *camera, physics_data_t *physics_data) {
    for (int i = 0; i < physics_data->colliders_count; ++i) {
        collider_t *collider = &physics_data->colliders[i];
        draw_collider_debug(renderer, camera, collider);
    }
}