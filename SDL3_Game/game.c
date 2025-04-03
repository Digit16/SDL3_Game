#include "game.h"
#include "init_sdl.h"
#include "init_shaders.h"
#include "media.h"

bool game_new(struct Game **game) {
    // allocate new game struct
    *game = calloc(1, sizeof(struct Game));
    GAME_ASSERT_MSG(*game, "Error allocating memory for game");
    struct Game *g = *game;

    GAME_ASSERT(game_init_sdl(g));
    GAME_ASSERT(game_load_media(g));
    GAME_ASSERT(game_init_shaders(g));

    g->target = SDL_CreateTexture(g->renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
    GAME_ASSERT_SDL(g->target, "Couldn't create target texture");

    g->is_running = true;

    return true;
}

void game_free(struct Game **game) {
    if (*game) {
        struct Game *g = *game;

        SDL_DestroyTexture(g->text_texture);
        TTF_CloseFont(g->font);
        SDL_DestroyTexture(g->background);
        shader_data_free(&g->shader);
        SDL_DestroyRenderer(g->renderer);
        SDL_DestroyWindow(g->window);

        free(*game);
        *game = NULL;
    }

    TTF_Quit();
    SDL_Quit();
}

void game_events(struct Game *g) {
    while (SDL_PollEvent(&g->event)) {
        switch (g->event.type) {
        case SDL_EVENT_QUIT:
            g->is_running = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (g->event.key.key) {
            case SDLK_ESCAPE:
            case SDLK_Q:
                g->is_running = false;
                break;
            case SDLK_R:
                game_set_random_draw_color(g);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void game_set_random_draw_color(struct Game *g) {
    SDL_SetRenderDrawColor(g->renderer, (uint8_t)rand(), (uint8_t)rand(), (uint8_t)rand(), 255);
}

void game_draw(struct Game *g) {
    SDL_SetRenderTarget(g->renderer, g->target);

    SDL_RenderClear(g->renderer);
    SDL_FRect dst = {0, 0, 200, 100};
    SDL_RenderTexture(g->renderer, g->background, 0, &dst);
    SDL_FRect text_dst = {
        .x = (WINDOW_WIDTH - g->text_rect.w) / 2,
        .y = (WINDOW_HEIGHT - g->text_rect.h) / 2,
        .w = g->text_rect.w,
        .h = g->text_rect.h,
    };
    SDL_RenderTexture(g->renderer, g->text_texture, 0, &text_dst);

    SDL_SetRenderTarget(g->renderer, NULL);
    SDL_SetRenderGPUState(g->renderer, g->shader.state);
    SDL_RenderTexture(g->renderer, g->target, NULL, NULL);
    SDL_SetRenderGPUState(g->renderer, NULL);

    SDL_RenderPresent(g->renderer);
}

void game_run(struct Game *g) {

    while (g->is_running) {
        game_events(g);
        game_draw(g);

        SDL_Delay(1000 / 60);
    }
}
