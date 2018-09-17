//
// Created by matheus on 15/09/2018.
//

#include <math.h>
#include <stdio.h>
#include "gui.h"
#include "input.h"

#define INDEX_TO_CHAR(index) ((ushort) ((index) + 32))
#define CHAR_TO_INDEX(ch) ((int) ((ch) - 32))

#define FORMAT_BUFFER_LEN 256
#define STR_FORMAT_BUFFER char buffer[FORMAT_BUFFER_LEN]

void string_format(char *buffer, const char *fmt, ...) {
    va_list va;
    va_start (va, fmt);
    vsprintf (buffer, fmt, va);
    va_end (va);
}

bool init_gui() {
    bool init_error = TTF_Init();
    if (init_error) {
        const char *error = TTF_GetError();
        SDL_Log(error);
    }
    
    return !init_error;
}

void init_font_from_file(SDL_Renderer *renderer, font_t *font, const char *font_path, int font_size_in_points, FONT_STYLE font_style) {
    
    font->font = load_font_from_file(font_path, font_size_in_points, font_style);
    font->line_spacing = TTF_FontLineSkip(font->font);
    font->size_in_points = font_size_in_points;
    
    for (int i = 0; i < DEFAULT_CHARACTER_SET_SIZE; ++i) {

        SDL_Color color = convert_color(COLOR_WHITE);
        SDL_Surface *surface = TTF_RenderGlyph_Solid(font->font, INDEX_TO_CHAR(i), color);
        
        if (surface == null) {
            SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "Couldn't get surface for character '%i'. Error msg: %s", INDEX_TO_CHAR(i), TTF_GetError());
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
        SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "Couldn't load font at path: '%s'. Make sure the font exists!", font_path);
    }

    return font;
}

void destroy_font(TTF_Font *font) {
    TTF_CloseFont(font);
}

vec2_t get_text_total_size(const font_t *font, const char *string, vec2_t max_size, int size_in_points) {
    vec2_t scale = get_vec2(size_in_points / font->size_in_points, size_in_points / font->size_in_points);
    return get_text_total_size_ex(font, string, max_size, scale);
}

vec2_t get_text_total_size_ex(const font_t *font, const char *string, vec2_t scaled_max_size, vec2_t scale) {

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
        
        if (desired_x > scaled_max_size.x) {
            text_size.x = scaled_max_size.x;
            
            current_size.y += line_spacing;
            current_size.x = 0;
        } else {
            current_size.x = desired_x;
            text_size.x = fmaxf(text_size.x, current_size.x);
        }
        
        if (desired_y > scaled_max_size.y)
            current_size.y = scaled_max_size.y;
    }
    
    text_size.y = current_size.y;
    return text_size;
}

void get_string_draw_actions(
        vec2_t current_total_size, 
        char current_char,
        vec2_t current_char_tex_size_scaled,
        vec2_t scaled_max_size,
        bool *break_line, 
        bool *stop_drawing, 
        bool *skip_current_character
) {
    if (current_char == '\n') {
        *break_line = true;
        *skip_current_character = true;
        *stop_drawing = false;
    } else {        
        vec2_t tex_size = current_char_tex_size_scaled;

        bool can_draw_on_next_line = current_total_size.height + tex_size.height < scaled_max_size.height;

        if (current_total_size.width + tex_size.width > scaled_max_size.width) {
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
        const font_t *font,
        vec2_t screen_pos,
        const char *string,
        color_t color
) {
    draw_gui_string_ex(renderer, font, screen_pos, RESIZE_TO_FIT, string, color, font->size_in_points, DEFAULT_PIVOT);
}

void draw_gui_string_ex(
        SDL_Renderer *renderer,
        const font_t *font,
        vec2_t screen_pos,
        vec2_t max_size,
        const char *string,
        color_t color,
        int size_in_points,
        PIVOT pivot
) {
    vec2_t scale = get_vec2(size_in_points / font->size_in_points, size_in_points / font->size_in_points);
    vec2_t scaled_max_size = mul_vec2(max_size, scale);

    vec2_t size = get_text_total_size_ex(font, string, scaled_max_size, scale);

    vec2_t normalized_pivot = get_normalized_pivot_point(pivot);
    vec2_t full_text_pivot = denormalize_point(size, normalized_pivot);

    vec2_t original_draw_pos = sub_vec2(screen_pos, full_text_pivot);
    
    vec2_t draw_pos = original_draw_pos;
    vec2_t current_text_size = VEC2_ZERO;

    int line_skip_height = (int) (font->line_spacing * scale.y);
    current_text_size.height = line_skip_height;
    
    size_t len = strlen(string);
    for (int i = 0; i < len; ++i) {
        char ch = string[i];

        int texture_index = CHAR_TO_INDEX(ch);
        texture_t texture = font->characters[texture_index];
        vec2_t scaled_tex_size = mul_vec2(texture.size, scale);

        bool skip_current_char;
        bool stop_drawing;
        bool break_line;

        get_string_draw_actions(current_text_size, ch, scaled_tex_size, scaled_max_size, &break_line, &stop_drawing, &skip_current_char);

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

        rect_t full_texture_rect = get_rect(VEC2_ZERO, scaled_tex_size);
        rect_t screen_rect = get_rect(draw_pos, scaled_tex_size);

        SDL_SetTextureColorMod(texture.handle, color.red, color.green, color.blue);
        draw_texture_ex(renderer, screen_rect, 0, full_texture_rect, VEC2_ZERO, &texture);

        current_text_size.width += screen_rect.size.width;
        draw_pos.x += screen_rect.size.width;
    }
}

void draw_world_string(
        SDL_Renderer *renderer,
        const font_t *font,
        const camera_t *camera,
        vec2_t world_pos,
        const char *string,
        color_t color
) {
    draw_world_string_ex(renderer, font, camera, world_pos, RESIZE_TO_FIT, string, color, font->size_in_points, DEFAULT_PIVOT);
}

void draw_world_string_ex(
        SDL_Renderer *renderer,
        const font_t *font,
        const camera_t *camera,
        vec2_t world_pos,
        vec2_t max_size,
        const char *string,
        color_t color,
        int size_in_points,
        PIVOT pivot
) {
    vec2_t screen_pos = sub_vec2(world_pos, camera->transform.position);
    draw_gui_string_ex(renderer, font, screen_pos, max_size, string, color, size_in_points, pivot);
}

void setup_label(label_t *label, font_t *font, const char *text, color_t color) {
    setup_label_ex(label, font, text, color, DEFAULT_PIVOT, font->size_in_points, LABEL_RESIZE_TO_FIT);
}

void setup_label_ex(label_t *label, font_t *font, const char *text, color_t color, PIVOT pivot, int text_size_in_points, LABEL_RESIZE_MODE resize_mode) {
    label->font = font;
    set_label_text(label, text);
    label->color = color;
    label->pivot = pivot;
    label->text_size_in_points = text_size_in_points;
    label->resize_mode = resize_mode;
}

vec2_t get_label_drawing_size(const label_t *label) {
    vec2_t max_size = get_label_max_size(label);
    vec2_t size = get_text_total_size(label->font, label->text, max_size, label->text_size_in_points);
    return size;
}

vec2_t get_label_pos_inside_rect(rect_t rect, PIVOT label_pivot) {
    vec2_t pivot = get_normalized_pivot_point(label_pivot);
    vec2_t pos = denormalize_point_inside_rect(rect, pivot);
    return pos;
}

void set_label_text(label_t *label, const char *text) {
    size_t len = strlen(text);
    SDL_assert(len < MAX_LABEL_STRING_LEN);
    strcpy(label->text, text);
}

vec2_t get_label_max_size(const label_t *label) {
    vec2_t max_size;
    switch (label->resize_mode) {
        case LABEL_RESIZE_TO_FIT:
            max_size = RESIZE_TO_FIT;
            break;
        case LABEL_RESIZE_HEIGHT:
            max_size = RESIZE_HEIGHT(label->max_width);
            break;
        case LABEL_FIXED_SIZE:
            max_size = label->max_size;
            break;
    }
    return max_size;
}

void draw_label(SDL_Renderer *renderer, vec2_t position, const label_t *label) {
    vec2_t max_size = get_label_max_size(label);    
    draw_gui_string_ex(renderer, label->font, position, max_size, label->text, label->color, label->text_size_in_points, label->pivot);
}

bool setup_base_click_area(input_data_t *input_data, vec2_t position, vec2_t size, PIVOT pivot, bool *clicked, bool *hovered, bool *released, rect_t *button_rect) {
    vec2_t pivot_point = get_normalized_pivot_point(pivot);
    vec2_t denormalized_pivot = denormalize_point(size, pivot_point);

    position = sub_vec2(position, denormalized_pivot);

    *button_rect = get_rect(position, size);
    
    vec2_t mouse_pos = input_data->mouse_pos;

    *hovered = false;
    *clicked = false;
    *released = false;
    if (is_button_held(input_data, BUTTON_LEFT)) {
        *clicked = is_point_inside_rect(*button_rect, mouse_pos);
    } else if (is_button_released(input_data, BUTTON_LEFT)) {
        *released = is_point_inside_rect(*button_rect, mouse_pos);
    } else {
        *hovered = is_point_inside_rect(*button_rect, mouse_pos);
    }

    if (*released)
        *clicked = true;
}

bool draw_click_area_sprites_ex(
        SDL_Renderer *renderer,

        input_data_t *input_data,

        vec2_t position,
        vec2_t size,
        PIVOT pivot,

        sprite_t *normal_sprite,
        sprite_t *hover_sprite,
        sprite_t *clicked_sprite,

        font_t *font,
        const char *text,
        int text_size_in_points,
        color_t normal_text_color,
        color_t clicked_text_color,
        PIVOT text_pivot
) {
    bool hovered = false;
    bool clicked = false;
    bool released = false;
    
    rect_t button_rect;
    setup_base_click_area(input_data, position, size, pivot, &clicked, &hovered, &released, &button_rect);
        
    color_t text_color;
    sprite_t *sprite;
    if (hovered) {
        sprite = hover_sprite;
        text_color = normal_text_color;
    } else if (clicked) {
        sprite = clicked_sprite;
        text_color = clicked_text_color;
    } else {
        sprite = normal_sprite;
        text_color = normal_text_color;
    }
    
    draw_texture_ex(renderer, button_rect, 0, sprite->texture_region, VEC2_ZERO, sprite->texture);

    label_t label;
    setup_label_ex(&label, font, text, text_color, text_pivot, text_size_in_points, LABEL_FIXED_SIZE);
    label.max_size = size;

    vec2_t label_pos = get_label_pos_inside_rect(button_rect, text_pivot);
    draw_label(renderer, label_pos, &label);
    
    return released;
}

bool draw_click_area_colored_sprites_ex(
        SDL_Renderer *renderer,
        input_data_t *input_data,

        vec2_t position,
        vec2_t size,
        PIVOT pivot,

        sprite_t *sprite,

        color_t normal_color,
        color_t hover_color,
        color_t click_color,

        font_t *font,
        const char *text,
        int text_size_in_points,
        color_t normal_text_color,
        color_t clicked_text_color,
        PIVOT text_pivot
) {
    bool hovered = false;
    bool clicked = false;
    bool released = false;

    rect_t button_rect;
    setup_base_click_area(input_data, position, size, pivot, &clicked, &hovered, &released, &button_rect);

    color_t text_color;
    color_t sprite_color;
    if (hovered) {
        sprite_color = hover_color;
        text_color = normal_text_color;
    } else if (clicked) {
        sprite_color = click_color;
        text_color = clicked_text_color;
    } else {
        sprite_color = normal_color;
        text_color = normal_text_color;
    }
    
    SDL_SetTextureColorMod(sprite->texture->handle, sprite_color.red, sprite_color.green, sprite_color.blue);

    draw_texture_ex(renderer, button_rect, 0, sprite->texture_region, VEC2_ZERO, sprite->texture);

    label_t label;
    setup_label_ex(&label, font, text, text_color, text_pivot, text_size_in_points, LABEL_FIXED_SIZE);
    label.max_size = size;

    vec2_t label_pos = get_label_pos_inside_rect(button_rect, text_pivot);
    draw_label(renderer, label_pos, &label);

    return released;
}

bool draw_click_area_color_ex(
        SDL_Renderer *renderer,
        input_data_t *input_data,

        vec2_t position,
        vec2_t size,
        PIVOT pivot,

        color_t normal_color,
        color_t hover_color,
        color_t click_color,

        font_t *font,
        const char *text,
        int text_size_in_points,
        color_t normal_text_color,
        color_t clicked_text_color,
        PIVOT text_pivot
) {
    bool hovered = false;
    bool clicked = false;
    bool released = false;

    rect_t button_rect;
    setup_base_click_area(input_data, position, size, pivot, &clicked, &hovered, &released, &button_rect);

    color_t text_color;
    color_t rect_color;
    if (hovered) {
        rect_color = hover_color;
        text_color = normal_text_color;
    } else if (clicked) {
        rect_color = click_color;
        text_color = clicked_text_color;
    } else {
        rect_color = normal_color;
        text_color = normal_text_color;
    }

    debug_draw_fill_rect(renderer, button_rect, rect_color);

    label_t label;
    setup_label_ex(&label, font, text, text_color, text_pivot, text_size_in_points, LABEL_FIXED_SIZE);
    label.max_size = size;

    vec2_t label_pos = get_label_pos_inside_rect(button_rect, text_pivot);
    draw_label(renderer, label_pos, &label);

    return released;
}

void setup_button(button_t *button, label_t label, vec2_t size, sprite_t sprite, color_t normal_color, color_t hover_color, color_t click_color, PIVOT pivot) {
    button->label = label;
    button->size = size;
    button->sprite = sprite;
    button->normal_color = normal_color;
    button->hover_color = hover_color;
    button->clicked_color = click_color;
    button->pivot = pivot;
}

bool draw_button(
        SDL_Renderer *renderer,
        input_data_t *input_data,
        vec2_t position,
        const button_t *button
) {
    return draw_click_area_colored_sprites_ex(
            renderer,
            input_data,
            position,
            button->size,
            button->pivot,
            &button->sprite,
            button->normal_color,
            button->hover_color,
            button->clicked_color,
            button->label.font,
            button->label.text,
            button->label.text_size_in_points,
            button->label.color,
            button->label.color,
            button->label.pivot
    );
}

