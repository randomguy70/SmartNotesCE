#include "main.h"
#include <fileioc.h>
#include <stdint.h>
#include <string.h>

#include <includes/file.h>
#include <includes/text.h>
#include <includes/ui.h>

void archiveAll()
{
   uint8_t fileSlot;
   char * fileName;
   void * search_pos = NULL;
   
   ti_CloseAll();
   while (((fileName = ti_Detect(&search_pos, "TXT")) != NULL)) {
      fileSlot = ti_Open(fileName, "r");
      ti_SetArchiveStatus(1, fileSlot);
   }
   ti_CloseAll();
}

uint8_t getNumFiles(const char * txt) {
   uint8_t result = 0;
   void * pos = NULL;
   while (ti_Detect(&pos, txt) != NULL)
      result++;
	
   return result;
}

// asks for user to input a string and makes a new file if one doesn't already exist with that name
uint8_t newFile(void) {
   char buffer[9] = {0};
   uint8_t file = 0;
	
   ti_CloseAll();
	
   if (inputString(buffer, 8, "New File")>0) {
      file = ti_Open(buffer, "r");
      if (!file) {
         file = ti_Open(buffer, "w+");
      }
		if (file) {
      	ti_Write("TXT", 3, 1, file);
		}
		
		ti_CloseAll();
		
      return 1;
   }
	
   return 0;
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

// formats the file with the given name into an organized structure (hence the struct)
int loadFile(struct file *file, const char *name) {
	
	// deal with both the names (os name and the full name. the os name is what the os calls the file, and the full name is what the user sees. the extra characters of the full name are stored in the file itself and appended on to the os name to form the full name)
	strcpy(file->os_name, name);
	getFullName(file->full_name, name);
	
	ti_CloseAll();
	file->slot = ti_Open(name, "r+");
	
	if(!file->slot)
		return false;
	
	// get its size
	file->size = ti_GetSize(file->slot);
	
	// get some direct pointers to the file's data (a SHARP idea! hah hah hah :)
	file->txtStart = ti_GetDataPtr(file->slot) + MIN_FILE_SIZE;
	file->txtEnd = file->txtStart + file->size;
	
	// create a malloced array to hold the file's text data
	file->buffer = (char *) malloc(file->size + 20);
	
	// copy the file's data into the array
	fileToArray(name, file->buffer);
	
	// record the current byte size of the array (for resizing because of insertion and deletion)
	file->bufferSize = file->size + 20;
	file->charsInBuffer = file->size;
	
	return 1;
};

int saveFile(struct file *file, char *name) {
	
}

int closeFile(struct file *file) {
	ti_CloseAll();
	free(file->buffer);
	
	return 1;
};

int fileToArray(const char *name, char *array) {
	uint8_t fileSlot;
	int fileSize;
	
	// safely open the file and return if there isn't a file with the given name
	ti_CloseAll();
	fileSlot = ti_Open(name, "r");
	
	if(!fileSlot)
		return 0;
	
	fileSize = ti_GetSize(fileSlot);
	
	// read the appropriate number of bytes from the file into the array
	ti_Seek(MIN_FILE_SIZE, 0, fileSlot);
	ti_Read(array, fileSize, 1, fileSlot);
	
	return fileSize;
}

int arrayToFile(char *array, const char *name, int bytes) {
	uint8_t fileSlot;
	int fileSize;
	
	// safely open the file and return if there isn't a file with the given name
	ti_CloseAll();
	fileSlot = ti_Open(name, "r");
	
	if(!fileSlot)
		return 0;
	
	fileSize = ti_GetSize(fileSlot);
	
	// read the appropriate number of bytes from the array into the file
	ti_Seek(MIN_FILE_SIZE, 0, fileSlot);
	ti_Write(array, bytes, 1, fileSlot);
	
	return fileSize;
}

uint8_t getFullName(char *fullNameBuffer, char *osName) {
	
	uint8_t osNameLen;
	uint8_t fullNameLen;
	uint8_t fileSlot;
	char *extraCharsPtr;
	
	ti_CloseAll();
	fileSlot = ti_Open(osName, "r");
	extraCharsPtr = ti_GetDataPtr(fileSlot) + 3;
	osNameLen = strlen(osName);
	
	strcpy(fullNameBuffer, osName);
	copyChars(&(fullNameBuffer[osNameLen]), extraCharsPtr, 10);
	
	return strlen(fullNameBuffer);
}