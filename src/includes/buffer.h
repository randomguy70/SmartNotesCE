#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

#include "includes/file.h"

#define MAX_BUFFER_SIZE 16000

// file data buffer (using gap buffer method)
// buf1 = ascii data before cursor
// buf2 = ascii data after cursor
struct buffer {
	char data[MAX_BUFFER_SIZE * 2];
	unsigned int dataSizeBeforeCursor;
	unsigned int dataSizeAfterCursor;
	unsigned int cursorOffset;
};

int fileToBuffer(const char *name, struct buffer* buffer);
int bufferToFile(struct buffer* buffer, char* name);

#endif