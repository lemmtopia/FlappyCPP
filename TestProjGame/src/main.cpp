#include "typedefs.h"
#include "sdl_state.h"
#include "input_state.h"
#include "util.h"
#include "game.h"

static i8 error_code = 0;

int main(int argc, char** argv) {
    int error_code = initialize_sdl_state();

    if (error_code != 0) {
        return error_code;
    }

    i64 last_frame_time = SDL_GetTicks();
    f64 frame_accumulator = 0;
    f32 fuzzy_factor = 2.0;

    // Initialize the game data
    game_setup();

    // Already initialized. Can start the main loop
    sdl_state.is_running = true;
    while (sdl_state.is_running) {
        i64 current_frame_time = SDL_GetTicks();
        i64 delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        if (delta_time > game_state.update_rate * 8) delta_time = game_state.update_rate;
        if (delta_time < 0) delta_time = 0;

        frame_accumulator += delta_time;

        process_exit_input();
        while (frame_accumulator >= 1000.0 / (game_state.update_rate + fuzzy_factor)) {
            game_update();
            frame_accumulator -= 1000.0 / game_state.update_rate;
            if (frame_accumulator < 0) frame_accumulator = 0;
        }

        // Render
        game_render();
    }

    game_destroy_resources();

    destroy_input_state();
    destroy_sdl_state();
    return 0;
}