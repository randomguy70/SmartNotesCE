#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

#include "includes/file.h"

struct buffer {
	char data[FILE_BUFFER_SIZE];
	size_t sig_chars;
	uint16_t numLines;
	char *lines[200];
};

int fileToBuffer(const char *name, struct buffer* buffer);
int bufferToFile(struct buffer* buffer, char* name);

#endif