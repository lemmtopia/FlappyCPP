#pragma once

#include "sdl_state.h"

enum InputDevice {
    INPUT_DEVICE_KEYBOARD,
    INPUT_DEVICE_MOUSE,
    INPUT_DEVICE_GAMEPAD
};

struct InputState {
    SDL_Gamepad* pad = NULL;
    SDL_MouseButtonFlags mouse_button_flags = 0;

    f32 mouse_x = 0;
    f32 mouse_y = 0;

    InputDevice device = INPUT_DEVICE_KEYBOARD;

    int num_keys = 0;
    const bool* keys = NULL;
    bool* old_keys = NULL;
};

extern InputState input_state;

void initialize_input_state(void);
void update_input_state(void);
void destroy_input_state(void);