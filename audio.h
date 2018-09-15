//
// Created by matheus on 15/09/2018.
//

#ifndef SDL_GAME_AUDIO_H
#define SDL_GAME_AUDIO_H

#include "SDL2/SDL_mixer.h"
#include "maths.h"
#include "time.h"

#define MAX_AUDIO_SOURCES 128
#define MAX_SOUND_SOURCE_DISTANCE 2000

typedef enum {
    EFFECT,
    MUSIC
} SOUND_TYPE;

typedef struct sound {
    SOUND_TYPE type;
    union {
        Mix_Chunk *chunk;
        Mix_Music *music;
    };
} sound_t;

typedef struct audio_source {
    vec2_t position;

    float volume;
    
    interpolation_t volume_interpolation;
    
    int _channel;
    
    bool playing;
    bool loop;
    
    bool do_panning;

    sound_t sound;
} audio_source_t;

typedef struct audio_listener {
    vec2_t position;
    float volume;
    float max_audio_source_distance;
} audio_listener_t;

typedef struct audio_data {
    audio_source_t sources[MAX_AUDIO_SOURCES];
    int audio_source_count;

    audio_listener_t listener;
} audio_data_t;

bool init_audio(audio_data_t *audio_data);

sound_t load_sound_from_file(const char *file_path, SOUND_TYPE type);
void destroy_sound(sound_t *sound);

void free_audio_source(audio_data_t *audio_data, audio_source_t *source);
audio_source_t *get_audio_source(audio_data_t *audio_data, sound_t sound);

void pause_audio_source(audio_source_t *source);
void resume_audio_source(audio_source_t *source);

void stop_audio_source(audio_source_t *source);
void play_audio_source(audio_source_t *source);

void reset_audio_source(audio_source_t *source);

void fade_out_audio_source(audio_source_t *source, float duration);
void fade_in_audio_source(audio_source_t *source, float duration);
void fade_audio_source(audio_source_t *source, float duration, float target_volume);

void set_volume(audio_source_t *source, float volume);

void update_audio_data(audio_data_t *audio_data, time_data_t *time_data);

#endif //SDL_GAME_AUDIO_H
