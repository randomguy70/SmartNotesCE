#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

#include "includes/file.h"

#define MAX_BUFFER_SIZE 16000

// file data buffer
// buf1 = data before cursor
// buf2 = data after cursor
struct buffer {
	char buf1[MAX_BUFFER_SIZE];
	char buf2[MAX_BUFFER_SIZE];
	int buf1_size;
	int buf2_size;
	int total_size;
};

int fileToBuffer(const char *name, struct buffer* buffer);
int bufferToFile(struct buffer* buffer, char* name);

#endif