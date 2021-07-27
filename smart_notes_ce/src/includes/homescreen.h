#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include <keypadc.h>

#include "main.h"

//// declarations

struct fileViewerStruct;


//// definitions

// contains properties of the file viewer
struct fileViewerStruct {
   char fileNames[30][9];     // array of detected text file names
   int fileSizes[30];         // array of detected text file sizes
   uint8_t numFiles;          // total number of files detected. shouldn't be more than 30
	uint8_t numFilesDisplayed; // number of files displayed on the screen at a single time (10 >= num >=0)
   uint8_t offset;            // the number of files that were shifted UP off the screen for the cursor to move more than  10 files down
   uint8_t selectedFile;      // the offset of the name of the selected file from the beginnning of the fileNames array
	bool shouldQuit; // whether or not the program should quit
	
	kb_key_t lastKeyPressed;
	int holdTime;
};

// displays homescreen and deals with all the homescreen functions and options
uint8_t dispHomeScreen(struct fileViewerStruct *HS);

#endif