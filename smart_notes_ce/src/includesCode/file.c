#include "main.h"
#include <fileioc.h>
#include <stdint.h>
#include <string.h>

#include <includes/file.h>
#include <includes/text.h>
#include <includes/ui.h>
#include <includes/editor.h>
#include <includes/buffer.h>

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
	ti_var_t fileSlot = 0;
	
	if (inputString(buffer, 8, "New File") > 0)
	{
		fileSlot = ti_Open(buffer, "w+");
		if(!fileSlot)
		{
			return false;
		}
		
		if (fileSlot)
		{
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
	
	if(alert(message) == true && fileExists(name)) {
		ti_Delete(name);
		return 1;
	}
	
	return 0;
}

bool renameFile(const char *name) {
	
	char newNameBuffer[10] = {0};
	char message[8+9] = {"Rename "};
	
	strcat(message, name);
	
	if(inputString(newNameBuffer, 8, message) > 0) {
		// ti_Rename(name, newNameBuffer);
		return true;
	}
	
	return false;
}

int loadFile(char *name, struct buffer* buffer) {
	
	if(!fileExists(name)) {
		return 0;
	}
	
	fileToBuffer(name, buffer);
	
	return 1;
};

int saveFile(char* name, struct buffer* buffer) {
	bufferToFile(buffer, name);
	return buffer->sig_chars;
};

uint8_t getFullName(char *fullNameBuffer, char *osName) {
	
	uint8_t osNameLen;
	uint8_t fileSlot;
	char *extraCharsPtr;
	
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

int toggleHiddenStatus(char* name) {
	ti_var_t fileSlot = ti_Open(name, "r");
	if(!fileSlot || strlen(name) > 8) {
		ti_Close(fileSlot);
		return -1;
	}
	
	char temp[9] = {0};
	ti_GetName(temp, fileSlot);
	temp[0] ^= 64;
	ti_Close(fileSlot);
	ti_Rename(name, temp);
	
	return isHidden(temp);
}

bool isHidden(char* name) {
	if(!fileExists(name))
		return false;
	return (name[0])<(65);
}
