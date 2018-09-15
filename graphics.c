//
// Created by matheus on 12/09/2018.
//

#include "engine.h"
#include "graphics.h"
#include "math.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

void load_texture_from_file(const char *file_path, SDL_Renderer *renderer, texture_t *destination) {
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


void draw_sprite_renderer(SDL_Renderer *renderer, const camera_t *camera, const sprite_renderer_t *tex_renderer) {
    rect_t screen_pos = calculate_rect(
            tex_renderer->transform.world_pos,
            tex_renderer->sprite.texture_region.size,
            tex_renderer->transform.scale,
            tex_renderer->normalized_pivot
    );

    screen_pos.position = sub_vec2(screen_pos.position, camera->transform.world_pos);
    
    float angle = tex_renderer->transform.angle;
    angle = angle - camera->transform.angle;

    rect_t tex_region = tex_renderer->sprite.texture_region;
    vec2_t screen_size = mul_vec2(tex_renderer->sprite.texture_region.size, tex_renderer->transform.scale);

    vec2_t pivot = denormalize_rect_point(get_rect(VEC2_ZERO, screen_size), tex_renderer->normalized_pivot);

    draw_texture_ex(renderer, screen_pos, angle, tex_region, pivot, tex_renderer->sprite.texture);
}

sprite_renderer_t *get_sprite_renderer(graphics_data_t *graphics_data, texture_t *texture) {
    SDL_assert(graphics_data->renderers_count < MAX_RENDERERS);

    sprite_renderer_t *renderer = &graphics_data->renderers[graphics_data->renderers_count++];
    renderer->sprite = create_sprite(texture, get_rect(VEC2_ZERO, texture->size));
    renderer->transform = IDENTITY_TRANS;
    renderer->normalized_pivot = get_normalized_pivot_point(PIVOT_CENTER);
    return renderer;
}

void destroy_sprite_renderer(graphics_data_t *graphics_data, sprite_renderer_t *renderer) {
    SDL_memmove(renderer, &graphics_data->renderers[graphics_data->renderers_count - 1], sizeof(sprite_renderer_t));
    graphics_data->renderers_count--;
}

void draw(SDL_Renderer *renderer, graphics_data_t *graphics_data) {
    for (int i = 0; i < graphics_data->renderers_count; ++i) {
        sprite_renderer_t *tex_renderer = &graphics_data->renderers[i];
        draw_sprite_renderer(renderer, &graphics_data->camera, tex_renderer);
    }
}

sprite_t create_sprite(texture_t *texture, rect_t region) {
    sprite_t sprite;
    sprite.texture = texture;
    sprite.texture_region = region;
    return sprite;
}


rect_t get_texture_frame(texture_t *texture, vec2_t frame_size, int frame_index) {

    int texture_columns = (int) (texture->size.width / frame_size.width);
    int texture_rows = (int) (texture->size.height / frame_size.height);

    int column = frame_index % texture_columns;
    int row = frame_index / texture_columns;

    row = (int) fminf(row, texture_rows);

    vec2_t pos = get_vec2(column, row);
    pos = mul_vec2(pos, frame_size);
    return get_rect(pos, frame_size);
}

void create_sprite_animation_from_sheet(
        texture_t *sprite_sheet,
        vec2_t sprite_size,
        int start_frame,
        int frame_count,
        int cycle_duration,
        bool loop,
        sprite_animation_t *destination
) {
    destination->cycle_duration = cycle_duration;
    destination->loop = loop;
    destination->frame_count = frame_count;
    destination->current_time = 0;

    for (int i = 0; i < frame_count; ++i) {
        destination->sprites[i].texture = sprite_sheet;
        destination->sprites[i].texture_region = get_texture_frame(sprite_sheet, sprite_size, start_frame + i);
    }
}

int get_sprite_animation_frame_index(const sprite_animation_t *animation) {
    float frame_duration = (animation->cycle_duration / animation->frame_count);
    int frame_index = (int) (animation->current_time / frame_duration);

    if (animation->loop)
        frame_index = frame_index % animation->frame_count;
    else if (frame_index >= animation->frame_count)
        frame_index = animation->frame_count - 1;

    return frame_index;
}

void update_sprite_animation(const time_data_t *time_data, sprite_animation_t *animation) {
    animation->current_time += time_data->dt;
}

void set_sprite_on_renderer(sprite_renderer_t *renderer, const sprite_animation_t *animation) {
    int frame_index = get_sprite_animation_frame_index(animation);
    SDL_assert(frame_index < animation->frame_count);
    sprite_t sprite = animation->sprites[frame_index];
    renderer->sprite = sprite;
}

color_t get_color(byte red, byte green, byte blue, byte alpha) {
    color_t color;
    color.red = red;
    color.green = green;
    color.blue = blue;
    color.alpha = alpha;
    return color;
}