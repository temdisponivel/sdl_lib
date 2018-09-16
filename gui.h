//
// Created by matheus on 15/09/2018.
//

#ifndef SDL_GAME_TEXT_H
#define SDL_GAME_TEXT_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "maths.h"
#include "graphics.h"
#define DEFAULT_FONT_SIZE 14
#define DEFAULT_FONT_PATH "data/consola.ttf"

#define DEFAULT_CHARACTER_SET_SIZE 128

#define DEFAULT_PIVOT PIVOT_CENTER_LEFT

#define RESIZE_TO_FIT get_vec2(0xFFFFFF, 0xFFFFFF)
#define RESIZE_HEIGHT(max_width) get_vec2((max_width), 0xFFFFFF)
#define GET_HEIGHT_FOR_LINES(gui_data, max_lines) ((gui_data).line_spacing * (max_lines))
#define RESIZE_HEIGHT_MAX_LINES(gui_data, max_width, max_lines) get_vec2((max_width), GET_HEIGHT_FOR_LINES((gui_data), (max_lines)))

typedef enum font_style {
    NORMAL = TTF_STYLE_NORMAL,
    ITALIC = TTF_STYLE_ITALIC,
    BOLD = TTF_STYLE_BOLD,
    STROKE = TTF_STYLE_STRIKETHROUGH,
    UNDERLINE = TTF_STYLE_UNDERLINE,    
} FONT_STYLE;

typedef struct font {
    TTF_Font *font;
    texture_t characters[DEFAULT_CHARACTER_SET_SIZE];
    int line_spacing;
    int size_in_points;
} font_t;

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

#endif //SDL_GAME_TEXT_H
