#include <fileioc.h>

#include "includes/buffer.h"

int fileToBuffer(const char *name, struct buffer* buffer) {
	uint8_t fileSlot;
	int fileSize;
	
	fileSlot = ti_Open(name, "r");
	
	if(!fileSlot) {
		ti_Close(fileSlot);
		return 0;
	}
	
	fileSize = ti_GetSize(fileSlot);
	
	// read the appropriate number of bytes from the file into the array
	ti_Seek(MIN_FILE_SIZE, 0, fileSlot);
	ti_Read(buffer->data, fileSize, 1, fileSlot);
	buffer->size = fileSize;
	
	return fileSize;
}

int bufferToFile(struct buffer* buffer, char* name) {
	uint8_t fileSlot;
	int fileSize;
	
	fileSlot = ti_Open(name, "r");
	
	if(!fileSlot) {
		ti_Close(fileSlot);
		return 0;
	}
	
	fileSize = ti_GetSize(fileSlot);
	
	ti_Seek(MIN_FILE_SIZE, 0, fileSlot);
	ti_Write(buffer->data, buffer->size, 1, fileSlot);
	ti_Resize(MIN_FILE_SIZE + buffer->size, fileSlot);
	ti_Close(fileSlot);
	
	return fileSize;
}

