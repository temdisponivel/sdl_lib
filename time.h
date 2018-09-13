//
// Created by matheus on 13/09/2018.
//

#ifndef SDL_GAME_TIME_H
#define SDL_GAME_TIME_H

typedef struct time_data {
    float _start_frame_time;
    float unscaled_dt;
    float dt;
    float time_scale;
    float time;
    float unscaled_time;
    int target_frame_rate; // 0 for unclamped
} time_data_t;

void start_frame(time_data_t *time_data);
void end_frame(time_data_t *time_data);

#endif //SDL_GAME_TIME_H
