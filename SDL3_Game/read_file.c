#include "read_file.h"

#include <SDL3/SDL.h>
#define LOG_ERROR(...) SDL_Log(__VA_ARGS__)

bool read_file(char *file_name, struct StringView *view) {

    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        LOG_ERROR("Error opening file: %s\n", file_name);
        return false;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        LOG_ERROR("Error seeking to end of file: %s\n", file_name);
        fclose(file);
        return false;
    }
    long size = ftell(file);
    if (size == -1) {
        LOG_ERROR("Error getting file size: %s\n", file_name);
        fclose(file);
        return false;
    }
    rewind(file);
    char *buffer = malloc(size + 1);
    if (buffer == NULL) {
        LOG_ERROR("Error allocating memory for file buffer: %s\n", file_name);
        fclose(file);
        return false;
    }
    size_t read = fread(buffer, 1, size, file);
    if (read != size) {
        LOG_ERROR("Error reading file: %s\n", file_name);
        free(buffer);
        fclose(file);
        return false;
    }

    buffer[size] = '\0';

    fclose(file);
    view->data = buffer;
    view->size = size;

    return true;
}

void string_view_free(struct StringView *view) {
    if (view->data) {
        free((void *)view->data);
        view->data = NULL;
    }
    view->size = 0;
}