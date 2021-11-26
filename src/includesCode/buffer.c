#include <fileioc.h>

#include "includes/buffer.h"

int fileToBuffer(const char *name, struct buffer *buffer) {
	ti_var_t file_slot;
	int data_to_transfer;
	
	file_slot = ti_Open(name, "r");
	
	if(!file_slot) {
		ti_Close(file_slot);
		return 0;
	}
	
	data_to_transfer = ti_GetSize(file_slot) - 3;
	
	if (data_to_transfer > MAX_BUFFER_SIZE)
	{
		data_to_transfer = MAX_BUFFER_SIZE;
	}
	
	ti_Seek(MIN_FILE_SIZE, SEEK_SET, file_slot);
	ti_Read(buffer->buf1, data_to_transfer, 1, file_slot);
	ti_Close(file_slot);
	
	buffer->total_size = data_to_transfer;
	buffer->buf1_size = data_to_transfer;
	buffer->buf2_size = 0;
	
	return data_to_transfer;
}

int bufferToFile(struct buffer *buffer, char *name) {
	ti_var_t file_slot;
	
	file_slot = ti_Open(name, "w");
	
	if(!file_slot) {
		ti_Close(file_slot);
		return 0;
	}
	
	ti_Seek(MIN_FILE_SIZE, SEEK_SET, file_slot);
	ti_Write("TXT", 3, 1, file_slot);
	
	ti_Seek(MIN_FILE_SIZE, SEEK_SET, file_slot);
	ti_Write(buffer->buf1, buffer->buf1_size, 1, file_slot);
	ti_Write(buffer->buf2, buffer->buf2_size, 1, file_slot);
	
	ti_Close(file_slot);
	
	return MIN_FILE_SIZE + buffer->buf1_size + buffer->buf2_size;
}
