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

void draw_texture_ex(
        SDL_Renderer *renderer, 
        rect_t screen_region, 
        float angle, 
        rect_t texture_region,
        vec2_t pivot,
        const texture_t *texture
) {
    SDL_Rect source_rect = convert_rect(texture_region);
    SDL_Rect sdl_dest_rect = convert_rect(screen_region);
    
    SDL_Point sdl_pivot = convert_vec2(pivot);
    
    SDL_RenderCopyEx(renderer, texture->handle, &source_rect, &sdl_dest_rect, angle, &sdl_pivot, SDL_FLIP_NONE);
}

void draw_texture_renderer(SDL_Renderer *renderer, const camera_t *camera, const texture_renderer_t* tex_renderer) {
    vec2_t world_pos;
    float angle;
    rect_t tex_region;
    vec2_t pivot;
    
    world_pos = tex_renderer->transform.world_pos;
    angle = tex_renderer->transform.angle;
    
    world_pos = sub_vec2(world_pos, camera->transform.world_pos);
    angle = angle - camera->transform.angle;
    
    tex_region = tex_renderer->texture_region;

    vec2_t screen_size = mul_vec2(tex_renderer->texture_region.size, tex_renderer->transform.scale);
    
    pivot = denormalize_rect_point(get_rect(VEC2_ZERO, screen_size), tex_renderer->normalized_pivot);
    world_pos = sub_vec2(world_pos, pivot);
    
    rect_t screen_region = get_rect(world_pos, screen_size);
    
    draw_texture_ex(renderer, screen_region, angle, tex_region, pivot, tex_renderer->texture);    
}

texture_renderer_t *create_texture_renderer(graphics_data_t *graphics_data, texture_t *texture) {
    SDL_assert(graphics_data->active_renderers < MAX_RENDERERS);
    
    texture_renderer_t *renderer = &graphics_data->renderers[graphics_data->active_renderers++];
    renderer->texture = texture;
    renderer->transform = IDENTITY_TRANS;
    renderer->texture_region = get_rect(VEC2_ZERO, texture->size);
    renderer->normalized_pivot = get_normalized_pivot_point(PIVOT_CENTER);
    return renderer;
}

void draw(SDL_Renderer *renderer, graphics_data_t *graphics_data) {
    for (int i = 0; i < graphics_data->active_renderers; ++i) {
        texture_renderer_t *tex_renderer = &graphics_data->renderers[i];
        draw_texture_renderer(renderer, &graphics_data->camera, tex_renderer);
    }
}