//
// Created by matheus on 16/09/2018.
//

#include "engine.h"

engine_data_t *init_engine(const window_parameters_t *window_parameters) {
    engine_data_t *engine_data = calloc(1, sizeof(engine_data_t));
    
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    if (result) {
        const char *error = SDL_GetError();
        SDL_Log(error);
        return null;
    }

    init_time_data(&engine_data->time_data);
    init_video(&engine_data->video_data, window_parameters);
    init_gui();

    bool audio_ok = init_audio(&engine_data->audio_data);
    if (!audio_ok) {
        return null;
    }
    
    return engine_data;
}

void free_engine(engine_data_t *engine_data) {
    //Audio will be automatically freed when SDL_Quit is called
    // free_audio();
    
    free_video(&engine_data->video_data);
    
    SDL_Quit();
}

void engine_start_update(engine_data_t *engine_data) {
    start_frame(&engine_data->time_data);
}

void engine_update_internal_systems(engine_data_t *engine_data) {
    update_input_data(&engine_data->input_data);
    update_physics_data(&engine_data->physics_data);
    update_audio_data(&engine_data->audio_data, &engine_data->time_data);
    update_graphics_data(&engine_data->graphics_data);
    update_video_data(&engine_data->video_data);
}

void engine_start_draw(engine_data_t *engine_data) {
    clear_window(&engine_data->video_data);
}

void engine_draw_internal_systems_and_flip_video(engine_data_t *engine_data) {

    // TODO: Move this into a engine_draw_debug_[something] functions
    draw_physics_debug(engine_data->video_data.sdl_renderer, &engine_data->physics_data);
    
    draw(engine_data->video_data.sdl_renderer, &engine_data->graphics_data);
    flip_video(&engine_data->video_data);
}

void engine_end_update(engine_data_t *engine_data) {
    end_frame(&engine_data->time_data);
}

bool engine_should_quit(engine_data_t *engine_data) {
    return engine_data->input_data.quit_event_called;
}