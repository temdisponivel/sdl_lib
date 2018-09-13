//
// Created by matheus on 13/09/2018.
//

#include "time.h"
#include "defines.h"

void start_frame(time_data_t *time_data) {
    float ticks = SDL_GetTicks() / 1000.f;
    time_data->_start_frame_time = ticks;
}

void end_frame(time_data_t *time_data) {
    float ticks;
    float dt;

    // If we have an target frame rate,
    // calculate our current frame rate and
    // wait the necessary time to execute the next frame
    if (time_data->target_frame_rate > 0) {
        ticks = SDL_GetTicks() / 1000.f;
        dt = ticks - time_data->_start_frame_time;
        float target_dt = 1.f / time_data->target_frame_rate;
        if (target_dt > dt) {
            uint target_wait = (uint) ((target_dt - dt) * 1000.f);
            SDL_Delay(target_wait);
        }
    }

    ticks = SDL_GetTicks() / 1000.f;
    dt = ticks - time_data->_start_frame_time;

    time_data->dt = dt * time_data->time_scale;
    time_data->unscaled_dt = dt;

    time_data->time += time_data->dt;
    time_data->unscaled_time += time_data->unscaled_dt;
}