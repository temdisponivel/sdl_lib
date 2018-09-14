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
    //if (channels > 3)
    //  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    stbi_image_free(data);

    destination->handle = texture;
    destination->size = get_vec2(width, height);
}

void destroy_texture(texture_t *texture) {
    SDL_DestroyTexture(texture->handle);
    texture->handle = 0xBAAD;
}

void draw_texture(SDL_Renderer *renderer, const texture_t *texture, vec2_t world_position) {
    SDL_Rect rect = get_rect(world_position, texture->size);
    SDL_RenderCopy(renderer, texture->handle, null, &rect);
}

vec2_t get_pivot_point(vec2_t region, TEXTURE_PIVOT pivot) {
    float x = 0;
    float y = 0;
    
    if ((pivot & PIVOT_CENTER) != 0) {
        x = region.x / 2.f;
        y = region.y / 2.f;
    }

    if ((pivot & PIVOT_BOTTOM) != 0) {
        y = region.height;
    }

    if ((pivot & PIVOT_TOP) != 0) {
        y = 0;
    }
    
    if ((pivot & PIVOT_LEFT) != 0) {
        x = 0;
    }
    
    if ((pivot & PIVOT_RIGHT) != 0) {
        x = region.width;
    }
    
    return get_vec2(x, y);
}

texture_renderer_t *create_texture_renderer(graphics_data_t *graphics_data, texture_t *texture) {
    SDL_assert(graphics_data->active_renderers < MAX_RENDERERS);
    
    texture_renderer_t *renderer = &graphics_data->renderers[graphics_data->active_renderers++];
    renderer->texture = texture;
    renderer->world_position = get_vec2(0, 0);
    renderer->texture_region = texture->size;
    renderer->pivot = get_pivot_point(renderer->texture_region, PIVOT_CENTER);
    return renderer;
}

void draw(SDL_Renderer *renderer, graphics_data_t *graphics_data) {
    for (int i = 0; i < graphics_data->active_renderers; ++i) {
        texture_renderer_t tex_renderer = graphics_data->renderers[i];
        
        vec2_t draw_pos = sum_vec2(tex_renderer.world_position, graphics_data->camera.world_position);

        draw_pos = sub_vec2(draw_pos, tex_renderer.pivot);
        draw_texture(renderer, tex_renderer.texture, draw_pos);
    }
}