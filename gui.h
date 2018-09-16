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

#define RESIZE_TO_FIT get_vec2(0xFFFFFF, 0xFFFFFF)
#define RESIZE_HEIGHT(max_width) get_vec2((max_width), 0xFFFFFF)
#define GET_HEIGHT_FOR_LINES(gui_data, max_lines) ((gui_data).line_spacing * (max_lines))
#define RESIZE_HEIGHT_MAX_LINES(gui_data, max_width, max_lines) get_vec2((max_width), GET_HEIGHT_FOR_LINES((gui_data), (max_lines))) 

typedef struct gui_data {
    TTF_Font *font;
    texture_t characters[DEFAULT_CHARACTER_SET_SIZE];
    int line_spacing;
} gui_data_t;

void init_gui_data(SDL_Renderer *renderer, gui_data_t *gui_data, const char *font_path, int font_size_in_points);

TTF_Font *load_font_from_file(const char *font_path, int size_in_points);
void destroy_font(TTF_Font *font);

void draw_gui_string(SDL_Renderer *renderer, gui_data_t *gui_data, vec2_t screen_pos, vec2_t max_size, const char *string, color_t color, PIVOT pivot);
void draw_world_string(SDL_Renderer *renderer, gui_data_t *gui_data, const camera_t *camera, const transform_t *transform, vec2_t max_size, const char *string, color_t color);

#endif //SDL_GAME_TEXT_H
