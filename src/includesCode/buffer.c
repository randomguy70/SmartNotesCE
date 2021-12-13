#include <fileioc.h>

#include "includes/buffer.h"

int fileToBuffer(const char *name, struct fileBuffer *buffer) {
	ti_var_t fileSlot;
	size_t dataSize;
	
	fileSlot = ti_Open(name, "r");
	if(!fileSlot) return 0;
	
	dataSize = ti_GetSize(fileSlot) - HEADER_STR_LEN;
	ti_Seek(HEADER_STR_LEN, SEEK_SET, fileSlot);
	ti_Read(buffer->data[FILE_BUFFER_SIZE - dataSize], dataSize, 1, fileSlot);
	ti_Close(fileSlot);
	
	buffer->dataLenAfterCursor = dataSize;
	buffer->dataLenBeforeCursor = 0;
	buffer->totalDataLen = dataSize;
	buffer->cursorOffset = 0;
	
	return dataSize;
}

int bufferToFile(struct fileBuffer *buffer, char *name) {
	ti_var_t fileSlot;
	
	fileSlot = ti_Open(name, "w+");
	if(!fileSlot) return 0;
	
	ti_Seek(0, SEEK_SET, fileSlot);
	ti_Write(HEADER_STR, HEADER_STR_LEN, 1, fileSlot);
	ti_Write(buffer->data, buffer->dataLenBeforeCursor, 1, fileSlot);
	ti_Write(buffer->data + FILE_BUFFER_SIZE - buffer->dataLenAfterCursor, buffer->dataLenAfterCursor, 1, fileSlot);
	ti_Resize(HEADER_STR_LEN + buffer->totalDataLen, fileSlot);
	ti_Close(fileSlot);
	
	return MIN_FILE_SIZE + buffer->dataSize;
}