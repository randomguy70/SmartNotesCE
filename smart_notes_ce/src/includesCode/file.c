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

uint8_t loadFiles(struct fileViewerStruct *HS) {
   uint8_t numFiles = 0;
   uint8_t fileSlot = 0; // slot of currently detected file
   char * namePtr = NULL;
   void * search_pos = NULL; // mem location of the currently detected file in the VAT
	
	ti_CloseAll();
	
   while ((namePtr = ti_Detect(&search_pos, "TXT")) != NULL) {
		
		// copy the currently detected file's name into the fileviewer struct's names array
      strcpy(HS->fileNames[numFiles], namePtr);
		
		//get some info from the currently detected file
      fileSlot = ti_Open(namePtr, "r+");
		
      HS->fileSizes[numFiles] = ti_GetSize(fileSlot);
		
		// files have to be at least 10 bytes large for future formatting data purposes
		if(HS->fileSizes[numFiles] < 10) {
			ti_Seek(3, 0, fileSlot);
			ti_Write((const void *)0xE40000, 7, 1, fileSlot);
		}
		
		// "always close files after opening them" -Jacobly, ergo...
		ti_Close(fileSlot);
      numFiles++;
   }
	
	ti_CloseAll();
	
   HS->numFiles = numFiles;
   return numFiles;
}

// formats the raw file data into an organized structure (hence the struct...obviously)
// P.S. I will completely change function this very soon, because it is horrible code and I am actually going to switch to the greedy method with displaying text.
int loadFile(struct file *file) {
	
	// reset editing variables
	file->curCol = 0;
	file->curLine = 0;
	file->lineDisplayOffset = 0;
	
	ti_CloseAll();
	file->slot = ti_Open(file->os_name, "r+");
	if(!file->slot)
		return 0;
	
	// load the 
};

int getLinePtrs(struct file *file) {
	
};