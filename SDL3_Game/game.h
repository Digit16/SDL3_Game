#ifndef SDL3_GAME_GAME_H
#define SDL3_GAME_GAME_H

#include "common.h"

struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
    SDL_Texture *target;
    TTF_Font *font;
    SDL_Texture *text_texture;
    SDL_FRect text_rect;
    SDL_Event event;
    SDL_GPUDevice *device;
    SDL_GPURenderState *render_state;
    bool is_running;
};

bool game_new(struct Game **game);
void game_free(struct Game **game);
void game_run(struct Game *g);
void game_events(struct Game *g);
void game_draw(struct Game *g);
void game_set_random_draw_color(struct Game *g);

#endif // SDL3_GAME_GAME_H
