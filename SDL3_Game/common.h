#ifndef SDL3_GAME_COMMON_H
#define SDL3_GAME_COMMON_H

#define SDL_MAIN_HANDLED

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

#define TEXT_VALUE "Hello World!"

#define FRAGMENT_SHADER_SOURCE "shaders/fragment_shader.frag"

// assert with message
#define GAME_ASSERT_MSG(x, msg)     \
    do {                            \
        if (!(x)) {                 \
            SDL_Log("%s\n", (msg)); \
            return false;           \
        }                           \
    } while (0)

// assert SDL operation and print message along with sdl error
#define GAME_ASSERT_SDL(x, msg)                         \
    do {                                                \
        if (!(x)) {                                     \
            SDL_Log("%s: %s\n", (msg), SDL_GetError()); \
            return false;                               \
        }                                               \
    } while (0)

// assert without message (used to pass down already handled errors)
#define GAME_ASSERT(x)    \
    do {                  \
        if (!(x)) {       \
            return false; \
        }                 \
    } while (0)

#endif // SDL3_GAME_COMMON_H
