#include "main.h"
#include <fileioc.h>
#include <stdint.h>
#include <string.h>

#include "includes/file.h"
#include "includes/text.h"
#include "includes/ui.h"
#include "includes/editor.h"
#include "includes/buffer.h"

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

uint8_t loadFiles(struct file files[])
{
	uint8_t numFiles = 0;
	char *namePtr = NULL;
	void *search_pos = NULL;
	
	while ((namePtr = ti_Detect(&search_pos, HEADER_STR)) != NULL)
	{
		if (numFiles > 30)
		{
			return numFiles;
		}
		
		strcpy(files[numFiles].os_name, namePtr);
		strcpy(files[numFiles].view_name, namePtr);
		
		if(isHidden(files[numFiles].view_name))
		{
			files[numFiles].view_name[0] ^= 64;
			files[numFiles].isHidden = true;
		}
		else
		{
			files[numFiles].isHidden = false;
		}
		
		numFiles++;
		
	}
	
	return numFiles;
}

bool newFile(void)
{
	char buffer[10] = {0};
	ti_var_t fileSlot;
	
	if (inputString(buffer, 8, "New File", true) > 0)
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
		ti_Write(HEADER_STR, sizeof HEADER_STR, 1, fileSlot);
		// ti_SetArchiveStatus(true, fileSlot); // for some reason this *&*%-ing line of code crashes
		ti_Close(fileSlot);
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
	uint8_t result;
	char newNameBuffer[9] = {0};
	char message[8+9] = {"Rename "};
	
	strcat(message, name);
	
	if(inputString(newNameBuffer, 8, message, true) > 0)
	{
		result = ti_Rename(name, newNameBuffer);
		
		if(result == 1)
		{
			char message[40] = "File ";
			strcat(message, newNameBuffer);
			strcat(message, " already exists.");
			alert(message);
			return false;
		}
		if(result == 2)
		{
			char message[55] = "An error occured while trying to rename ";
			strcat(message, newNameBuffer);
			strcat(message, ".");
			alert(message);
			return false;
		}
		
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