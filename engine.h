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

typedef struct engine_data {
    graphics_data_t graphics_data;
    input_data_t input_data;
    time_data_t time_data ;
    physics_data_t physics_data;
    audio_data_t audio_data;
    video_data_t video_data;
} engine_data_t;

#endif //SDL_GAME_ENGINE_H
