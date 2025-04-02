#include "game.h"
#include "media.h"

bool game_new(struct Game **game) {
    *game = calloc(1, sizeof(struct Game));
    if (!*game) {
        fprintf(stderr, "Error allocating memory for game\n");
        return false;
    }
    struct Game *g = *game;

    if (!game_init_sdl(g)) {
        return false;
    }

    if (!game_load_media(g)) {
        return false;
    }

    srand((unsigned int)time(NULL));

    g->is_running = true;

    return true;
}

void game_free(struct Game **game) {
    if (*game) {
        struct Game *g = *game;

        if (g->text_texture) {
            SDL_DestroyTexture(g->text_texture);
            g->text_texture = NULL;
        }

        if (!g->font) {
            TTF_CloseFont(g->font);
            g->font = NULL;
        }

        if (!g->background) {
            SDL_DestroyTexture(g->background);
            g->background = NULL;
        }

        if (!g->renderer) {
            SDL_DestroyRenderer(g->renderer);
            g->renderer = NULL;
        }

        if (!g->window) {
            SDL_DestroyWindow(g->window);
            g->window = NULL;
        }

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
    SDL_RenderClear(g->renderer);
    SDL_FRect dst = {0, 0, 400, 200};
    SDL_RenderTexture(g->renderer, g->background, 0, &dst);
    SDL_RenderTexture(g->renderer, g->text_texture, 0, &g->text_rect);
    SDL_RenderPresent(g->renderer);
}

void game_run(struct Game *g) {

    while (g->is_running) {
        game_events(g);
        game_draw(g);

        SDL_Delay(1000 / 60);
    }
}
