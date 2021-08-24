#include <fileioc.h>

#include "includes/buffer.h"

int fileToBuffer(const char *name, struct buffer* buffer) {
	uint8_t fileSlot;
	int fileSize;
	
	// safely open the file and return if there isn't a file with the given name
	ti_CloseAll();
	fileSlot = ti_Open(name, "r");
	
	if(!fileSlot) {
		ti_Close(fileSlot);
		return 0;
	}
		
	fileSize = ti_GetSize(fileSlot);
	
	// read the appropriate number of bytes from the file into the array
	ti_Seek(MIN_FILE_SIZE, 0, fileSlot);
	ti_Read(buffer->data, fileSize, 1, fileSlot);
	buffer->sig_chars=fileSize;
	
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
	ti_Write(buffer->data, buffer->sig_chars, 1, fileSlot);
	ti_Resize(MIN_FILE_SIZE + buffer->sig_chars, fileSlot);
	ti_Close(fileSlot);
	
	return fileSize;
}

