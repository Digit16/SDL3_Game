#include "init_gpu.h"

bool game_init_gpu(struct Game *g) {

    SDL_GPUDevice *device = (SDL_GPUDevice *)SDL_GetPointerProperty(SDL_GetRendererProperties(g->renderer), SDL_PROP_RENDERER_GPU_DEVICE_POINTER, NULL);
    if (!device) {
        SDL_Log("Couldn't get GPU device: %s\n", SDL_GetError());
        return false;
    }

    SDL_GPUShaderFormat formats = SDL_GetGPUShaderFormats(device);
    if (formats == SDL_GPU_SHADERFORMAT_INVALID) {
        SDL_Log("Couldn't get supported shader formats: %s", SDL_GetError());
        return false;
    }

    SDL_GPUShaderCreateInfo info;
    SDL_zero(info);

    info.format = SDL_GPU_SHADERFORMAT_SPIRV;
    info.code = data->spirv_shader_source;
    info.code_size = data->spirv_shader_source_len;
}