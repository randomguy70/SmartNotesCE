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
   if (inputString(buffer, 8)>0) {
      file = ti_Open(buffer, "r+");
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
int loadFile(struct fileStruct * file) {

   char * readPos; // pointer to the current reading position in the file
	uint8_t loopIsDone = 0; // whether or not the while loop is finished yet. I was going to make this a bool except for the fact that I forgot the syntax and I will probably forget about this later, so don't worry if you see this because it  will (perhaps, maybe, hopefully) be corrected soon (in several years). :P P.S. It is June 25, 2021 right now when I am creating this if you are (will be) wondering.
	unsigned int curLine = 0;
	uint8_t linePos = 0; // offset in the line array where you are writing words into

	// seek to the beginning of the text data and store the pointer into both the fileStruct, if I didn't already get the pointer, and the readPos (for this while loop)	
   ti_Seek(TEXT_ORIGIN, 0, file->slot);
	if(file->txtDat==NULL) {
		file->txtDat = ti_GetDataPtr(file->slot);
	}
	readPos = file->txtDat;

	// initialize the file's 2 dim line array
	for(uint8_t i=0; i<200; i++) {
		for(uint8_t ii=0; i<40; i++) {
			file->linesArray[i][ii] = '\0';
		}
	}

   while(!loopIsDone) {
		struct wordStruct word;
		getWordLen(readPos, &word);
		file->lineLengths[curLine] = gfx_GetStringWidth(file->linesArray[curLine]);

		// if the current word is short enough to be added to the current line
      if(word.pixelLen + file->lineLengths[curLine] < 300) {
			ti_Read(&(file->linesArray[curLine][linePos]), 1, word.numChars, file->slot);
			linePos+=word.numChars;
			readPos+=word.numChars;
		}

		// if a single word is greater than a line because somebody was messing around
		if(file->lineLengths[curLine]==0 && word.pixelLen>300) {
			while(gfx_GetStringWidth(file->linesArray[curLine])<300) {
				file->linesArray[curLine][linePos] = ti_GetC(file->slot);
				linePos++;
				readPos++;
			}
			linePos = 0;
			curLine++;
		}
		
		if(word.pixelLen + file->lineLengths[curLine] > 300) {
			curLine++;
			linePos = 0;
		}
   }
	return file->numLines;
}

// gives an option whether or not to delete the selected file. i should really just make an alert function with 2-3 const char* and coordinate parameters to save space and easily create other possible messages, sortof like the wrapped text box functions with headers, body, and footers that epsilon5 has in Vysion CE.
uint8_t checkIfDelete(struct fileViewerStruct *HS ) {
	/*
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
	*/
	gfx_SetDraw(0);
	int result = alert("Delete?", 100, 100, 50, 50);
	thick_Rectangle(1, 1, 10, 10, 2);
	gfx_SwapDraw();
	if(result) {
		ti_Delete(HS->fileNames[HS->selectedFile]);
		return 1;
	}
	return 0;
}
