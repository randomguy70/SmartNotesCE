#include <fileioc.h>

#include "includes/buffer.h"

// I will switch to gap buffer method later, but for now...
int fileToBuffer(const char *name, struct buffer *buffer) {
	ti_var_t fileSlot;
	int dataSize;
	
	fileSlot = ti_Open(name, "r");
	if(!fileSlot) return 0;
	
	dataSize = ti_GetSize(fileSlot);
	ti_Seek(MIN_FILE_SIZE, SEEK_SET, fileSlot);
	ti_Read(buffer->buffer, dataSize, 1, fileSlot);
	ti_Close(fileSlot);
	
	return dataSize;
}

// I will switch to gap buffer method later, but for now...
int bufferToFile(struct buffer *buffer, char *name) {
	ti_var_t fileSlot;
	
	fileSlot = ti_Open(name, "w+");
	if(!fileSlot) return 0;
	
	ti_Seek(0, SEEK_SET, fileSlot);
	ti_Write(HEADER_STR, sizeof HEADER_STR, 1, fileSlot);
	ti_Write(buffer->buffer, buffer->dataSize, 1, fileSlot);
	ti_Close(fileSlot);
	
	return MIN_FILE_SIZE + buffer->dataSize;
}