//
// Created by matheus on 12/09/2018.
//

#ifndef SDL_GAME_ENGINE_H
#define SDL_GAME_ENGINE_H

#include "defines.h"
#include "graphics.h"
#include "input.h"
#include "physics.h"
#include "audio.h"
#include "video.h"
#include "gui.h"

typedef struct engine_data {
    graphics_data_t graphics_data;
    input_data_t input_data;
    time_data_t time_data ;
    physics_data_t physics_data;
    audio_data_t audio_data;
    video_data_t video_data;
} engine_data_t;

engine_data_t *init_engine(const window_parameters_t *window_parameters);
void free_engine(engine_data_t *engine_data);

void engine_start_update(engine_data_t *engine_data);
void engine_update_internal_systems(engine_data_t *engine_data);

void engine_start_draw(engine_data_t *engine_data);
void engine_draw_internal_systems(engine_data_t *engine_data);
void engine_flip_buffers(engine_data_t *engine_data);

void engine_end_update(engine_data_t *engine_data);

bool engine_should_quit(engine_data_t *engine_data);

#endif //SDL_GAME_ENGINE_H
