#include "main.h"

void archiveAll()
{
   uint8_t fileSlot;
   char * fileName;
   void *search_pos = NULL;
   
   ti_CloseAll();
   while (((fileName = ti_Detect(search_pos, "TXT")) != NULL)) {
      fileSlot = ti_Open(fileName, "r");
      ti_SetArchiveStatus(1, fileSlot);
   }
   ti_CloseAll();
}

uint8_t getNumFiles(const char* txt) {
   uint8_t result = '\0';
   void *search_pos = NULL;
   while (ti_Detect(&search_pos, txt) != NULL) {
      result++;
   }
   return result;
}

// asks for user to input a string and makes a new file if one doesn't already exist with that name
uint8_t newFile() {
   char buffer[9] = {0};
   uint8_t file = 0;
   ti_CloseAll();
   if (inputString(buffer, 8)>0){
      file = ti_Open(buffer, "r+");
      if(!file) {
         file = ti_Open(buffer, "w+");
      }
      ti_Write("TXT", 3, 1, file);
      return 1;
   }
   return 0;
}

uint8_t loadFiles(struct fileViewerStruct *HS) {
	uint8_t numFiles = 0;
	uint8_t fileSlot;
	char *namePtr = NULL;
	void *search_pos = NULL;

	while ((namePtr = ti_Detect(&search_pos, "TXT")) != NULL) {
		strcpy(HS->fileNames[numFiles], namePtr);
		fileSlot = ti_Open(namePtr, "r");
		ti_SetArchiveStatus(fileSlot, 1);
		if(ti_GetSize(fileSlot)<10) {
		   ti_Resize(10, fileSlot);
		}
		HS->fileSizes[numFiles] = ti_GetSize(fileSlot);
		numFiles++;
	}
	HS->numFiles=numFiles;
	return numFiles;
}

// formats the raw character data in the text array into an organized structure (hence the struct...obviously)
int loadFile(struct fileStruct * file) {

   char * readPos = NULL; // pointer to the current reading position in the file
   unsigned int numChars = ti_GetSize(slot)-10;
	uint8_t loopIsDone = 0;
	struct lineStruct curLinePtr = NULL;
	unsigned int curLine = 0;

	// seek to the beginning of the text data and store the pointer into both the fileStruct (for later usage if necessary), and the readPos (for this while loop)
   ti_Seek(TEXT_ORIGIN, 0, slot);
   file->txtDat = ti_GetDataPtr(slot);
	readPos = file->txtDat;

	// create a pointer to the current line struct, will update this each time i create a new line
	struct lineStruct * linePtr;

	// create a starting line as the beginning of the linked list
	struct lineStruct line;

	// initialize the first line's string array
	for(uint8_t i=0; i<40; i++) {
		line.line[i] = '\0';
	}

   while(!loopIsDone) {
		struct wordStruct word;
		getWordLen(readPos, &word);
      if(word.pixelLen+curLineLen) {

		}
		curLine++;
   }
	return file->numLines;
}

struct lineStruct * appendLine(struct lineStruct * end) {
	struct lineStruct * new;
	new = malloc(sizeof(struct lineStruct));
	end->next=new;
	new->next=NULL;
	// initialize & empty the line's string
	for(uint8_t i=0; i<40; i++) {
		new->line[i] = '\0';
	}
	return new;
}

void deleteAllLines(struct lineStruct * start) {
	struct lineStruct * ptr = start;
	while(ptr!=NULL {
		start = ptr;
		ptr = ptr->next;
		free(start);
	}
}

// gives an option whether or not to delete the selected file. i should really just make an alert function with 2-3 const char* and coordinate parameters to save space and easily create other possible messages, sortof like the wrapped text box functions with headers, body, and footers that epsilon5 has in Vysion CE.
uint8_t checkIfDelete(struct fileViewerStruct *HS ) {
	gfx_SetDraw(0);
	gfx_SetColor(LIGHTER_BLUE);
	gfx_FillRectangle_NoClip(100,90,121,40); // main rect
	gfx_SetColor(RED);
	gfx_Rectangle_NoClip(100,90,121,40);// outline rect
	gfx_Rectangle_NoClip(101,91,119,38);// outline rect
	gfx_SetTextFGColor(RED);
	gfx_PrintStringXY("Are you sure?",112,100);
	gfx_PrintStringXY("Yes=2nd  No=Clear",102,115);
	uint8_t result = 0;
	uint8_t keyPressed = 0;
	while (keyPressed!=sk_2nd && keyPressed!=sk_Clear) {
		keyPressed = os_GetCSC();
		if (keyPressed==sk_2nd) {
		   ti_Delete(HS->fileNames[HS->selectedFile]);
		   result = 1;;
		}
		if (keyPressed==sk_Clear) {
		   result = 2;
		   delay(100);
		}
	}
	if(result==1) {
	   return 1;
	}else {
	   return 0;
	}
}
