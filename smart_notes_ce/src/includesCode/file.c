#include "main.h"
#include <fileioc.h>
#include <stdint.h>
#include <string.h>

#include <includes/file.h>
#include <includes/text.h>
#include <includes/ui.h>
#include <includes/editor.h>

void archiveAll()
{
   ti_var_t fileSlot;
   char * fileName;
   void * search_pos = NULL;
   
   while (((fileName = ti_Detect(&search_pos, "TXT")) != NULL)) {
      fileSlot = ti_Open(fileName, "r");
      ti_SetArchiveStatus(true, fileSlot);
		ti_Close(fileSlot);
   }
	
}

uint8_t getNumFiles(const char * txt)
{
   uint8_t result = 0;
   void * pos = NULL;
	
   while (ti_Detect(&pos, txt) != NULL)
      result++;
	
   return result;
}

// asks for user to input a string and makes a new file if one doesn't already exist with that name
bool newFile(void) {
   char buffer[9] = {0};
   uint8_t fileSlot = 0;
	
   if (inputString(buffer, 8, "New File") > 0) {
		if(fileExists(buffer)) {
			return false;
		}
      fileSlot = ti_Open(buffer, "w");
		
		if (fileSlot) {
      	ti_Write("TXT", 3, 1, fileSlot);
		}
		
		ti_Close(fileSlot);
      return true;
   }
	
   return false;
}

// gives an option whether or not to delete the selected file
bool checkIfDeleteFile(char *name) {	
	char message[100] = {"Are you sure you want to delete "};
	strcat(message, name);
	strcat(message, "?");
	
	if(alert(message) == true) {
		ti_Delete(name);
		return 1;
	}
	
	return 0;
}

bool renameFile(const char *name) {
	
	char newNameBuffer[10] = {0};
	char message[25] = {"Rename "};
	
	strcat(message, name);
	
	if(inputString(newNameBuffer, 8, message) > 0) {
		ti_Rename(name, newNameBuffer);
		return true;
	}
	
	return false;
}

int loadFile(struct file *file, char *name) {
	
	// deal with both the names (os name and the full name. the os name is what the os calls the file, and the full name is what the user sees. the extra characters of the full name are stored in the file itself and appended on to the os name to form the full name)
	strcpy(file->os_name, name);
	getFullName(file->full_name, name);
	
	ti_CloseAll();
	file->slot = ti_Open(name, "r+");
	
	if(!file->slot)
		return false;
	
	// get its size
	file->size = ti_GetSize(file->slot);
	
	// copy the file's data into the array
	fileToArray(name, file->buffer);
	
	file->charsInBuffer = file->size - MIN_FILE_SIZE;
	
	return 1;
};

int saveFile(char* name, struct buffer* buffer) {
	arrayToFile(file->buffer, file->os_name, file->charsInBuffer);
	return file->charsInBuffer;
};

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
	
	// read the appropriate number of bytes from the array into the file
	ti_Seek(MIN_FILE_SIZE, 0, fileSlot);
	ti_Write(array, bytes, 1, fileSlot);
	
	ti_Close(fileSlot);
	return fileSize;
}

uint8_t getFullName(char *fullNameBuffer, char *osName) {
	
	uint8_t osNameLen;
	uint8_t fileSlot;
	char *extraCharsPtr;
	
	ti_CloseAll();
	fileSlot = ti_Open(osName, "r");
	extraCharsPtr = ti_GetDataPtr(fileSlot) + strlen(HEADER_STR);
	osNameLen = strlen(osName);
	
	strcpy(fullNameBuffer, osName);
	// copyChars(&(fullNameBuffer[osNameLen]), extraCharsPtr, 10);
	copyWordL(fullNameBuffer + osNameLen, extraCharsPtr, 10);
	
	return strlen(fullNameBuffer);
}

bool fileExists(char* name) {
	ti_var_t slot = ti_Open(name, "r");
	ti_Close(slot);
	if(!slot)
		return false;
	return true;
}
