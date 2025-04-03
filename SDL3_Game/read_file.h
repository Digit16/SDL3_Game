#ifndef SDL3_GAME_READ_FILE_H  
#define SDL3_GAME_READ_FILE_H  

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


struct StringView {
    const char *data;
    size_t size;
};

bool read_file(char *file_name, struct StringView *view);

void free_string_view(struct StringView *view);

#endif // SDL3_GAME_READ_FILE_H