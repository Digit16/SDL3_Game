#include "media.h"


bool game_load_media(struct Game* g) {
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

    SDL_Surface* text_surface = TTF_RenderText_Blended(g->font, TEXT_VALUE, strlen(TEXT_VALUE), (SDL_Color) { 255, 255, 255, 255 });
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