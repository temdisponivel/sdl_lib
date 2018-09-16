//
// Created by matheus on 16/09/2018.
//

#ifndef SDL_GAME_VIDEO_H
#define SDL_GAME_VIDEO_H

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include "maths.h"
#include "input.h"
#include "graphics.h"

#define MAX_WINDOW_TITLE_LEN 64

typedef struct window_parameters {
    vec2_t resolution;
    bool full_screen;
    bool resizable;
    bool v_sync_on;
    char *title;
    color_t clear_color;
} window_parameters_t;

typedef struct video_data {
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;
    
    vec2_t resolution;
    bool full_screen_on;
    bool window_resizable;
    
    color_t clear_color;
    
    bool dirty;
    
    char window_title[MAX_WINDOW_TITLE_LEN];
} video_data_t;

bool init_video(video_data_t *video_data, const window_parameters_t *parameters);
void update_video_data(video_data_t *video_data);
void clear_window(video_data_t *video_data);
void flip_video(video_data_t *video_data);

void set_video_resolution(video_data_t *video_data, vec2_t resolution);
void set_video_full_screen(video_data_t *video_data, bool full_screen);
void set_window_title(video_data_t *video_data, const char *title);
void set_video_clear_color(video_data_t *video_data, color_t color);

#endif //SDL_GAME_VIDEO_H
