//
// Created by matheus on 16/09/2018.
//

#include <engine.h>

typedef struct key_bind {
    KEY up;
    KEY down;
} key_bind_t;

typedef struct ball {
    transform_t transform;
    sprite_renderer_t *renderer;
    vec2_t velocity;
    collider_t *collider;
} ball_t;

typedef struct paddle {
    transform_t transform;
    sprite_renderer_t *renderer;
    
    collider_t *collider;
    key_bind_t key_bind;
} paddle_t;

engine_data_t *engine;// = init_engine(&default_params);
texture_t paddle_texture;

void setup_paddle(paddle_t *paddle, PIVOT pivot) {
    paddle->transform = IDENTITY_TRANS;
    paddle->renderer = get_sprite_renderer(&engine->graphics_data, &paddle_texture);
    paddle->renderer->transform = &paddle->transform;
    paddle->renderer->normalized_pivot = get_normalized_pivot_point(pivot);
}

int main(int handle, char **params) {
    window_parameters_t default_params;
    fill_default_window_parameters(&default_params);
    engine = init_engine(&default_params);

    #define PADDLE_SPEED 80
    #define PADDLE_COUNT 2
    paddle_t paddles[PADDLE_COUNT];
    
    paddle_t *left = &paddles[0];
    paddle_t *right = &paddles[1];
    
    ball_t ball = {};
    
    load_texture_from_file("data/pong/paddle.png", engine->video_data.sdl_renderer, &paddle_texture);
    
    texture_t ball_texture;
    load_texture_from_file("data/pong/ball.png", engine->video_data.sdl_renderer, &ball_texture);
    
    setup_paddle(left, PIVOT_CENTER_LEFT);
    setup_paddle(right, PIVOT_CENTER_RIGHT);

    ball.transform = IDENTITY_TRANS;
    ball.renderer = get_sprite_renderer(&engine->graphics_data, &ball_texture);
    ball.renderer->transform = &ball.transform;
    
    left->transform.position.x = 0;
    left->transform.position.y = (engine->video_data.resolution.height / 2);
    left->key_bind.up = KEY_w;
    left->key_bind.down = KEY_s;

    right->transform.position.x = (engine->video_data.resolution.width );
    right->transform.position.y = (engine->video_data.resolution.height / 2);
    right->key_bind.up = KEY_UP;
    right->key_bind.down = KEY_DOWN;
    
    ball.transform.position = div_vec2(engine->video_data.resolution, 2.f);
    
    while (!engine_should_quit(engine)) {
        engine_start_update(engine);
        
        engine_update_internal_systems(engine);
        
        engine_start_draw(engine);

        for (int i = 0; i < PADDLE_COUNT; ++i) {
            paddle_t *paddle = &paddles[i];
            
            vec2_t direction = VEC2_ZERO;
            if (is_key_held(&engine->input_data, paddle->key_bind.up)) {
                direction.y = -1;
            }

            if (is_key_held(&engine->input_data, paddle->key_bind.down)) {
                direction.y = 1;
            }
            
            vec2_t speed = scale_vec2(direction, PADDLE_SPEED * engine->time_data.dt);
            
            paddle->transform.position = sum_vec2(paddle->transform.position, speed); 
        }        
        
        engine_draw_internal_systems_and_flip_video(engine);
        
        engine_end_update(engine);
    }
}