#include "string_view.h"
#include <stdio.h>
#include <stdlib.h>

void string_view_free(StringView* view) {
	free((void*)view->data);
	view->data = NULL;
	view->size = 0;
}


StringView read_file_to_string_view(const char* filename) {
	StringView view = STRING_VIEW_INIT;

	FILE* file = fopen(filename, "rb");
	if (file == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	if (fseek(file, 0, SEEK_END) != 0) {
		perror("fseek");
		exit(EXIT_FAILURE);
	}
	long size = ftell(file);
	if (size == -1) {
		perror("ftell");
		exit(EXIT_FAILURE);
	}
	rewind(file);
	char* buffer = malloc(size + 1);
	if (buffer == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	size_t read = fread(buffer, 1, size, file);
	if (read != size) {
		perror("fread");
		exit(EXIT_FAILURE);
	}

	buffer[size] = '\0';

	fclose(file);
	view.data = buffer;
	view.size = size;

	return view;
}
