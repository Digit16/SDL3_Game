#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <stdio.h>

typedef struct StringView {
	const char* data;
	size_t size;
} StringView;

#define STRING_VIEW_INIT {NULL, 0}

void string_view_free(StringView* view);

StringView read_file_to_string_view(const char* filename);

#endif // STRING_VIEW_H
