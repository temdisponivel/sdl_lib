//
// Created by matheus on 16/09/2018.
//

#include <engine.h>
#include <stdio.h>

typedef struct key_bind {
    KEY up;
    KEY down;
} key_bind_t;

typedef struct ball {
    int id;
    transform_t transform;
    sprite_renderer_t *renderer;
    vec2_t direction;
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
vec2_t screen_center;

void setup_paddle(paddle_t *paddle, PIVOT pivot) {
    paddle->transform = IDENTITY_TRANS;
    paddle->renderer = get_sprite_renderer(&engine->graphics_data, &paddle_texture);
    paddle->renderer->transform = &paddle->transform;
    paddle->renderer->normalized_pivot = get_normalized_pivot_point(pivot);
    paddle->collider = get_box_collider(&engine->physics_data, paddle->id, VEC2_ZERO);
}

void start_ball(ball_t *ball) {
    ball->transform = IDENTITY_TRANS;
    ball->transform.position = screen_center;
    ball->direction = normalize_vec2(get_vec2(RANDOM01(), RANDOM01()));
}

int main(int handle, char **params) {
//int not_main() {
    window_parameters_t default_params;
    fill_default_window_parameters(&default_params);
    engine = init_engine(&default_params);

    #define BALL_SPEED 400
    #define PADDLE_SPEED 300
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
    ball.renderer = get_sprite_renderer(&engine->graphics_data, &ball_texture);
    ball.renderer->transform = &ball.transform;
    ball.collider = get_box_collider(&engine->physics_data, ball.id, VEC2_ZERO);

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

    sound_t ball_kick_sound = load_sound_from_file("data/pong/ball_kick.wav", EFFECT);
    audio_source_t *ball_kick_source = get_audio_source(&engine->audio_data, ball_kick_sound);

    sound_t goal_sound = load_sound_from_file("data/pong/goal.wav", EFFECT);
    audio_source_t *goal_source = get_audio_source(&engine->audio_data, goal_sound);
    
    sound_t background_music = load_sound_from_file("data/music.mp3", MUSIC);
    audio_source_t *background_music_source = get_audio_source(&engine->audio_data, background_music);
    background_music_source->volume = 0;
    fade_in_audio_source(background_music_source, 5);
    play_audio_source(background_music_source);

    font_t font;
    init_font_from_file(engine->video_data.sdl_renderer, &font, "data/consola.ttf", 24, FONT_NORMAL);

    label_t points_label;
    setup_label(&points_label, &font, "%i X %i", COLOR_WHITE);

    label_t button_label;
    setup_label(&button_label, &font, "START", COLOR_BLUE);
    
    sprite_t sprite = create_sprite(&engine->graphics_data.white_texture);
    button_t start_button;
    setup_button(&start_button, button_label, get_vec2(200, 75), sprite, COLOR_WHITE, COLOR_GREY, COLOR_BLACK, PIVOT_CENTER);

    vec2_t ball_half_size = div_vec2(ball.renderer->sprite.texture_region.size, 2.f);

    int left_points = 0;
    int right_points = 0;

    screen_center = div_vec2(engine->video_data.resolution, 2);

    bool started = false;

    while (!engine_should_quit(engine)) {
        engine_start_update(engine);

        engine_start_draw(engine);

        if (started) {
            if (ball.transform.position.x - ball_half_size.x <= 0) {
                right_points++;
                start_ball(&ball);
                play_audio_source(goal_source);
            } else if (ball.transform.position.x + ball_half_size.x >= engine->video_data.resolution.width) {
                left_points++;
                start_ball(&ball);
                play_audio_source(goal_source);
            }

            vec2_t ball_speed = scale_vec2(ball.direction, BALL_SPEED * engine->time_data.dt);

            vec2_t desired_ball_pos = sum_vec2(ball.transform.position, ball_speed);
            if (desired_ball_pos.y - ball_half_size.y <= 0 || desired_ball_pos.y + ball_half_size.y >= engine->video_data.resolution.y) {
                ball.direction.y = ball.direction.y * -1;
                play_audio_source(ball_kick_source);
            } else {
                ball.transform.position = desired_ball_pos;
            }

            update_collider_pos_based_on_renderer(ball.renderer, ball.collider);

            for (int i = 0; i < PADDLE_COUNT; ++i) {
                paddle_t *paddle = &paddles[i];

                vec2_t direction = VEC2_ZERO;
                if (is_key_held(&engine->input_data, paddle->key_bind.up)) {
                    direction.y = -1;
                }

                if (is_key_held(&engine->input_data, paddle->key_bind.down)) {
                    direction.y = 1;
                }

                vec2_t half_paddle = div_vec2(paddle->renderer->sprite.texture_region.size, 2.f);

                vec2_t speed = scale_vec2(direction, PADDLE_SPEED * engine->time_data.dt);
                vec2_t desired_pos = sum_vec2(paddle->transform.position, speed);
                if (desired_pos.y - half_paddle.y >= 0 && desired_pos.y + half_paddle.y <= engine->video_data.resolution.height) {
                    paddle->transform.position = desired_pos;
                }

                update_collider_pos_based_on_renderer(paddle->renderer, paddle->collider);

                for (int j = 0; j < paddle->collider->collision_enter_count; ++j) {
                    collision_t *collision = paddle->collider->collision_enter[j];
                    if (collision->other->owner == ball.id) {
                        ball.direction.x = ball.direction.x * -1;
                        play_audio_source(ball_kick_source);
                    }
                }
            }
        }

        if (is_key_pressed(&engine->input_data, KEY_r)) {
            start_ball(&ball);
        }

        engine_update_internal_systems(engine);

        engine_draw_internal_systems(engine);

        if (started) {
            string_format(points_label.text, "%i X %i", left_points, right_points);
            draw_label(engine->video_data.sdl_renderer, screen_center, &points_label);
        } else {
            started = draw_button(engine->video_data.sdl_renderer, &engine->input_data, screen_center, &start_button);            
            start_ball(&ball);
        }

        engine_flip_buffers(engine);

        engine_end_update(engine);
    }
}