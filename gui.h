//
// Created by matheus on 15/09/2018.
//

#ifndef SDL_GAME_TEXT_H
#define SDL_GAME_TEXT_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "maths.h"
#include "graphics.h"
#include "input.h"

#define DEFAULT_FONT_SIZE 14
#define DEFAULT_FONT_PATH "data/consola.ttf"

#define DEFAULT_CHARACTER_SET_SIZE 128

#define DEFAULT_PIVOT PIVOT_CENTER_LEFT

#define RESIZE_TO_FIT get_vec2(0xFFFFFF, 0xFFFFFF)
#define RESIZE_HEIGHT(max_width) get_vec2((max_width), 0xFFFFFF)

#define MAX_LABEL_STRING_LEN 256

typedef enum font_style {
    NORMAL = TTF_STYLE_NORMAL,
    ITALIC = TTF_STYLE_ITALIC,
    BOLD = TTF_STYLE_BOLD,
    STROKE = TTF_STYLE_STRIKETHROUGH,
    UNDERLINE = TTF_STYLE_UNDERLINE,    
} FONT_STYLE;

typedef enum {
    LABEL_RESIZE_TO_FIT,
    LABEL_RESIZE_HEIGHT,
    LABEL_FIXED_SIZE
} LABEL_RESIZE_MODE;

typedef struct font {
    TTF_Font *font;
    texture_t characters[DEFAULT_CHARACTER_SET_SIZE];
    int line_spacing;
    int size_in_points;
} font_t;

typedef struct label {
    font_t *font;
    char text[MAX_LABEL_STRING_LEN];
    color_t color;
    PIVOT pivot;
    int text_size_in_points;

    LABEL_RESIZE_MODE resize_mode;
    
    union {
        float max_width;
        vec2_t max_size;
    };
} label_t;

typedef struct button {
    label_t label;
    vec2_t size;
    
    PIVOT pivot;
    
    sprite_t sprite;
    color_t normal_color;
    color_t hover_color;
    color_t clicked_color;
} button_t;

void init_font_from_file(SDL_Renderer *renderer, font_t *font, const char *font_path, int font_size_in_points, FONT_STYLE font_style);

TTF_Font *load_font_from_file(const char *font_path, int size_in_points, FONT_STYLE font_style);
void destroy_font(TTF_Font *font);

vec2_t get_text_total_size(const font_t *font, const char *string, vec2_t max_size, int size_in_points);

vec2_t get_text_total_size_ex(const font_t *font, const char *string, vec2_t scaled_max_size, vec2_t scale);

void draw_gui_string(
        SDL_Renderer *renderer,
        const font_t *font,
        vec2_t screen_pos,
        const char *string,
        color_t color
);

void draw_gui_string_ex(
        SDL_Renderer *renderer, 
        const font_t *font, 
        vec2_t screen_pos, 
        vec2_t max_size, 
        const char *string, 
        color_t color,
        int size_in_points,
        PIVOT pivot
);

void draw_world_string(
        SDL_Renderer *renderer,
        const font_t *font,
        const camera_t *camera,
        vec2_t world_pos,
        const char *string,
        color_t color
);

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
);

void setup_label(label_t *label, font_t *font, const char *text, color_t color);

void setup_label_ex(
        label_t *label,
        font_t *font, 
        const char *text, 
        color_t color, 
        PIVOT pivot, 
        int text_size_in_points,
        LABEL_RESIZE_MODE resize_mode
);

vec2_t get_label_pos_inside_rect(rect_t rect, PIVOT label_pivot);

void set_label_text(label_t *label, const char *text);

void draw_label(SDL_Renderer *renderer, vec2_t position, const label_t *label);
        
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
);


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
);
        
bool draw_click_area_color_ex(
        SDL_Renderer *renderer,
        input_data_t *input_data,

        vec2_t position,
        vec2_t size,
        PIVOT button_pivot,

        color_t normal_color,
        color_t hover_color,
        color_t click_color,

        font_t *font,
        const char *text,
        int text_size_in_points,
        color_t normal_text_color,
        color_t clicked_text_color,
        PIVOT text_pivot
);

void setup_button(
        button_t *button,
        label_t label, 
        vec2_t size,
        sprite_t sprite, 
        color_t normal_color, 
        color_t hover_color, 
        color_t click_color,
        PIVOT pivot
);

void draw_button(
        SDL_Renderer *renderer,
        input_data_t *input_data,
        vec2_t position,
        const button_t *button
);


bool draw_click_area_colored_sprites_ex_test(
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
        PIVOT text_pivot,
        vec2_t normalized_text_position
);

#endif //SDL_GAME_TEXT_H