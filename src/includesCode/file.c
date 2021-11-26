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

bool newFile(void)
{
	char buffer[10] = {0};
	ti_var_t fileSlot;
	
	if (inputString(buffer, 8, "New File") > 0)
	{
		fileSlot = ti_Open(buffer, "w+");
		
		if(!fileSlot)
		{
			return false;
		}
		
		ti_Seek(0, SEEK_SET, fileSlot);
		ti_Write(HEADER_STR, sizeof HEADER_STR, 1, fileSlot);
		
		ti_Close(fileSlot);
		return true;
		
	}
	
	return false;
}

// gives an option whether or not to delete the selected file
bool checkIfDeleteFile(char *name)
{	
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
	
	char newNameBuffer[9] = {0};
	char message[8+9] = {"Rename "};
	
	strcat(message, name);
	
	if(inputString(newNameBuffer, 8, message) > 0) {
		ti_Rename(name, newNameBuffer);
		return true;
	}
	
	return false;
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
	
	char temp[10] = {0};
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
