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
    
    if (data == null) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't load texture at path: '%s'", file_path);
        SDL_assert(false);
    }

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
    
    SDL_assert(tex_renderer->transform != null);
    
    rect_t tex_region = tex_renderer->sprite.texture_region;
    vec2_t screen_size = mul_vec2(tex_region.size, tex_renderer->transform->scale);
    vec2_t pivot = denormalize_point(screen_size, tex_renderer->normalized_pivot);
    vec2_t world_pos = sub_vec2(tex_renderer->transform->position, pivot);
    rect_t screen_region = get_rect(world_pos, screen_size);
    float angle = tex_renderer->transform->angle - camera->transform.angle;
    
    draw_texture_ex(renderer, screen_region, angle, tex_region, pivot, tex_renderer->sprite.texture);
}

sprite_renderer_t *get_sprite_renderer_empty(graphics_data_t *graphics_data) {
    SDL_assert(graphics_data->renderers_count < MAX_RENDERERS);
    sprite_renderer_t *renderer = &graphics_data->renderers[graphics_data->renderers_count++];
    sprite_t sprite = {};
    renderer->sprite = sprite;
    renderer->transform = null;
    renderer->normalized_pivot = get_normalized_pivot_point(PIVOT_CENTER);
    renderer->depth_inside_layer = 0;
    renderer->layer = FIRST_LAYER;
    return renderer;
}

sprite_renderer_t *get_sprite_renderer(graphics_data_t *graphics_data, texture_t *texture) {
    sprite_renderer_t *renderer = get_sprite_renderer_empty(graphics_data);
    renderer->sprite = create_sprite_ex(texture, get_rect(VEC2_ZERO, texture->size));
    return renderer;
}

void free_sprite_renderer(graphics_data_t *graphics_data, sprite_renderer_t *renderer) {
    SDL_memmove(renderer, &graphics_data->renderers[graphics_data->renderers_count - 1], sizeof(sprite_renderer_t));
    graphics_data->renderers_count--;
}

rect_t calculate_rect_based_on_pivot_and_scale(vec2_t position, vec2_t size, vec2_t scale, vec2_t normalized_pivot) {
    size = mul_vec2(size, scale);
    
    vec2_t pivot = denormalize_point(size, normalized_pivot);
    vec2_t pos = sub_vec2(position, pivot);
    
    rect_t rect = get_rect(pos, size);
    
    return rect;
}

int compare_depth_func(const void *first, const void *second) {
    sprite_renderer_t *first_renderer =  *((sprite_renderer_t **) first); 
    sprite_renderer_t *second_renderer = *((sprite_renderer_t **) second);
    
    int result;    
    if (first_renderer->depth_inside_layer > second_renderer->depth_inside_layer) {
        result = 1;        
    } else {
        result = -1;
    }
    
    return result;
}

void update_graphics_data(graphics_data_t *graphics_data) {
    for (int i = 0; i < MAX_LAYERS; ++i) {
        graphics_data->layers[i].renderer_count = 0;
    }
    
    for (int i = 0; i < graphics_data->renderers_count; ++i) {
        sprite_renderer_t *renderer = &graphics_data->renderers[i];
        SDL_assert(renderer->layer >= 0 && renderer->layer < MAX_LAYERS);
        drawing_layer_t *layer = &graphics_data->layers[renderer->layer];
        layer->renderers[layer->renderer_count++] = renderer;
    }
    
    for (int i = 0; i < MAX_LAYERS; ++i) {
        drawing_layer_t *layer = &graphics_data->layers[i];
        qsort(layer->renderers, (size_t) layer->renderer_count, sizeof(sprite_renderer_t *), compare_depth_func);
    }
}

void draw(SDL_Renderer *renderer, graphics_data_t *graphics_data) {
    for (int i = 0; i < MAX_LAYERS; ++i) {
        drawing_layer_t *layer = &graphics_data->layers[i];
        for (int j = 0; j < layer->renderer_count; ++j) {
            sprite_renderer_t *tex_renderer = layer->renderers[j];
            draw_sprite_renderer(renderer, &graphics_data->camera, tex_renderer);
        }
    }
}

sprite_t create_sprite(texture_t *texture) {
    rect_t region = get_rect(VEC2_ZERO, texture->size);
    return create_sprite_ex(texture, region);
}

sprite_t create_sprite_ex(texture_t *texture, rect_t region) {
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
    destination->playing = true;

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

void stop_animation(sprite_animation_t *animation) {
    animation->playing = false;
    animation->current_time = 0;
}

void reset_animation(sprite_animation_t *animation) {
    animation->current_time = 0;
}

void update_sprite_animation(const time_data_t *time_data, sprite_animation_t *animation) {
    if (animation->playing) {
        animation->current_time += time_data->dt;
    }
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

SDL_Color convert_color(color_t color) {
    SDL_Color sdl_color;
    sdl_color.r = color.red;
    sdl_color.g = color.green;
    sdl_color.b = color.blue;
    sdl_color.a = color.alpha;
    return sdl_color;
}

void debug_draw_circle(SDL_Renderer *renderer, vec2_t center, float radius, color_t color) {
    
    SDL_Color sdl_color = convert_color(color);
    SDL_SetRenderDrawColor(renderer, sdl_color.r, sdl_color.g, sdl_color.b, sdl_color.a);

    #define POINTS_COUNT 360

    SDL_Point points[POINTS_COUNT];

    for (int i = 0; i < POINTS_COUNT; ++i) {
        
        float rad = RADIANS(i);
        float x = center.x + (cosf(rad) * radius);
        float y = center.y + (sinf(rad) * radius);
        
        points[i].x = (int) x;
        points[i].y = (int) y;
    }

    SDL_RenderDrawPoints(renderer, points, POINTS_COUNT);
}

void debug_draw_rect(SDL_Renderer *renderer, rect_t rect, color_t color) {
    SDL_Color sdl_color = convert_color(color);
    SDL_SetRenderDrawColor(renderer, sdl_color.r, sdl_color.g, sdl_color.b, sdl_color.a);
    
    SDL_Rect sdl_rect = convert_rect(rect);
    SDL_RenderDrawRect(renderer, &sdl_rect);
}

void debug_draw_fill_rect(SDL_Renderer *renderer, rect_t rect, color_t color) {
    SDL_Color sdl_color = convert_color(color);
    SDL_SetRenderDrawColor(renderer, sdl_color.r, sdl_color.g, sdl_color.b, sdl_color.a);

    SDL_Rect sdl_rect = convert_rect(rect);
    SDL_RenderFillRect(renderer, &sdl_rect);
}