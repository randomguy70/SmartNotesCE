#include "main.h"

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

// formats the file data into an organized structure (hence the struct)
int loadFile(struct file *file, char *name) {
	
	strcpy(file->os_name, name);
	
	// reset editing variables
	file->curCol = 0;
	file->curLine = 0;
	file->lineDisplayOffset = 0;
	
	ti_CloseAll();
	file->slot = ti_Open(name, "r+");
	if(!file->slot)
		return false;
	
	// get its full name the extra characters of the full name(if there are any) are stored as a string starting at the 4th byte in the file). ps, haven't added full name (yet)
	
	// get its size
	file->size = ti_GetSize(file->slot);
	
	// get some pointers
	file->txtPtr = ti_GetDataPtr(file->slot) + MIN_FILE_SIZE;
	
	return 1;
};

int getLinePtrs(struct file *file) {
	
};