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

void destroy_collider(physics_data_t *physics_data, collider_t *collider) {
    SDL_memmove(collider, &physics_data->colliders[physics_data->colliders_count - 1], sizeof(collider_t));
    physics_data->colliders_count--;
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

    for (int i = 0; i < WORLD_AREAS; ++i) {
        physics_data->areas[i].colliders_count = 0;
    }

    for (int i = 0; i < physics_data->colliders_count; ++i) {
        collider_t *collider = &physics_data->colliders[i];

        WORLD_AREA area;
        if (collider->position.x < 0) {
            if (collider->position.y < 0)
                area = TOP_LEFT_AREA;
            else
                area = BOTTOM_LEFT_AREA;
        } else {
            if (collider->position.y < 0)
                area = TOP_RIGHT_AREA;
            else
                area = BOTTOM_RIGHT_AREA;
        }
        
        world_area_t *world_area = &physics_data->areas[area];
        
        SDL_assert(world_area->colliders_count < MAX_COLLIDERS);
        
        world_area->colliders[world_area->colliders_count++] = collider;
    }

    for (int i = 0; i < WORLD_AREAS; ++i) {
        world_area_t area = physics_data->areas[i];
        
        for (int j = 0; j < area.colliders_count; ++j) {
            collider_t *collider_a = area.colliders[j];
            
            for (int k = j + 1; k < area.colliders_count; ++k) {
                collider_t *collider_b = area.colliders[k];
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
}

void draw_collider_debug(SDL_Renderer *renderer, collider_t *collider) {
    if (collider->collision_count > 0)
        set_draw_color(renderer, COLOR_BLUE);
    else
        set_draw_color(renderer, COLOR_RED);
    
    if (collider->shape == BOX) {
        rect_t rect = get_rect(collider->position, collider->box_size);
        SDL_Rect sdl_rect = convert_rect(rect);
        SDL_RenderDrawRect(renderer, &sdl_rect);
    } else {
        #define POINTS_COUNT 360
        
        SDL_Point points[POINTS_COUNT];

        for (int i = 0; i < POINTS_COUNT; ++i) {
            float x = sin(i) * collider->circle_radius;
            float y = cos(i) * collider->circle_radius;
            x += collider->position.x;
            y += collider->position.y;
            
            points[i].x = x;
            points[i].y = y;
        }
        
        SDL_RenderDrawPoints(renderer, points, POINTS_COUNT);
    }
}

void draw_physics_debug(SDL_Renderer *renderer, physics_data_t *physics_data) {
    for (int i = 0; i < physics_data->colliders_count; ++i) {
        collider_t *collider = &physics_data->colliders[i];
        draw_collider_debug(renderer, collider);
    }
}