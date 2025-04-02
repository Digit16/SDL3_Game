#include "init_sdl.h"


bool game_init_sdl(struct Game* g) {
    if (!SDL_Init(SDL_FLAGS)) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        fprintf(stderr, "Error initializing TTF: %s\n", SDL_GetError());
        return false;
    }

    g->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (!g->window) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return false;
    }

    g->renderer = SDL_CreateRenderer(g->window, "gpu");
    if (!g->renderer) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderVSync(g->renderer, 1);

    SDL_Surface* icon_surface = IMG_Load("images/C_Logo.png");

    if (!icon_surface) {
        fprintf(stderr, "Error loading icon surface: %s\n", SDL_GetError());
        return false;
    }

    bool is_icon_set = SDL_SetWindowIcon(g->window, icon_surface);
    SDL_DestroySurface(icon_surface);
    if (!is_icon_set) {
        fprintf(stderr, "Error setting window icon: %s\n", SDL_GetError());
        return false;
    }

    return true;
}