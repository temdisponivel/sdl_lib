//
// Created by matheus on 15/09/2018.
//

#include <math.h>
#include "gui.h"

#define INDEX_TO_CHAR(index) ((ushort) ((index) + 32))
#define CHAR_TO_INDEX(ch) ((int) ((ch) - 32))

void init_font(SDL_Renderer *renderer, font_t *font, const char *font_path, int font_size_in_points, FONT_STYLE font_style) {
    
    font->font = load_font_from_file(font_path, font_size_in_points, font_style);
    font->line_spacing = TTF_FontLineSkip(font->font);
    font->size_in_points = font_size_in_points;
    
    for (int i = 0; i < DEFAULT_CHARACTER_SET_SIZE; ++i) {

        SDL_Color color = {};
        color.r = 255;
        color.a = 255;
        SDL_Surface *surface = TTF_RenderGlyph_Solid(font->font, INDEX_TO_CHAR(i), color);
        if (surface == null) {
            SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "Couldn't get surface for character '%i'. Error msg: %s",
                         INDEX_TO_CHAR(i), TTF_GetError());
        }
        texture_t texture;
        texture.size = get_vec2(surface->w, surface->h);
        texture.handle = SDL_CreateTextureFromSurface(renderer, surface);;
        font->characters[i] = texture;
    }
}

TTF_Font *load_font_from_file(const char *font_path, int size_in_points, FONT_STYLE font_style) {
    TTF_Font *font = TTF_OpenFont(font_path, size_in_points);
    
    TTF_SetFontHinting(font, font_style);

    if (font == null) {
        SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "Couldn't load font at path: '%s'. Make sure the font exists!",
                     font_path);
    }

    return font;
}

void destroy_font(TTF_Font *font) {
    TTF_CloseFont(font);
}

vec2_t get_text_total_size(const font_t *font, const char *string, vec2_t max_size, int size_in_points) {

    vec2_t scale = get_vec2(size_in_points / font->size_in_points, size_in_points / font->size_in_points);

    max_size = mul_vec2(max_size, scale);
    
    vec2_t current_size = {};
    vec2_t text_size = {};
    size_t len = strlen(string);

    int line_spacing = (int) (font->line_spacing  * scale.y);
    current_size.height = line_spacing;
    
    for (int i = 0; i < len; ++i) {
        char ch = string[i];
        if (ch < 32 || ch > DEFAULT_CHARACTER_SET_SIZE + 32) {
            continue;
        }
        
        int texture_index = CHAR_TO_INDEX(ch);
        texture_t texture = font->characters[texture_index];
        
        vec2_t size = texture.size;
        size = mul_vec2(size, scale);
        
        float desired_x = current_size.x + size.x; 
        float desired_y = current_size.y + size.y;
        
        if (desired_x > max_size.x) {
            text_size.x = max_size.x;
            
            current_size.y += line_spacing;
            current_size.x = 0;
        } else {
            current_size.x = desired_x;
            text_size.x = fmaxf(text_size.x, current_size.x);
        }
        
        if (desired_y > max_size.y)
            current_size.y = max_size.y;
    }
    
    text_size.y = current_size.y;
    return text_size;
}

void get_string_draw_actions(
        const font_t *font, 
        vec2_t current_total_size, 
        char current_char, 
        vec2_t max_size,
        int size_in_points,
        bool *break_line, 
        bool *stop_drawing, 
        bool *skip_current_character
) {
    if (current_char == '\n') {
        *break_line = true;
        *skip_current_character = true;
        *stop_drawing = false;
    } else {
        vec2_t scale = get_vec2(size_in_points / font->size_in_points, size_in_points / font->size_in_points);
        
        max_size = mul_vec2(max_size, scale);
        
        int texture_index = CHAR_TO_INDEX(current_char);
        texture_t current_char_tex = font->characters[texture_index];
        
        vec2_t tex_size = current_char_tex.size;
        tex_size = mul_vec2(tex_size, scale);

        bool can_draw_on_next_line = current_total_size.height + tex_size.height < max_size.height;

        if (current_total_size.width + tex_size.width > max_size.width) {
            *break_line = can_draw_on_next_line;
            *stop_drawing = !can_draw_on_next_line;
            *skip_current_character = false;
        } else {
            *break_line = false;
            *stop_drawing = false;
            *skip_current_character = false;
        }
    }
}

void draw_gui_string_ex(
        SDL_Renderer *renderer,
        const font_t *font,
        vec2_t screen_pos,
        vec2_t max_size,
        const char *string,
        color_t color,
        float angle,
        int size_in_points,
        PIVOT pivot
) {
    vec2_t scale = get_vec2(size_in_points / font->size_in_points, size_in_points / font->size_in_points);

    vec2_t size = get_text_total_size(font, string, max_size, size_in_points);
    //size = min_vec2(size, max_size);

    rect_t full_text_rect = get_rect(VEC2_ZERO, size);

    vec2_t normalized_pivot = get_normalized_pivot_point(pivot);
    vec2_t full_text_pivot = denormalize_rect_point(full_text_rect, normalized_pivot);

    vec2_t original_draw_pos = sub_vec2(screen_pos, full_text_pivot);
    
    vec2_t draw_pos = original_draw_pos;
    vec2_t current_text_size = VEC2_ZERO;

    int line_skip_height = (int) (font->line_spacing * scale.y);
    current_text_size.height = line_skip_height;

    size_t len = strlen(string);
    for (int i = 0; i < len; ++i) {
        char ch = string[i];

        bool skip_current_char;
        bool stop_drawing;
        bool break_line;

        get_string_draw_actions(font, current_text_size, ch, max_size, size_in_points, &break_line, &stop_drawing, &skip_current_char);

        if (stop_drawing) {
            break;
        } else {
            if (break_line) {
                draw_pos.x = original_draw_pos.x;
                draw_pos.y += line_skip_height;

                current_text_size.height += line_skip_height;
                current_text_size.width = 0;
            }

            if (skip_current_char) {
                continue;
            }
        }

        int texture_index = CHAR_TO_INDEX(ch);
        texture_t texture = font->characters[texture_index];

        rect_t full_texture_rect = get_rect(VEC2_ZERO, texture.size);

        rect_t screen_rect = get_rect(draw_pos, texture.size);
        screen_rect.size = mul_vec2(screen_rect.size, scale);

        current_text_size.width += screen_rect.size.width;

        draw_texture_ex(renderer, screen_rect, angle, full_texture_rect, VEC2_ZERO, &texture);

        draw_pos.x += screen_rect.size.width;
    }
}

void draw_world_string(
        SDL_Renderer *renderer,
        const font_t *font,
        const camera_t *camera,
        const transform_t *transform,
        vec2_t max_size,
        const char *string,
        float angle,
        color_t color
) {
    vec2_t screen_pos = sub_vec2(camera->transform.world_pos, transform->world_pos);
    
}