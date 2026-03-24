#pragma once

#include "typedefs.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>

#define GAME_TITLE "testicle (small test)"
#define BASE_W 640
#define BASE_H 360

struct SDL_State {
    bool is_running = false;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    MIX_Mixer* mixer = NULL;
};

extern SDL_State sdl_state;

i8 initialize_sdl_state(void);
void process_exit_input(void);
void destroy_sdl_state(void);

SDL_Texture* sdl_state_load_texture(SDL_State state, const char* path);