//
// Created by matheus on 15/09/2018.
//

#include <math.h>
#include "audio.h"
#include "time.h"

#define MAX_PANNING 255

bool init_audio(audio_data_t *audio_data) {
    bool result = false;
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) {
        result = false;
    } else {
        int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
        int init = Mix_Init(flags);
        if ((init & flags) == 0) {
            const char *error = Mix_GetError();
            SDL_Log(error);
            result = false;
        } else {
            result = true;
        }
    }

    if (result) {
        Mix_AllocateChannels(MAX_AUDIO_SOURCES);
    }

    audio_data->listener.volume = 1;
    audio_data->listener.max_audio_source_distance = MAX_SOUND_SOURCE_DISTANCE;

    return result;
}

audio_source_t *get_base_audio_source(audio_data_t *audio_data) {
    SDL_assert(audio_data->audio_source_count < MAX_AUDIO_SOURCES);

    audio_source_t *source = &audio_data->sources[audio_data->audio_source_count++];
    source->position = VEC2_ZERO;
    source->volume = 1;
    source->volume_interpolation = blank_interpolation();
    source->_channel = -1;
    source->loop = false;
    source->playing = false;
    source->do_panning = true;
    return source;
}

void free_audio_source(audio_data_t *audio_data, audio_source_t *source) {
    
    memset(source, 0, sizeof(audio_source_t));
    SDL_memmove(source, &audio_data->sources[audio_data->audio_source_count - 1], sizeof(audio_source_t));

    audio_data->audio_source_count--;
}

sound_t load_sound_from_file(const char *file_path, SOUND_TYPE type) {
    sound_t sound;
    sound.type = type;
    
    if (type == MUSIC) {
        Mix_Music *music = Mix_LoadMUS(file_path);
        
        if (music == null) {
            SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "Couldn't load music file at path: '%s'. Make sure the file exists and it's of supported format: OGG, MP3 or WAV.");
        }
        
        sound.music = music;
    } else {
        Mix_Chunk *chunk = Mix_LoadWAV(file_path);
        
        if (chunk == null) {
            SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "Couldn't load sound effect file at path: '%s'. Make sure the file exists and it's of supported format: OGG, MP3 or WAV.");
        }
        
        sound.chunk = chunk;
    }
    
    return sound;
}

void destroy_sound(sound_t *sound) {
    if (sound->type == MUSIC) {
        SDL_assert(sound->music);
        Mix_FreeMusic(sound->music);
    } else {
        SDL_assert(sound->chunk);
        Mix_FreeChunk(sound->chunk);
    }
}

audio_source_t *get_audio_source(audio_data_t *audio_data, sound_t sound) {
    audio_source_t *source = get_base_audio_source(audio_data);
    source->sound = sound;
    return source;
}

void pause_audio_source(audio_source_t *source) {
    source->playing = false;

    if (source->sound.type == MUSIC) {
        Mix_PauseMusic();
    } else if (source->_channel != -1) {
        Mix_Pause(source->_channel);
    }
}

void resume_audio_source(audio_source_t *source) {
    if (source->sound.type == MUSIC) {
        Mix_ResumeMusic();
    } else {
        if (source->_channel == -1) {
            play_audio_source(source);
        } else {
            Mix_Resume(source->_channel);
        }
    }

    source->playing = true;
}

void stop_audio_source(audio_source_t *source) {
    source->playing = false;
    if (source->sound.type == MUSIC) {
        Mix_PauseMusic();
        Mix_RewindMusic();
    } else {
        Mix_Pause(source->_channel);
    }
}

void play_audio_source(audio_source_t *source) {
    int loop = 0;
    if (source->loop)
        loop = -1;

    if (source->sound.type == MUSIC) {
        
        Mix_PlayMusic(source->sound.music, loop);
        
        if (Mix_PausedMusic()) {
            Mix_ResumeMusic();
        }
        
    } else {
        source->_channel = Mix_PlayChannel(source->_channel, source->sound.chunk, loop);
    }

    source->playing = true;
}

void reset_audio_source(audio_source_t *source) {
    stop_audio_source(source);
    play_audio_source(source);
}

void fade_out_audio_source(audio_source_t *source, float duration) {
    fade_audio_source(source, duration, 0);
}

void fade_in_audio_source(audio_source_t *source, float duration) {
    fade_audio_source(source, duration, 1);
}

void fade_audio_source(audio_source_t *source, float duration, float target_volume) {
    source->volume_interpolation = blank_interpolation();
    source->volume_interpolation.target = target_volume;
    source->volume_interpolation.start = source->volume;
    source->volume_interpolation.time = 0;
    source->volume_interpolation.duration = duration;
}

void set_volume(audio_source_t *source, float volume) {
    source->volume_interpolation = blank_interpolation();
    source->volume = volume;
}

void update_audio_data(audio_data_t *audio_data, time_data_t *time_data) {
    audio_listener_t *listener = &audio_data->listener;

    for (int i = 0; i < audio_data->audio_source_count; ++i) {
        audio_source_t *source = &audio_data->sources[i];

        if (source->volume_interpolation.duration > 0) {
            source->volume_interpolation.time += time_data->dt;
            source->volume = get_current_interpolation_value(&source->volume_interpolation);
        }

        float distance = sqrd_distance_vec2(listener->position, source->position);
        float distance_normalized = distance / SQUARE(listener->max_audio_source_distance /
                                                      3.f); // / 3 was found by experimentation, it's good enough
        float target_distance_volume = 1 / SQUARE(distance_normalized); // inversed square distance law

        float left_panning = 0;
        float right_panning = 0;

        if (source->position.x < listener->position.x) {

            right_panning = LERP(0, 1, target_distance_volume);
            left_panning = LERP(1, 0, target_distance_volume);

        } else if (source->position.x > listener->position.x) {

            right_panning = LERP(1, 0, target_distance_volume);
            left_panning = LERP(0, 1, target_distance_volume);

        } else {
            left_panning = 1;
            right_panning = 1;
        }

        int volume = (int) floorf(source->volume * audio_data->listener.volume * MIX_MAX_VOLUME);

        if (source->sound.type == MUSIC) {

            Mix_VolumeMusic(volume);
            source->playing = Mix_PausedMusic() <= 0 && Mix_PlayingMusic() > 0;

        } else if (source->_channel != -1) {
            volume *= target_distance_volume;
            Mix_Volume(source->_channel, volume);

            if (source->do_panning) {
                Mix_SetPanning(source->_channel, (ubyte) (left_panning * MAX_PANNING), (ubyte) (right_panning * MAX_PANNING));
            }

            source->playing = Mix_Paused(source->_channel) <= 0 && Mix_Playing(source->_channel) > 0;
        }
    }
}