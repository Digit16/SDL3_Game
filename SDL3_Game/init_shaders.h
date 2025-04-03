#ifndef SDL3_GAME_INIT_GPU_H
#define SDL3_GAME_INIT_GPU_H

#include "common.h"

struct ShaderData {
    SDL_GPUDevice *device;
    SDL_GPUShader *shader;
    SDL_GPURenderState *state;
};

#include "game.h"

bool game_init_shaders(struct Game *g);

void shader_data_free(struct ShaderData *shader_data);

#endif // SDL3_GAME_INIT_GPU_H