//
// Created by matheus on 12/09/2018.
//

#ifndef SDL_GAME_INPUT_H
#define SDL_GAME_INPUT_H

#include "defines.h"
#include "maths.h"

typedef enum {
    STATE_NORMAL,
    STATE_PRESSED,
    STATE_HELD,
    STATE_RELEASED,
} STATE;

typedef enum {
    KEY_RETURN,
    KEY_ESCAPE,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_SPACE,
    KEY_EXCLAIM,
    KEY_QUOTEDBL,
    KEY_HASH,
    KEY_PERCENT,
    KEY_DOLLAR,
    KEY_AMPERSAND,
    KEY_QUOTE,
    KEY_LEFTPAREN,
    KEY_RIGHTPAREN,
    KEY_ASTERISK,
    KEY_PLUS,
    KEY_COMMA,
    KEY_MINUS,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_COLON,
    KEY_SEMICOLON,
    KEY_LESS,
    KEY_EQUALS,
    KEY_GREATER,
    KEY_QUESTION,
    KEY_AT,
    KEY_LEFTBRACKET,
    KEY_BACKSLASH,
    KEY_RIGHTBRACKET,
    KEY_CARET,
    KEY_UNDERSCORE,
    KEY_BACKQUOTE,
    KEY_a,
    KEY_b,
    KEY_c,
    KEY_d,
    KEY_e,
    KEY_f,
    KEY_g,
    KEY_h,
    KEY_i,
    KEY_j,
    KEY_k,
    KEY_l,
    KEY_m,
    KEY_n,
    KEY_o,
    KEY_p,
    KEY_q,
    KEY_r,
    KEY_s,
    KEY_t,
    KEY_u,
    KEY_v,
    KEY_w,
    KEY_x,
    KEY_y,
    KEY_z,
    KEY_CAPSLOCK,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_PRINTSCREEN,
    KEY_SCROLLLOCK,
    KEY_PAUSE,
    KEY_INSERT,
    KEY_HOME,
    KEY_PAGEUP,
    KEY_DELETE,
    KEY_END,
    KEY_PAGEDOWN,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_DOWN,
    KEY_UP,
    KEY_NUMLOCKCLEAR,
    KEY_KP_DIVIDE,
    KEY_KP_MULTIPLY,
    KEY_KP_MINUS,
    KEY_KP_PLUS,
    KEY_KP_ENTER,
    KEY_KP_1,
    KEY_KP_2,
    KEY_KP_3,
    KEY_KP_4,
    KEY_KP_5,
    KEY_KP_6,
    KEY_KP_7,
    KEY_KP_8,
    KEY_KP_9,
    KEY_KP_0,
    KEY_KP_PERIOD,
    KEY_APPLICATION,
    KEY_POWER,
    KEY_KP_EQUALS,
    KEY_F13,
    KEY_F14,
    KEY_F15,
    KEY_F16,
    KEY_F17,
    KEY_F18,
    KEY_F19,
    KEY_F20,
    KEY_F21,
    KEY_F22,
    KEY_F23,
    KEY_F24,
    KEY_EXECUTE,
    KEY_HELP,
    KEY_MENU,
    KEY_SELECT,
    KEY_STOP,
    KEY_AGAIN,
    KEY_UNDO,
    KEY_CUT,
    KEY_COPY,
    KEY_PASTE,
    KEY_FIND,
    KEY_MUTE,
    KEY_VOLUMEUP,
    KEY_VOLUMEDOWN,
    KEY_KP_COMMA,
    KEY_KP_EQUALSAS400,
    KEY_ALTERASE,
    KEY_SYSREQ,
    KEY_CANCEL,
    KEY_CLEAR,
    KEY_PRIOR,
    KEY_RETURN2,
    KEY_SEPARATOR,
    KEY_OUT,
    KEY_OPER,
    KEY_CLEARAGAIN,
    KEY_CRSEL,
    KEY_EXSEL,
    KEY_KP_00,
    KEY_KP_000,
    KEY_THOUSANDSSEPARATOR,
    KEY_DECIMALSEPARATOR,
    KEY_CURRENCYUNIT,
    KEY_CURRENCYSUBUNIT,
    KEY_KP_LEFTPAREN,
    KEY_KP_RIGHTPAREN,
    KEY_KP_LEFTBRACE,
    KEY_KP_RIGHTBRACE,
    KEY_KP_TAB,
    KEY_KP_BACKSPACE,
    KEY_KP_A,
    KEY_KP_B,
    KEY_KP_C,
    KEY_KP_D,
    KEY_KP_E,
    KEY_KP_F,
    KEY_KP_XOR,
    KEY_KP_POWER,
    KEY_KP_PERCENT,
    KEY_KP_LESS,
    KEY_KP_GREATER,
    KEY_KP_AMPERSAND,
    KEY_KP_DBLAMPERSAND,
    KEY_KP_VERTICALBAR,
    KEY_KP_DBLVERTICALBAR,
    KEY_KP_COLON,
    KEY_KP_HASH,
    KEY_KP_SPACE,
    KEY_KP_AT,
    KEY_KP_EXCLAM,
    KEY_KP_MEMSTORE,
    KEY_KP_MEMRECALL,
    KEY_KP_MEMCLEAR,
    KEY_KP_MEMADD,
    KEY_KP_MEMSUBTRACT,
    KEY_KP_MEMMULTIPLY,
    KEY_KP_MEMDIVIDE,
    KEY_KP_PLUSMINUS,
    KEY_KP_CLEAR,
    KEY_KP_CLEARENTRY,
    KEY_KP_BINARY,
    KEY_KP_OCTAL,
    KEY_KP_DECIMAL,
    KEY_KP_HEXADECIMAL,
    KEY_LCTRL,
    KEY_LSHIFT,
    KEY_LALT,
    KEY_LGUI,
    KEY_RCTRL,
    KEY_RSHIFT,
    KEY_RALT,
    KEY_RGUI,
    KEY_MODE,
    KEY_AUDIONEXT,
    KEY_AUDIOPREV,
    KEY_AUDIOSTOP,
    KEY_AUDIOPLAY,
    KEY_AUDIOMUTE,
    KEY_MEDIASELECT,
    KEY_WWW,
    KEY_MAIL,
    KEY_CALCULATOR,
    KEY_COMPUTER,
    KEY_AC_SEARCH,
    KEY_AC_HOME,
    KEY_AC_BACK,
    KEY_AC_FORWARD,
    KEY_AC_STOP,
    KEY_AC_REFRESH,
    KEY_AC_BOOKMARKS,
    KEY_BRIGHTNESSDOWN,
    KEY_BRIGHTNESSUP,
    KEY_DISPLAYSWITCH,
    KEY_KBDILLUMTOGGLE,
    KEY_KBDILLUMDOWN,
    KEY_KBDILLUMUP,
    KEY_EJECT,
    KEY_SLEEP,
    KEY_APP1,
    KEY_APP2,
    KEY_AUDIOREWIND,
    KEY_AUDIOFASTFORWARD,
    KEY_LAST
} KEY;

typedef enum {
    MODIFIER_NONE = KMOD_NONE,
    MODIFIER_NUM = KMOD_NUM,
    MODIFIER_CAPS = KMOD_CAPS,
    MODIFIER_LCTRL = KMOD_LCTRL,
    MODIFIER_RCTRL = KMOD_RCTRL,
    MODIFIER_RSHIFT = KMOD_RSHIFT,
    MODIFIER_LSHIFT = KMOD_LSHIFT,
    MODIFIER_RALT = KMOD_RALT,
    MODIFIER_LALT = KMOD_LALT,
    MODIFIER_CTRL = KMOD_CTRL,
    MODIFIER_SHIFT = KMOD_SHIFT,
    MODIFIER_ALT = KMOD_ALT,
} MODIFIERS;

typedef enum {
    BUTTON_LEFT,
    BUTTON_CENTER,
    BUTTON_RIGHT,
    BUTTON_LAST
} BUTTON;

typedef struct mouse_button_state {
    int click_count;
    STATE state;
} mouse_button_state_t;

typedef struct input_data {
    // KEYBOARD
    STATE last_frame_keyboard[KEY_LAST];
    STATE current_frame_keyboard[KEY_LAST];
    
    MODIFIERS current_modifiers;

    // MISC
    bool quit_event_called;

    // WINDOW
    bool window_resized;
    vec2_t window_new_size;
    
    // MOUSE    
    vec2_t mouse_pos;
    vec2_t mouse_delta;
    float mouse_scroll_delta;
    mouse_button_state_t last_frame_buttons[BUTTON_LAST];
    mouse_button_state_t current_frame_buttons[BUTTON_LAST];
} input_data_t;

KEY get_key_index(SDL_Keycode key);

void update_input_data(input_data_t *input_data);

STATE get_key_state(input_data_t *input_data, KEY key);
mouse_button_state_t get_button_state(input_data_t *input_data, BUTTON button);
int get_click_count(input_data_t *input_data, BUTTON button);

bool is_key_normal(input_data_t *input_data, KEY key);
bool is_key_pressed(input_data_t *input_data, KEY key);
bool is_key_held(input_data_t *input_data, KEY key);
bool is_key_released(input_data_t *input_data, KEY key);

bool is_button_normal(input_data_t *input_data, BUTTON button);
bool is_button_pressed(input_data_t *input_data, BUTTON button);
bool is_button_held(input_data_t *input_data, BUTTON button);
bool is_button_released(input_data_t *input_data, BUTTON button);

bool is_modifier_on(input_data_t *input_data, MODIFIERS modifier);

#endif //SDL_GAME_INPUT_H
