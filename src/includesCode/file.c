#include "main.h"
#include <fileioc.h>
#include <stdint.h>
#include <string.h>

#include <includes/file.h>
#include <includes/text.h>
#include <includes/ui.h>
#include <includes/editor.h>
#include <includes/buffer.h>

uint8_t getNumFiles(const char * txt)
{
	uint8_t result = 0;
	void * pos = NULL;
	
	while (ti_Detect(&pos, txt) != NULL)
	{
		result++;
	}
	
	return result;
}

bool newFile(void)
{
	char buffer[10] = {0};
	ti_var_t fileSlot;
	
	if (inputString(buffer, 8, "New File") > 0)
	{
		
		if(fileExists(buffer))
		{
			alert("File already exists.");
			return false;
		}
		
		fileSlot = ti_Open(buffer, "w+");
		
		if(!fileSlot)
		{
			return false;
		}
		
		ti_Seek(0, SEEK_SET, fileSlot);
		ti_Write(HEADER_STR, 3, 1, fileSlot);
		ti_SetArchiveStatus(true, fileSlot);
		ti_Close(fileSlot);
		
		alert("wrote to file");
		
		return true;
	}
	
	return false;
}

bool checkIfDeleteFile(char *name)
{	
	char message[100] = {"Are you sure you want to delete "};
	strcat(message, name);
	strcat(message, "?");
	
	if(alert(message) == true)
	{
		if(ti_Delete(name) != 0)
		{
			return true;
		}
		else
		{
			alert("Error encountered.");
			return false;
		}
	}
	
	return false;
}

bool renameFile(const char *name)
{
	
	char newNameBuffer[9] = {0};
	char message[8+9] = {"Rename "};
	
	strcat(message, name);
	
	if(inputString(newNameBuffer, 8, message) > 0)
	{
		ti_Rename(name, newNameBuffer);
		return true;
	}
	
	return false;
}

bool fileExists(char* name)
{
	ti_var_t slot = ti_Open(name, "r");
		
	if(!slot)
	{
		return false;
	}
	
	ti_Close(slot);
	
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

void archiveAll(void)
{
	void *searchPtr = NULL;
	char *namePtr = NULL;
	ti_var_t fileSlot;
	
	while ((namePtr = ti_Detect(&searchPtr, HEADER_STR)) != NULL)
	{
		fileSlot = ti_Open(namePtr, "r");
		
		// I really don't trust fileioc, so extra precautions...
		if(!fileSlot)
		{
			return;
		}
		
		if (ti_SetArchiveStatus(true, fileSlot))
		{
			ti_Close(fileSlot);
		}
		else
		{
			return;
		}
	}
}
