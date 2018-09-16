//
// Created by matheus on 15/09/2018.
//

#include "gui.h"

#define INDEX_TO_CHAR(index) ((ushort) ((index) + 32))
#define CHAR_TO_INDEX(ch) ((int) ((ch) - 32))

void init_gui_data(SDL_Renderer *renderer, gui_data_t *gui_data, const char *font_path, int font_size_in_points) {
    gui_data->font = load_font_from_file(font_path, font_size_in_points);
    gui_data->line_spacing = TTF_FontLineSkip(gui_data->font);
    for (int i = 0; i < DEFAULT_CHARACTER_SET_SIZE; ++i) {

        SDL_Color color = {};
        color.r = 255;
        color.a = 255;
        SDL_Surface *surface = TTF_RenderGlyph_Solid(gui_data->font, INDEX_TO_CHAR(i), color);
        if (surface == null) {
            SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "Couldn't get surface for character '%i'. Error msg: %s",
                         INDEX_TO_CHAR(i), TTF_GetError());
        }
        texture_t texture;
        texture.size = get_vec2(surface->w, surface->h);
        texture.handle = SDL_CreateTextureFromSurface(renderer, surface);;
        gui_data->characters[i] = texture;
    }
}

TTF_Font *load_font_from_file(const char *font_path, int size_in_points) {
    TTF_Font *font = TTF_OpenFont(font_path, size_in_points);

    if (font == null) {
        SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "Couldn't load font at path: '%s'. Make sure the font exists!",
                     font_path);
    }

    return font;
}

void destroy_font(TTF_Font *font) {
    TTF_CloseFont(font);
}

vec2_t get_text_total_size(gui_data_t *gui_data, const char *string, vec2_t max_size) {
    vec2_t current_size = {};
    vec2_t text_size = {};
    size_t len = strlen(string);

    current_size.height = gui_data->line_spacing;
    for (int i = 0; i < len; ++i) {
        char ch = string[i];
        if (ch < 32 || ch > DEFAULT_CHARACTER_SET_SIZE + 32) {
            continue;
        }
        
        int texture_index = CHAR_TO_INDEX(ch);
        texture_t texture = gui_data->characters[texture_index];
        
        float desired_x = current_size.x + texture.size.x; 
        if (desired_x > max_size.x) {
            text_size.x = max_size.x;
            
            current_size.y += gui_data->line_spacing;
            current_size.x = 0;
        } else {
            current_size.x = desired_x;
            text_size.x = current_size.x;
        }
    }
    
    text_size.y = current_size.y;
    return text_size;
}

void get_string_draw_actions(
        gui_data_t *gui_data, 
        vec2_t current_total_size, 
        char current_char, 
        vec2_t max_size, 
        bool *break_line, 
        bool *stop_drawing, 
        bool *skip_current_character
) {
    
    if (current_char == '\n') {
        *break_line = true;
        *skip_current_character = true;
        *stop_drawing = false;
    } else {
        int texture_index = CHAR_TO_INDEX(current_char);
        texture_t current_char_tex = gui_data->characters[texture_index];

        bool can_draw_on_next_line = current_total_size.height + current_char_tex.size.height < max_size.height;

        if (current_total_size.width + current_char_tex.size.width > max_size.width) {
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

void draw_gui_string(
        SDL_Renderer *renderer, 
        gui_data_t *gui_data, 
        vec2_t screen_pos, 
        vec2_t max_size, 
        const char *string, 
        color_t color, 
        PIVOT pivot
) {

    vec2_t original_draw_pos;

    vec2_t size = get_text_total_size(gui_data, string, max_size);
    size = min_vec2(size, max_size);

    rect_t full_text_rect = get_rect(VEC2_ZERO, size);

    vec2_t normalized_pivot = get_normalized_pivot_point(pivot);
    vec2_t full_text_pivot = denormalize_rect_point(full_text_rect, normalized_pivot);

    original_draw_pos = sub_vec2(screen_pos, full_text_pivot);

    vec2_t draw_pos = original_draw_pos;
    vec2_t current_text_size = VEC2_ZERO;

    int line_skip_height = gui_data->line_spacing;
    current_text_size.height = line_skip_height;

    size_t len = strlen(string);
    for (int i = 0; i < len; ++i) {
        char ch = string[i];

        bool skip_current_char;
        bool stop_drawing;
        bool break_line;

        get_string_draw_actions(gui_data, current_text_size, ch, max_size, &break_line, &stop_drawing, &skip_current_char);

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
        texture_t texture = gui_data->characters[texture_index];

        rect_t full_texture_rect = get_rect(VEC2_ZERO, texture.size);

        rect_t screen_rect = get_rect(draw_pos, texture.size);

        current_text_size.width += screen_rect.size.width;

        draw_texture_ex(renderer, screen_rect, 0, full_texture_rect, VEC2_ZERO, &texture);

        draw_pos.x += texture.size.width;
    }
}

/*
void draw_world_string(gui_data_t *gui_data, const camera_t *camera, const transform_t *transform, vec2_t max_size, const char *string, color_t color) {
    
}*/
