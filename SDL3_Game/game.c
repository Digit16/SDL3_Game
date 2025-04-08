#include "game.h"
#include "init_sdl.h"
#include "init_shaders.h"
#include "media.h"
#include <inttypes.h>

bool game_new(struct Game **game) {
    // allocate new game struct
    *game = calloc(1, sizeof(struct Game));
    GAME_ASSERT_MSG(*game, "Error allocating memory for game");
    struct Game *g = *game;

    GAME_ASSERT(game_init_sdl(g));
    GAME_ASSERT(game_load_media(g));
    GAME_ASSERT(game_init_shaders(g));

    g->is_running = true;

    return true;
}

void game_free(struct Game **game) {
    if (*game) {
        struct Game *g = *game;

        SDL_DestroyTexture(g->text_texture);
        TTF_CloseFont(g->font);
        SDL_DestroyTexture(g->background);
        shader_data_free(&g->grayscale_shader);
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
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void game_draw(struct Game *g, double delta_time) {

    // use grayscale shader
    // SDL_SetRenderGPUState(g->renderer, g->grayscale_shader.state);

    SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
    SDL_RenderClear(g->renderer);
    SDL_FRect dst = {0, 0, 200, 100};
    SDL_RenderTexture(g->renderer, g->background, 0, &dst);
    SDL_FRect text_dst = {
        .x = (WINDOW_WIDTH - g->text_rect.w) / 2,
        .y = (WINDOW_HEIGHT - g->text_rect.h) / 2,
        .w = g->text_rect.w,
        .h = g->text_rect.h,
    };

    SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 255);
    SDL_RenderRect(g->renderer, &text_dst);

    SDL_RenderTexture(g->renderer, g->text_texture, 0, &text_dst);
    char *text = "Hello World!";
    SDL_RenderDebugText(g->renderer, 100, 100, text);

    // switch back to default shader
    SDL_SetRenderGPUState(g->renderer, NULL);

    // render frame
    SDL_RenderPresent(g->renderer);
}

void game_update(struct Game *g, double delta_time) {
    // SDL_Log("Delta time: %lf\n", 1 / delta_time);
}

void game_run(struct Game *g) {
    uint32_t last_frame_ms = SDL_GetTicks();

    while (g->is_running) {

        uint32_t time_ms = SDL_GetTicks();
        double delta_time = (double)(time_ms - last_frame_ms) / 1000.0;
        last_frame_ms = time_ms;

        game_events(g);
        game_update(g, delta_time);
        game_draw(g, delta_time);

        SDL_Delay(TARGET_FRAME_TIME_MS);
    }
}