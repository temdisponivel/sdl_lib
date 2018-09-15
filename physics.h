//
// Created by matheus on 14/09/2018.
//

#ifndef SDL_GAME_PHYSICS_H
#define SDL_GAME_PHYSICS_H

#include "maths.h"

#define MAX_COLLIDERS 128
#define MAX_COLLISIONS MAX_COLLIDERS * MAX_COLLIDERS
#define WORLD_AREAS 4
#define MAX_COLLISIONS_PER_COLLIDER 16

typedef enum {
    TOP_LEFT_AREA,
    TOP_RIGHT_AREA,
    BOTTOM_LEFT_AREA,
    BOTTOM_RIGHT_AREA,
} WORLD_AREA;

typedef enum {
    CIRCLE,
    BOX,
} COLLIDER_SHAPE;

struct collider;

typedef struct collision {
    struct collider *first;
    struct collider *second;
} collision_t;

typedef struct collider {
    int owner;
    bool active;
    
    vec2_t position;
    COLLIDER_SHAPE shape;
    
    union {
        float circle_radius;
        vec2_t box_size;
    };
    
    collision_t collisions[MAX_COLLISIONS_PER_COLLIDER];
    int collision_count;
} collider_t;

typedef struct world_area {
    collider_t *colliders[MAX_COLLIDERS];
    int colliders_count;
} world_area_t;

typedef struct physics_data {
    collider_t colliders[MAX_COLLIDERS];
    int colliders_count;

    world_area_t areas[WORLD_AREAS];
} physics_data_t;

collider_t *get_box_collider(physics_data_t *physics_data, int owner, vec2_t size);
collider_t *get_circle_collider(physics_data_t *physics_data, int owner, float radius);
void destroy_collider(physics_data_t *physics_data, collider_t *collider);

bool validate_collision(const collider_t *collider_a, const collider_t *collider_b);

void update_physics_data(physics_data_t *physics_data);

void draw_collider_debug(SDL_Renderer *renderer, collider_t *collider);
void draw_physics_debug(SDL_Renderer *renderer, physics_data_t *physics_data);

#endif //SDL_GAME_PHYSICS_H
