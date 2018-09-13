//
// Created by matheus on 12/09/2018.
//

#include "engine.h"
#include "graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void load_texture_from_file(const char* file_path, SDL_Renderer *renderer, texture_t *destination) {
    #define GET_CHANNELS_FROM_IMAGE 0

    int width, height;
    int channels;
    stbi_uc *data = stbi_load(file_path, &width, &height, &channels, GET_CHANNELS_FROM_IMAGE);

    uint format;
    if (channels == 3) {
        format = SDL_PIXELFORMAT_RGB24;
    } else {
        format = SDL_PIXELFORMAT_RGBA32;
    }

    int pitch = width * channels;

    SDL_Texture *texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STATIC, width, height);
    
    bool fail = SDL_UpdateTexture(texture, null, data, pitch);
    SDL_assert(!fail);
    
    // Enable blending for texture with alpha channels
    if (channels > 3)
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    stbi_image_free(data);

    destination->handle = texture;
    destination->size = get_vec2(width, height);
}

void destroy_texture(texture_t *texture) {
    SDL_DestroyTexture(texture->handle);
    texture->handle = 0xBAAD;
}

void draw_texture(SDL_Renderer *renderer, const texture_t *texture, ivec2_t position) {
    SDL_Rect rect = get_rect(position, texture->size);
    SDL_RenderCopy(renderer, texture->handle, null, &rect);
}