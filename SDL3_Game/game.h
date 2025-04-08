#ifndef SDL3_GAME_GAME_H
#define SDL3_GAME_GAME_H

#include "common.h"
#include "init_shaders.h"

struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
    TTF_Font *font;
    SDL_Texture *text_texture;
    SDL_FRect text_rect;
    SDL_Event event;
    struct ShaderData grayscale_shader;
    bool is_running;
};

bool game_new(struct Game **game);
void game_free(struct Game **game);
void game_run(struct Game *g);
void game_events(struct Game *g);
void game_draw(struct Game *g, double delta_time);
void game_update(struct Game *g, double delta_time);

#endif // SDL3_GAME_GAME_H
