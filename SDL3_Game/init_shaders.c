#include "init_shaders.h"
#include "shaderc/shaderc.h"
#include "read_file.h"
#include "game.h"

bool game_init_shaders(struct Game *g) {

    SDL_GPUDevice *device = (SDL_GPUDevice *)SDL_GetPointerProperty(SDL_GetRendererProperties(g->renderer), SDL_PROP_RENDERER_GPU_DEVICE_POINTER, NULL);
    GAME_ASSERT_SDL(device, "Couldn't get GPU device");

    SDL_GPUShaderFormat formats = SDL_GetGPUShaderFormats(device);
    GAME_ASSERT_SDL(formats != SDL_GPU_SHADERFORMAT_INVALID, "Couldn't get supported shader formats");
    GAME_ASSERT_MSG(formats & SDL_GPU_SHADERFORMAT_SPIRV, "SPIR-V shader format not supported");

    shaderc_compiler_t shader_compiler = shaderc_compiler_initialize();
    shaderc_compile_options_t shader_compiler_options = shaderc_compile_options_initialize();
    
    // create crt shader
    struct StringView fragment_shader_source;
    read_file(FRAGMENT_SHADER_SOURCE, &fragment_shader_source);

    shaderc_compilation_result_t result = shaderc_compile_into_spv(
        shader_compiler, fragment_shader_source.data, fragment_shader_source.size,
        shaderc_fragment_shader, "fragment_shader.frag", "main", shader_compiler_options
    );

    // free string view
    string_view_free(&fragment_shader_source);
    shaderc_compile_options_release(shader_compiler_options);
    shaderc_compiler_release(shader_compiler);

    if (shaderc_result_get_compilation_status(result) != shaderc_compilation_status_success) {
        SDL_Log("Shader compilation failed: %s\n", shaderc_result_get_error_message(result));
        return false;
    }

    size_t spv_size = shaderc_result_get_length(result);
    const char *spv_data = shaderc_result_get_bytes(result);

    SDL_GPUShaderCreateInfo info;
    SDL_zero(info);

    info.format = SDL_GPU_SHADERFORMAT_SPIRV;
    info.code = spv_data;
    info.code_size = spv_size;
    info.entrypoint = "main";
    info.num_samplers = 1;
    info.num_uniform_buffers = 0;
    info.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;

    SDL_GPUShader *shader = SDL_CreateGPUShader(device, &info);
    GAME_ASSERT_SDL(shader, "Couldn't create shader");

    SDL_GPURenderStateDesc desc;
    SDL_INIT_INTERFACE(&desc);
    desc.fragment_shader = shader;
    SDL_GPURenderState *state = SDL_CreateGPURenderState(g->renderer, &desc);
    GAME_ASSERT_SDL(state, "Couldn't create render state");

    g->shader = (struct ShaderData) {
        .device = device,
        .shader = shader,
        .state = state
    };

    return true;
}

void shader_data_free(struct ShaderData *shader_data) {
    SDL_ReleaseGPUShader(shader_data->device, shader_data->shader);
    SDL_DestroyGPURenderState(shader_data->state);
}