#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "Hello Window!"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
    TTF_Font *font;
    SDL_Texture *text_texture;
    SDL_FRect text_rect;
    SDL_Event event;
    bool is_running;
};

bool game_init_sdl(struct Game *g);
bool game_new(struct Game **game);
void game_free(struct Game **game);
void game_run(struct Game *g);
void game_poll_events(struct Game *g);
void game_draw(struct Game *g);
bool game_load_media(struct Game *g);
void game_set_random_draw_color(struct Game *g);



bool game_init_sdl(struct Game *g) {
    if (!SDL_Init(SDL_FLAGS)) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        fprintf(stderr, "Error initializing TTF: %s\n", SDL_GetError());
        return false;
    }

    g->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!g->window) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return false;
    }

    g->renderer = SDL_CreateRenderer(g->window, "gpu");
    if (!g->renderer) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return false;
    }

    //SDL_GPUDevice *device = (SDL_GPUDevice *)SDL_GetPointerProperty(SDL_GetRendererProperties(g->renderer), SDL_PROP_RENDERER_GPU_DEVICE_POINTER, NULL);
    //if (!device) {
    //    SDL_Log("Couldn't get GPU device: %s\n", SDL_GetError());
    //    return false;
    //}

    //SDL_GPUShaderFormat formats = SDL_GetGPUShaderFormats(device);
    //if (formats == SDL_GPU_SHADERFORMAT_INVALID) {
    //    SDL_Log("Couldn't get supported shader formats: %s", SDL_GetError());
    //    return false;
    //}

    //SDL_GPUShaderCreateInfo info;
    //SDL_zero(info);

    //info.format = SDL_GPU_SHADERFORMAT_SPIRV;
    //info.code = data->spirv_shader_source;
    //info.code_size = data->spirv_shader_source_len;


    SDL_Surface *icon_surface = IMG_Load("images/C_Logo.png");

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

bool game_load_media(struct Game *g) {
    g->background = IMG_LoadTexture(g->renderer, "images/sdl3.png");
    if (!g->background) {
        fprintf(stderr, "Error loading background texture: %s\n", SDL_GetError());
        return false;
    }

    g->font = TTF_OpenFont("fonts/Courier New.ttf", 48);
    if (!g->font) {
        fprintf(stderr, "Error loading font: %s\n", SDL_GetError());
        return false;
    }

    char *text = "Hello World!";
    SDL_Surface *text_surface = TTF_RenderText_Blended(g->font, text, strlen(text), (SDL_Color){255, 255, 255, 255});
    if (!text_surface) {
        fprintf(stderr, "Error rendering text surface: %s\n", SDL_GetError());
        return false;
    }

    g->text_rect.x = 0;
    g->text_rect.y = 0;
    g->text_rect.w = (float)text_surface->w;
    g->text_rect.h = (float)text_surface->h;

    g->text_texture = SDL_CreateTextureFromSurface(g->renderer, text_surface);

    SDL_DestroySurface(text_surface);
    text_surface = NULL;

    if (!g->text_texture) {
        fprintf(stderr, "Error creating text texture: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

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

void game_poll_events(struct Game *g) {
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
        game_poll_events(g);
        game_draw(g);

        SDL_Delay(1000 / 60);
    }
}

int main(int argc, char *argv[]) {
    bool exit_status = EXIT_FAILURE;

    struct Game *game = NULL;

    if (game_new(&game)) {
        game_run(game);

        exit_status = EXIT_SUCCESS;
    }

    game_free(&game);

    return exit_status;
}