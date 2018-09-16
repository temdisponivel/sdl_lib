//
// Created by matheus on 16/09/2018.
//

#include "video.h"
#include "graphics.h"

void fill_default_window_parameters(window_parameters_t *parameters) {
    parameters->resolution = get_vec2(800, 600);
    parameters->full_screen = false;
    parameters->resizable = false;
    parameters->clear_color = COLOR_BLACK;
    parameters->v_sync_on = false;
    parameters->title = "SDL!";
}

bool init_video(video_data_t *video_data, const window_parameters_t *parameters) {
    
    int flags = SDL_WINDOW_OPENGL;
    if (parameters->full_screen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    } 
    
    if (parameters->resizable) {
        flags |= SDL_WINDOW_RESIZABLE;
    }
    
    SDL_Window *window = SDL_CreateWindow(
            parameters->title, 
            SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, 
            (int) parameters->resolution.width, 
            (int) parameters->resolution.height, 
            flags
    );
    
    if (window == null) {
        return -1;
    }

    int renderer_flags = SDL_RENDERER_ACCELERATED;
    
    if (parameters->v_sync_on) {
        renderer_flags |= SDL_RENDERER_PRESENTVSYNC;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, renderer_flags);
    SDL_SetRenderDrawColor(renderer, COLOR_TO_PARAMETERS(parameters->clear_color));
    
    video_data->clear_color = parameters->clear_color;
    video_data->full_screen_on = parameters->full_screen;
    video_data->resolution = parameters->resolution;
    video_data->sdl_window = window;
    video_data->sdl_renderer = renderer;
    video_data->window_resizable = parameters->resizable;
    strcpy(video_data->window_title, parameters->title);
}

void free_video(video_data_t *video_data) {
    SDL_DestroyRenderer(video_data->sdl_renderer);
    SDL_DestroyWindow(video_data->sdl_window);
}

void update_video_data(video_data_t *video_data) {
    
    if (video_data->dirty) {
        SDL_SetWindowTitle(video_data->sdl_window, video_data->window_title);

        if (video_data->full_screen_on) {
            SDL_SetWindowFullscreen(video_data->sdl_window, SDL_WINDOW_FULLSCREEN);
        } else {
            SDL_SetWindowFullscreen(video_data->sdl_window, false);
        }

        SDL_SetWindowResizable(video_data->sdl_window, video_data->window_resizable);
        SDL_SetWindowSize(video_data->sdl_window, video_data->resolution.width, video_data->resolution.height);
        SDL_RenderSetLogicalSize(video_data->sdl_renderer, video_data->resolution.width, video_data->resolution.height);
        
        video_data->dirty = false;
    }    
}

void clear_window(video_data_t *video_data) {
    SDL_SetRenderDrawColor(video_data->sdl_renderer, COLOR_TO_PARAMETERS(video_data->clear_color));
    SDL_RenderClear(video_data->sdl_renderer);
}

void flip_video(video_data_t *video_data) {
    SDL_RenderPresent(video_data->sdl_renderer);
}

void set_video_resolution(video_data_t *video_data, vec2_t resolution) {
    video_data->resolution = resolution;
    video_data->dirty = true;
}
void set_video_full_screen(video_data_t *video_data, bool full_screen) {
    video_data->full_screen_on = full_screen;
    video_data->dirty = true;
}

void set_window_title(video_data_t *video_data, const char *title) {
    strcpy(video_data->window_title, title);
    video_data->dirty = true;
}

void set_video_clear_color(video_data_t *video_data, color_t color) {
    video_data->clear_color = color;
    video_data->dirty = true;
}