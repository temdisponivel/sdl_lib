//
// Created by matheus on 12/09/2018.
//

#include "input.h"

KEY get_key_index(SDL_Keycode key) {
    KEY index = KEY_RETURN;

    switch (key) {
        case SDLK_RETURN:
            index = KEY_RETURN;
            break;
        case SDLK_ESCAPE:
            index = KEY_ESCAPE;
            break;
        case SDLK_BACKSPACE:
            index = KEY_BACKSPACE;
            break;
        case SDLK_TAB:
            index = KEY_TAB;
            break;
        case SDLK_SPACE:
            index = KEY_SPACE;
            break;
        case SDLK_EXCLAIM:
            index = KEY_EXCLAIM;
            break;
        case SDLK_QUOTEDBL:
            index = KEY_QUOTEDBL;
            break;
        case SDLK_HASH:
            index = KEY_HASH;
            break;
        case SDLK_PERCENT:
            index = KEY_PERCENT;
            break;
        case SDLK_DOLLAR:
            index = KEY_DOLLAR;
            break;
        case SDLK_AMPERSAND:
            index = KEY_AMPERSAND;
            break;
        case SDLK_QUOTE:
            index = KEY_QUOTE;
            break;
        case SDLK_LEFTPAREN:
            index = KEY_LEFTPAREN;
            break;
        case SDLK_RIGHTPAREN:
            index = KEY_RIGHTPAREN;
            break;
        case SDLK_ASTERISK:
            index = KEY_ASTERISK;
            break;
        case SDLK_PLUS:
            index = KEY_PLUS;
            break;
        case SDLK_COMMA:
            index = KEY_COMMA;
            break;
        case SDLK_MINUS:
            index = KEY_MINUS;
            break;
        case SDLK_PERIOD:
            index = KEY_PERIOD;
            break;
        case SDLK_SLASH:
            index = KEY_SLASH;
            break;
        case SDLK_0:
            index = KEY_0;
            break;
        case SDLK_1:
            index = KEY_1;
            break;
        case SDLK_2:
            index = KEY_2;
            break;
        case SDLK_3:
            index = KEY_3;
            break;
        case SDLK_4:
            index = KEY_4;
            break;
        case SDLK_5:
            index = KEY_5;
            break;
        case SDLK_6:
            index = KEY_6;
            break;
        case SDLK_7:
            index = KEY_7;
            break;
        case SDLK_8:
            index = KEY_8;
            break;
        case SDLK_9:
            index = KEY_9;
            break;
        case SDLK_COLON:
            index = KEY_COLON;
            break;
        case SDLK_SEMICOLON:
            index = KEY_SEMICOLON;
            break;
        case SDLK_LESS:
            index = KEY_LESS;
            break;
        case SDLK_EQUALS:
            index = KEY_EQUALS;
            break;
        case SDLK_GREATER:
            index = KEY_GREATER;
            break;
        case SDLK_QUESTION:
            index = KEY_QUESTION;
            break;
        case SDLK_AT:
            index = KEY_AT;
            break;
        case SDLK_LEFTBRACKET:
            index = KEY_LEFTBRACKET;
            break;
        case SDLK_BACKSLASH:
            index = KEY_BACKSLASH;
            break;
        case SDLK_RIGHTBRACKET:
            index = KEY_RIGHTBRACKET;
            break;
        case SDLK_CARET:
            index = KEY_CARET;
            break;
        case SDLK_UNDERSCORE:
            index = KEY_UNDERSCORE;
            break;
        case SDLK_BACKQUOTE:
            index = KEY_BACKQUOTE;
            break;
        case SDLK_a:
            index = KEY_a;
            break;
        case SDLK_b:
            index = KEY_b;
            break;
        case SDLK_c:
            index = KEY_c;
            break;
        case SDLK_d:
            index = KEY_d;
            break;
        case SDLK_e:
            index = KEY_e;
            break;
        case SDLK_f:
            index = KEY_f;
            break;
        case SDLK_g:
            index = KEY_g;
            break;
        case SDLK_h:
            index = KEY_h;
            break;
        case SDLK_i:
            index = KEY_i;
            break;
        case SDLK_j:
            index = KEY_j;
            break;
        case SDLK_k:
            index = KEY_k;
            break;
        case SDLK_l:
            index = KEY_l;
            break;
        case SDLK_m:
            index = KEY_m;
            break;
        case SDLK_n:
            index = KEY_n;
            break;
        case SDLK_o:
            index = KEY_o;
            break;
        case SDLK_p:
            index = KEY_p;
            break;
        case SDLK_q:
            index = KEY_q;
            break;
        case SDLK_r:
            index = KEY_r;
            break;
        case SDLK_s:
            index = KEY_s;
            break;
        case SDLK_t:
            index = KEY_t;
            break;
        case SDLK_u:
            index = KEY_u;
            break;
        case SDLK_v:
            index = KEY_v;
            break;
        case SDLK_w:
            index = KEY_w;
            break;
        case SDLK_x:
            index = KEY_x;
            break;
        case SDLK_y:
            index = KEY_y;
            break;
        case SDLK_z:
            index = KEY_z;
            break;
        case SDLK_CAPSLOCK:
            index = KEY_CAPSLOCK;
            break;
        case SDLK_F1:
            index = KEY_F1;
            break;
        case SDLK_F2:
            index = KEY_F2;
            break;
        case SDLK_F3:
            index = KEY_F3;
            break;
        case SDLK_F4:
            index = KEY_F4;
            break;
        case SDLK_F5:
            index = KEY_F5;
            break;
        case SDLK_F6:
            index = KEY_F6;
            break;
        case SDLK_F7:
            index = KEY_F7;
            break;
        case SDLK_F8:
            index = KEY_F8;
            break;
        case SDLK_F9:
            index = KEY_F9;
            break;
        case SDLK_F10:
            index = KEY_F10;
            break;
        case SDLK_F11:
            index = KEY_F11;
            break;
        case SDLK_F12:
            index = KEY_F12;
            break;
        case SDLK_PRINTSCREEN:
            index = KEY_PRINTSCREEN;
            break;
        case SDLK_SCROLLLOCK:
            index = KEY_SCROLLLOCK;
            break;
        case SDLK_PAUSE:
            index = KEY_PAUSE;
            break;
        case SDLK_INSERT:
            index = KEY_INSERT;
            break;
        case SDLK_HOME:
            index = KEY_HOME;
            break;
        case SDLK_PAGEUP:
            index = KEY_PAGEUP;
            break;
        case SDLK_DELETE:
            index = KEY_DELETE;
            break;
        case SDLK_END:
            index = KEY_END;
            break;
        case SDLK_PAGEDOWN:
            index = KEY_PAGEDOWN;
            break;
        case SDLK_RIGHT:
            index = KEY_RIGHT;
            break;
        case SDLK_LEFT:
            index = KEY_LEFT;
            break;
        case SDLK_DOWN:
            index = KEY_DOWN;
            break;
        case SDLK_UP:
            index = KEY_UP;
            break;
        case SDLK_NUMLOCKCLEAR:
            index = KEY_NUMLOCKCLEAR;
            break;
        case SDLK_KP_DIVIDE:
            index = KEY_KP_DIVIDE;
            break;
        case SDLK_KP_MULTIPLY:
            index = KEY_KP_MULTIPLY;
            break;
        case SDLK_KP_MINUS:
            index = KEY_KP_MINUS;
            break;
        case SDLK_KP_PLUS:
            index = KEY_KP_PLUS;
            break;
        case SDLK_KP_ENTER:
            index = KEY_KP_ENTER;
            break;
        case SDLK_KP_1:
            index = KEY_KP_1;
            break;
        case SDLK_KP_2:
            index = KEY_KP_2;
            break;
        case SDLK_KP_3:
            index = KEY_KP_3;
            break;
        case SDLK_KP_4:
            index = KEY_KP_4;
            break;
        case SDLK_KP_5:
            index = KEY_KP_5;
            break;
        case SDLK_KP_6:
            index = KEY_KP_6;
            break;
        case SDLK_KP_7:
            index = KEY_KP_7;
            break;
        case SDLK_KP_8:
            index = KEY_KP_8;
            break;
        case SDLK_KP_9:
            index = KEY_KP_9;
            break;
        case SDLK_KP_0:
            index = KEY_KP_0;
            break;
        case SDLK_KP_PERIOD:
            index = KEY_KP_PERIOD;
            break;
        case SDLK_APPLICATION:
            index = KEY_APPLICATION;
            break;
        case SDLK_POWER:
            index = KEY_POWER;
            break;
        case SDLK_KP_EQUALS:
            index = KEY_KP_EQUALS;
            break;
        case SDLK_F13:
            index = KEY_F13;
            break;
        case SDLK_F14:
            index = KEY_F14;
            break;
        case SDLK_F15:
            index = KEY_F15;
            break;
        case SDLK_F16:
            index = KEY_F16;
            break;
        case SDLK_F17:
            index = KEY_F17;
            break;
        case SDLK_F18:
            index = KEY_F18;
            break;
        case SDLK_F19:
            index = KEY_F19;
            break;
        case SDLK_F20:
            index = KEY_F20;
            break;
        case SDLK_F21:
            index = KEY_F21;
            break;
        case SDLK_F22:
            index = KEY_F22;
            break;
        case SDLK_F23:
            index = KEY_F23;
            break;
        case SDLK_F24:
            index = KEY_F24;
            break;
        case SDLK_EXECUTE:
            index = KEY_EXECUTE;
            break;
        case SDLK_HELP:
            index = KEY_HELP;
            break;
        case SDLK_MENU:
            index = KEY_MENU;
            break;
        case SDLK_SELECT:
            index = KEY_SELECT;
            break;
        case SDLK_STOP:
            index = KEY_STOP;
            break;
        case SDLK_AGAIN:
            index = KEY_AGAIN;
            break;
        case SDLK_UNDO:
            index = KEY_UNDO;
            break;
        case SDLK_CUT:
            index = KEY_CUT;
            break;
        case SDLK_COPY:
            index = KEY_COPY;
            break;
        case SDLK_PASTE:
            index = KEY_PASTE;
            break;
        case SDLK_FIND:
            index = KEY_FIND;
            break;
        case SDLK_MUTE:
            index = KEY_MUTE;
            break;
        case SDLK_VOLUMEUP:
            index = KEY_VOLUMEUP;
            break;
        case SDLK_VOLUMEDOWN:
            index = KEY_VOLUMEDOWN;
            break;
        case SDLK_KP_COMMA:
            index = KEY_KP_COMMA;
            break;
        case SDLK_KP_EQUALSAS400:
            index = KEY_KP_EQUALSAS400;
            break;
        case SDLK_ALTERASE:
            index = KEY_ALTERASE;
            break;
        case SDLK_SYSREQ:
            index = KEY_SYSREQ;
            break;
        case SDLK_CANCEL:
            index = KEY_CANCEL;
            break;
        case SDLK_CLEAR:
            index = KEY_CLEAR;
            break;
        case SDLK_PRIOR:
            index = KEY_PRIOR;
            break;
        case SDLK_RETURN2:
            index = KEY_RETURN2;
            break;
        case SDLK_SEPARATOR:
            index = KEY_SEPARATOR;
            break;
        case SDLK_OUT:
            index = KEY_OUT;
            break;
        case SDLK_OPER:
            index = KEY_OPER;
            break;
        case SDLK_CLEARAGAIN:
            index = KEY_CLEARAGAIN;
            break;
        case SDLK_CRSEL:
            index = KEY_CRSEL;
            break;
        case SDLK_EXSEL:
            index = KEY_EXSEL;
            break;
        case SDLK_KP_00:
            index = KEY_KP_00;
            break;
        case SDLK_KP_000:
            index = KEY_KP_000;
            break;
        case SDLK_THOUSANDSSEPARATOR:
            index = KEY_THOUSANDSSEPARATOR;
            break;
        case SDLK_DECIMALSEPARATOR:
            index = KEY_DECIMALSEPARATOR;
            break;
        case SDLK_CURRENCYUNIT:
            index = KEY_CURRENCYUNIT;
            break;
        case SDLK_CURRENCYSUBUNIT:
            index = KEY_CURRENCYSUBUNIT;
            break;
        case SDLK_KP_LEFTPAREN:
            index = KEY_KP_LEFTPAREN;
            break;
        case SDLK_KP_RIGHTPAREN:
            index = KEY_KP_RIGHTPAREN;
            break;
        case SDLK_KP_LEFTBRACE:
            index = KEY_KP_LEFTBRACE;
            break;
        case SDLK_KP_RIGHTBRACE:
            index = KEY_KP_RIGHTBRACE;
            break;
        case SDLK_KP_TAB:
            index = KEY_KP_TAB;
            break;
        case SDLK_KP_BACKSPACE:
            index = KEY_KP_BACKSPACE;
            break;
        case SDLK_KP_A:
            index = KEY_KP_A;
            break;
        case SDLK_KP_B:
            index = KEY_KP_B;
            break;
        case SDLK_KP_C:
            index = KEY_KP_C;
            break;
        case SDLK_KP_D:
            index = KEY_KP_D;
            break;
        case SDLK_KP_E:
            index = KEY_KP_E;
            break;
        case SDLK_KP_F:
            index = KEY_KP_F;
            break;
        case SDLK_KP_XOR:
            index = KEY_KP_XOR;
            break;
        case SDLK_KP_POWER:
            index = KEY_KP_POWER;
            break;
        case SDLK_KP_PERCENT:
            index = KEY_KP_PERCENT;
            break;
        case SDLK_KP_LESS:
            index = KEY_KP_LESS;
            break;
        case SDLK_KP_GREATER:
            index = KEY_KP_GREATER;
            break;
        case SDLK_KP_AMPERSAND:
            index = KEY_KP_AMPERSAND;
            break;
        case SDLK_KP_DBLAMPERSAND:
            index = KEY_KP_DBLAMPERSAND;
            break;
        case SDLK_KP_VERTICALBAR:
            index = KEY_KP_VERTICALBAR;
            break;
        case SDLK_KP_DBLVERTICALBAR:
            index = KEY_KP_DBLVERTICALBAR;
            break;
        case SDLK_KP_COLON:
            index = KEY_KP_COLON;
            break;
        case SDLK_KP_HASH:
            index = KEY_KP_HASH;
            break;
        case SDLK_KP_SPACE:
            index = KEY_KP_SPACE;
            break;
        case SDLK_KP_AT:
            index = KEY_KP_AT;
            break;
        case SDLK_KP_EXCLAM:
            index = KEY_KP_EXCLAM;
            break;
        case SDLK_KP_MEMSTORE:
            index = KEY_KP_MEMSTORE;
            break;
        case SDLK_KP_MEMRECALL:
            index = KEY_KP_MEMRECALL;
            break;
        case SDLK_KP_MEMCLEAR:
            index = KEY_KP_MEMCLEAR;
            break;
        case SDLK_KP_MEMADD:
            index = KEY_KP_MEMADD;
            break;
        case SDLK_KP_MEMSUBTRACT:
            index = KEY_KP_MEMSUBTRACT;
            break;
        case SDLK_KP_MEMMULTIPLY:
            index = KEY_KP_MEMMULTIPLY;
            break;
        case SDLK_KP_MEMDIVIDE:
            index = KEY_KP_MEMDIVIDE;
            break;
        case SDLK_KP_PLUSMINUS:
            index = KEY_KP_PLUSMINUS;
            break;
        case SDLK_KP_CLEAR:
            index = KEY_KP_CLEAR;
            break;
        case SDLK_KP_CLEARENTRY:
            index = KEY_KP_CLEARENTRY;
            break;
        case SDLK_KP_BINARY:
            index = KEY_KP_BINARY;
            break;
        case SDLK_KP_OCTAL:
            index = KEY_KP_OCTAL;
            break;
        case SDLK_KP_DECIMAL:
            index = KEY_KP_DECIMAL;
            break;
        case SDLK_KP_HEXADECIMAL:
            index = KEY_KP_HEXADECIMAL;
            break;
        case SDLK_LCTRL:
            index = KEY_LCTRL;
            break;
        case SDLK_LSHIFT:
            index = KEY_LSHIFT;
            break;
        case SDLK_LALT:
            index = KEY_LALT;
            break;
        case SDLK_LGUI:
            index = KEY_LGUI;
            break;
        case SDLK_RCTRL:
            index = KEY_RCTRL;
            break;
        case SDLK_RSHIFT:
            index = KEY_RSHIFT;
            break;
        case SDLK_RALT:
            index = KEY_RALT;
            break;
        case SDLK_RGUI:
            index = KEY_RGUI;
            break;
        case SDLK_MODE:
            index = KEY_MODE;
            break;
        case SDLK_AUDIONEXT:
            index = KEY_AUDIONEXT;
            break;
        case SDLK_AUDIOPREV:
            index = KEY_AUDIOPREV;
            break;
        case SDLK_AUDIOSTOP:
            index = KEY_AUDIOSTOP;
            break;
        case SDLK_AUDIOPLAY:
            index = KEY_AUDIOPLAY;
            break;
        case SDLK_AUDIOMUTE:
            index = KEY_AUDIOMUTE;
            break;
        case SDLK_MEDIASELECT:
            index = KEY_MEDIASELECT;
            break;
        case SDLK_WWW:
            index = KEY_WWW;
            break;
        case SDLK_MAIL:
            index = KEY_MAIL;
            break;
        case SDLK_CALCULATOR:
            index = KEY_CALCULATOR;
            break;
        case SDLK_COMPUTER:
            index = KEY_COMPUTER;
            break;
        case SDLK_AC_SEARCH:
            index = KEY_AC_SEARCH;
            break;
        case SDLK_AC_HOME:
            index = KEY_AC_HOME;
            break;
        case SDLK_AC_BACK:
            index = KEY_AC_BACK;
            break;
        case SDLK_AC_FORWARD:
            index = KEY_AC_FORWARD;
            break;
        case SDLK_AC_STOP:
            index = KEY_AC_STOP;
            break;
        case SDLK_AC_REFRESH:
            index = KEY_AC_REFRESH;
            break;
        case SDLK_AC_BOOKMARKS:
            index = KEY_AC_BOOKMARKS;
            break;
        case SDLK_BRIGHTNESSDOWN:
            index = KEY_BRIGHTNESSDOWN;
            break;
        case SDLK_BRIGHTNESSUP:
            index = KEY_BRIGHTNESSUP;
            break;
        case SDLK_DISPLAYSWITCH:
            index = KEY_DISPLAYSWITCH;
            break;
        case SDLK_KBDILLUMTOGGLE:
            index = KEY_KBDILLUMTOGGLE;
            break;
        case SDLK_KBDILLUMDOWN:
            index = KEY_KBDILLUMDOWN;
            break;
        case SDLK_KBDILLUMUP:
            index = KEY_KBDILLUMUP;
            break;
        case SDLK_EJECT:
            index = KEY_EJECT;
            break;
        case SDLK_SLEEP:
            index = KEY_SLEEP;
            break;
        case SDLK_APP1:
            index = KEY_APP1;
            break;
        case SDLK_APP2:
            index = KEY_APP2;
            break;
        case SDLK_AUDIOREWIND:
            index = KEY_AUDIOREWIND;
            break;
        case SDLK_AUDIOFASTFORWARD:
            index = KEY_AUDIOFASTFORWARD;
            break;
    }

    return index;
}

STATE combine_key_state(STATE last_frame, STATE current_frame) {
    if (current_frame == STATE_PRESSED) {

        if (last_frame == STATE_NORMAL || last_frame == STATE_RELEASED)
            return STATE_PRESSED;
        else if (last_frame == STATE_HOLDED || last_frame == STATE_PRESSED)
            return STATE_HOLDED;

    } else if (current_frame == STATE_RELEASED) {

        if (last_frame == STATE_HOLDED || last_frame == STATE_PRESSED)
            return STATE_RELEASED;
        else if (last_frame == STATE_NORMAL || last_frame == STATE_RELEASED)
            return STATE_NORMAL;

    }

    return STATE_NORMAL;
}

void handle_key_event(input_data_t *data, SDL_Event event) {
    SDL_KeyboardEvent key_event = event.key;
    SDL_Keysym key = key_event.keysym;
    KEY index = get_key_index(key.sym);

    STATE current_key_state;
    if (event.type == SDL_KEYDOWN)
        current_key_state = STATE_PRESSED;
    else
        current_key_state = STATE_RELEASED;

    STATE last_key_state = data->last_frame_keyboard[index];

    STATE final = combine_key_state(last_key_state, current_key_state);
    data->current_frame_keyboard[index] = final;
}

void handle_window_event(input_data_t *state, SDL_Event event) {
    SDL_WindowEvent win_event = event.window;
    if (win_event.event == SDL_WINDOWEVENT_RESIZED) {
        state->window_resized = true;
        state->window_new_size = get_vec2(event.window.data1, event.window.data2);
    }
}

void handle_mouse_event(input_data_t *state, SDL_Event event) {
    if (event.type == SDL_MOUSEMOTION) {
        SDL_MouseMotionEvent motion = event.motion;
        state->mouse_pos = get_vec2(motion.x, motion.y);
        state->mouse_delta = get_vec2(motion.xrel, motion.yrel);
    } else if (event.type == SDL_MOUSEWHEEL) {
        SDL_MouseWheelEvent wheel = event.wheel;

        // Inverted becasue wheel.y is position away from the user (upwards), but the coordinate system says that Y increases to bottom
        int scroll_delta = wheel.y * -1;

        if (wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
            scroll_delta = wheel.y * -1;

        state->mouse_scroll_delta = scroll_delta;
    } else {
        SDL_MouseButtonEvent button = event.button;
        state->mouse_pos = get_vec2(button.x, button.y);


    }
}

void update_input_data(input_data_t *input_data) {
    // RESET
    memcpy(input_data->last_frame_keyboard, input_data->current_frame_keyboard, sizeof(input_data->current_frame_keyboard));
    //memset(input_data->current_frame_keyboard, 0, sizeof(input_data->current_frame_keyboard));

    input_data->quit_event_called = false;
    input_data->window_resized = false;

    input_data->mouse_delta = get_vec2(0, 0);
    input_data->mouse_scroll_delta = 0;
    memcpy(input_data->last_frame_buttons, input_data->current_frame_buttons, sizeof(input_data->current_frame_buttons));

    // POLL

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                input_data->quit_event_called = true;
                break;
            case SDL_KEYDOWN: {
                int index = get_key_index(event.key.keysym.sym);
                input_data->current_frame_keyboard[index] = STATE_PRESSED;
            }
                break;
            case SDL_KEYUP: {
                int index = get_key_index(event.key.keysym.sym);
                input_data->current_frame_keyboard[index] = STATE_RELEASED;
            }
                break;
            case SDL_WINDOWEVENT:
                handle_window_event(input_data, event);
                break;
            case SDL_MOUSEMOTION:
            case SDL_MOUSEWHEEL:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                handle_mouse_event(input_data, event);
                break;
        }
    }

    // UPDATE FROM LAST TO CURRENT

    for (int i = 0; i < KEY_LAST; ++i) {
        STATE last_frame = input_data->last_frame_keyboard[i];
        STATE current_frame = input_data->current_frame_keyboard[i];

        STATE final = combine_key_state(last_frame, current_frame);
        input_data->current_frame_keyboard[i] = final;
    }
}


STATE get_key_state(input_data_t *input_data, KEY key) {
    STATE state = input_data->current_frame_keyboard[key];
    return state;
}

STATE get_button_state(input_data_t *input_data, BUTTON button) {
    STATE state = input_data->current_frame_buttons[button];
    return state;
}

bool is_key_normal(input_data_t *input_data, KEY key) {
    STATE state = get_key_state(input_data, key);
    return state == STATE_NORMAL;
}

bool is_key_pressed(input_data_t *input_data, KEY key) {
    STATE state = get_key_state(input_data, key);
    return state == STATE_PRESSED;
}

bool is_key_holded(input_data_t *input_data, KEY key) {
    STATE state = get_key_state(input_data, key);
    return state == STATE_HOLDED;
}

bool is_key_released(input_data_t *input_data, KEY key) {
    STATE state = get_key_state(input_data, key);
    return state == STATE_RELEASED;
}

bool is_button_normal(input_data_t *input_data, BUTTON button) {
    STATE state = get_button_state(input_data, button);
    return state == STATE_NORMAL;
}

bool is_button_pressed(input_data_t *input_data, BUTTON button) {
    STATE state = get_button_state(input_data, button);
    return state == STATE_PRESSED;
}

bool is_button_holded(input_data_t *input_data, BUTTON button) {
    STATE state = get_button_state(input_data, button);
    return state == STATE_HOLDED;
}

bool is_button_released(input_data_t *input_data, BUTTON button) {
    STATE state = get_button_state(input_data, button);
    return state == STATE_RELEASED;
}