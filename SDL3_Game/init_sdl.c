#include "init_sdl.h"

bool game_init_sdl(struct Game *g) {

    GAME_ASSERT_SDL(SDL_Init(SDL_FLAGS), "Error initializing SDL");
    GAME_ASSERT_SDL(TTF_Init(), "Error initializing TTF");

    g->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    GAME_ASSERT_SDL(g->window, "Error creating window");

    g->renderer = SDL_CreateRenderer(g->window, "gpu");
    GAME_ASSERT_SDL(g->renderer, "Error creating renderer");

    SDL_Surface *icon_surface = IMG_Load("images/C_Logo.png");
    GAME_ASSERT_SDL(icon_surface, "Error loading icon surface");

    bool is_icon_set = SDL_SetWindowIcon(g->window, icon_surface);
    SDL_DestroySurface(icon_surface);
    GAME_ASSERT_SDL(is_icon_set, "Error setting window icon");

    return true;
}