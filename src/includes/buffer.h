#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

#include "includes/file.h"

#define FILE_BUFFER_SIZE (2 * MAX_FILE_DATA_SIZE)

struct fileBuffer {
	char data[FILE_BUFFER_SIZE];
	size_t dataLenBeforeCursor;
	size_t dataLenAfterCursor;
	size_t totalDataLen;
	int cursorOffset;
};

int fileToBuffer(const char *name, struct fileBuffer *buffer);
int bufferToFile(struct fileBuffer *buffer, char* name);

#endif