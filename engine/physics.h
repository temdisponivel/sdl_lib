//
// Created by matheus on 14/09/2018.
//

#ifndef SDL_GAME_PHYSICS_H
#define SDL_GAME_PHYSICS_H

#include "maths.h"
#include "graphics.h"

#define MAX_COLLIDERS 256
#define MAX_COLLISIONS_PER_COLLIDER 16

typedef enum {
    TOP_LEFT_AREA = 1 << 0,
    TOP_RIGHT_AREA = 1 << 1,
    BOTTOM_LEFT_AREA = 1 << 2,
    BOTTOM_RIGHT_AREA = 1 << 3,
} WORLD_AREA;

typedef enum {
    CIRCLE,
    BOX,
} COLLIDER_SHAPE;

struct collider;

typedef struct collision {
    struct collider *self;
    struct collider *other;
} collision_t;

typedef struct collision_list {
    collision_t collisions[MAX_COLLISIONS_PER_COLLIDER];
    int collision_count;
} collision_list_t; 

typedef struct collider {
    int owner;
    bool active;
    
    vec2_t position;
    COLLIDER_SHAPE shape;
    
    union {
        float circle_radius;
        vec2_t box_size;
    };
    
    collision_t *collision_enter[MAX_COLLISIONS_PER_COLLIDER];
    int collision_enter_count;
    
    collision_t *collision_stay[MAX_COLLISIONS_PER_COLLIDER];
    int collision_stay_count;
    
    collision_t *collision_exit[MAX_COLLISIONS_PER_COLLIDER];
    int collision_exit_count;

    collision_list_t last_frame_collisions;
    collision_list_t current_frame_collisions;
} collider_t;

// TODO: Add areas to better performance (octree or something)
typedef struct physics_data {
    collider_t colliders[MAX_COLLIDERS];
    int colliders_count;
} physics_data_t;

collider_t *get_box_collider(physics_data_t *physics_data, int owner, vec2_t size);
collider_t *get_circle_collider(physics_data_t *physics_data, int owner, float radius);
void free_collider(physics_data_t *physics_data, collider_t *collider);

// TODO: This is not the best place for this function because we'll have to include graphics.h
// which basically couples the physics with the graphic system - an better place would be an file
// with entity-component-related functions, although I don't think we'll use entity-component here
void update_collider_pos_based_on_renderer(const sprite_renderer_t *sprite_renderer, collider_t *collider);

bool validate_collision(const collider_t *collider_a, const collider_t *collider_b);

void update_physics_data(physics_data_t *physics_data);

void draw_collider_debug(SDL_Renderer *renderer, collider_t *collider);
void draw_physics_debug(SDL_Renderer *renderer, physics_data_t *physics_data);

#endif //SDL_GAME_PHYSICS_H
