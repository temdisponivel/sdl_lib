//
// Created by matheus on 16/09/2018.
//

#include <engine.h>

typedef struct key_bind {
    KEY up;
    KEY down;
} key_bind_t;

typedef struct ball {
    int id;
    transform_t transform;
    sprite_renderer_t *renderer;
    vec2_t velocity;
    collider_t *collider;
} ball_t;

typedef struct paddle {
    int id;
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
    paddle->collider = get_box_collider(&engine->physics_data, paddle->id, VEC2_ZERO);
}

int main(int handle, char **params) {
    window_parameters_t default_params;
    fill_default_window_parameters(&default_params);
    engine = init_engine(&default_params);

#define BALL_SPEED 250
#define PADDLE_SPEED 150
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

    ball.id = 2;
    ball.transform = IDENTITY_TRANS;
    ball.renderer = get_sprite_renderer(&engine->graphics_data, &ball_texture);
    ball.renderer->transform = &ball.transform;
    ball.collider = get_box_collider(&engine->physics_data, ball.id, VEC2_ZERO);
    ball.velocity = get_vec2(RANDOM01(), RANDOM01());

    left->id = 0;
    left->transform.position.x = 0;
    left->transform.position.y = (engine->video_data.resolution.height / 2);
    left->key_bind.up = KEY_w;
    left->key_bind.down = KEY_s;

    right->id = 1;
    right->transform.position.x = (engine->video_data.resolution.width);
    right->transform.position.y = (engine->video_data.resolution.height / 2);
    right->key_bind.up = KEY_UP;
    right->key_bind.down = KEY_DOWN;

    ball.transform.position = div_vec2(engine->video_data.resolution, 2.f);

    vec2_t ball_half_size = div_vec2(ball.renderer->sprite.texture_region.size, 2.f);
    while (!engine_should_quit(engine)) {
        engine_start_update(engine);

        engine_start_draw(engine);

        vec2_t ball_speed = scale_vec2(ball.velocity, BALL_SPEED * engine->time_data.dt);
        ball.transform.position = sum_vec2(ball.transform.position, ball_speed);
        update_collider_pos_based_on_renderer(ball.renderer, ball.collider);
        
        if (ball.transform.position.y - ball_half_size.y <= 0 || ball.transform.position.y + ball_half_size.y >= engine->video_data.resolution.y) {
            ball.velocity.y = -ball.velocity.y;
        }

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
            update_collider_pos_based_on_renderer(paddle->renderer, paddle->collider);

            for (int j = 0; j < paddle->collider->collision_enter_count; ++j) {
                collision_t *collision = paddle->collider->collision_enter[j];
                if (collision->other->owner == ball.id) {
                    ball.velocity.x = ball.velocity.x * -1;
                }
            }
        }

        engine_update_internal_systems(engine);

        engine_draw_internal_systems_and_flip_video(engine);

        engine_end_update(engine);
    }
}