#include "media.h"

bool game_load_media(struct Game *g) {
    g->background = IMG_LoadTexture(g->renderer, "images/sdl3.png");
    GAME_ASSERT_SDL(g->background, "Error loading background texture");

    g->font = TTF_OpenFont("fonts/Courier New.ttf", 48);
    GAME_ASSERT_SDL(g->font, "Error loading font");

    SDL_Surface *text_surface = TTF_RenderText_Blended(g->font, TEXT_VALUE, strlen(TEXT_VALUE), (SDL_Color){255, 255, 255, 255});
    GAME_ASSERT_SDL(text_surface, "Error rendering text surface");

    g->text_rect = (SDL_FRect){0, 0, (float)text_surface->w, (float)text_surface->h};
    g->text_texture = SDL_CreateTextureFromSurface(g->renderer, text_surface);

    // clear warning "Parameter 'src' is invalid"
    SDL_ClearError();

    SDL_DestroySurface(text_surface);
    text_surface = NULL;
    GAME_ASSERT_SDL(g->text_texture, "Error creating text texture");

    return true;
}