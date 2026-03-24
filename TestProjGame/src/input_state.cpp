#include "input_state.h"

InputState input_state;

void initialize_input_state(void) {
    input_state.keys = SDL_GetKeyboardState(&input_state.num_keys);

    input_state.old_keys = (bool*)malloc(input_state.num_keys);

    // Setup gamepad
    int pad_count = 0;
    SDL_JoystickID* pad_ids = SDL_GetGamepads(&pad_count);

    if (pad_count > 0) {
        input_state.pad = SDL_OpenGamepad(pad_ids[0]);
    }
}

void update_input_state(void) {
    memcpy(input_state.old_keys, input_state.keys, input_state.num_keys);

    input_state.mouse_button_flags = SDL_GetMouseState(&input_state.mouse_x, &input_state.mouse_y);
}

void destroy_input_state(void) {
    free(input_state.old_keys);
    SDL_CloseGamepad(input_state.pad);
}