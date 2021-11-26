#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

#include "includes/file.h"

#define MAX_BUFFER_SIZE 16000

// file data buffer (using gap buffer method)
// buf1 = ascii data before cursor
// buf2 = ascii data after cursor
struct buffer {
	char buf1[MAX_BUFFER_SIZE];
	char buf2[MAX_BUFFER_SIZE];
	
	int buf1_size;
	int buf2_size;
	int buf2_pos;
	int buf2_pos;
	
	int total_size;
};

int fileToBuffer(const char *name, struct buffer* buffer);
int bufferToFile(struct buffer* buffer, char* name);

#endif