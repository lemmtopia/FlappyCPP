#include "sdl_state.h"

static int window_scale = 2;
SDL_State sdl_state;

i8 initialize_sdl_state() {
    SDL_InitFlags init_flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
    if (SDL_HasGamepad()) {
        init_flags |= SDL_INIT_GAMEPAD;
    }

    if (!SDL_Init(init_flags)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init error!", SDL_GetError(), NULL);
        return -1;
    }

    // Window config
    u16 window_width = BASE_W * window_scale;
    u16 window_height = BASE_H * window_scale;

    bool fullscreen = false;
    SDL_WindowFlags window_flags = 0;
    window_flags |= (fullscreen) ? SDL_WINDOW_FULLSCREEN : 0;

    sdl_state.window = SDL_CreateWindow(GAME_TITLE, window_width, window_height, window_flags);

    if (sdl_state.window == NULL) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow error!", SDL_GetError(), NULL);
        destroy_sdl_state();
        return -2;
    }

    sdl_state.renderer = SDL_CreateRenderer(sdl_state.window, NULL);

    if (sdl_state.renderer == NULL) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateRenderer error!", SDL_GetError(), NULL);
        destroy_sdl_state();
        return -3;
    }

    //SDL_SetRenderVSync(sdl_state.renderer, SDL_WINDOW_SURFACE_VSYNC_ADAPTIVE);

    // Set virtual resolution
    SDL_SetRenderLogicalPresentation(sdl_state.renderer, BASE_W, BASE_H, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

    // Init mix
    MIX_Init();

    sdl_state.mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (sdl_state.mixer == NULL) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "MIX_CreateMixerDevice failed!", SDL_GetError(), NULL);
        return -4;
    }

    return 0;
}

void process_exit_input(void) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT) sdl_state.is_running = false;
    }
}

void destroy_sdl_state(void) {
    MIX_DestroyMixer(sdl_state.mixer);
    SDL_DestroyRenderer(sdl_state.renderer);
    SDL_DestroyWindow(sdl_state.window);
    SDL_Quit();
}

SDL_Texture* sdl_state_load_texture(SDL_State state, const char* path) {
    return IMG_LoadTexture(state.renderer, path);
}