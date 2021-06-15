#ifndef UI_H
#define UI_H

struct fileViewerStruct HS;
// displays homescreen and deals with all the homescreen functions and options
uint8_t dispHomeScreen();

// displays the buttons for the homescreen
void dispHSButtons();

// displays the homescreen background
void dispHomeScreenBG();

// archives appvars and quits the program
void exitFull();

// displays files at a given offset using the data in the homescreen struct (HS)
uint8_t dispFiles(struct fileViewerStruct *HS);

// check if user wants to delete a file, and deleted the selected file if so
uint8_t checkIfDelete(char *name);

// contains properties of the file viewer
struct fileViewerStruct {
   char fileNames[30][9];   // array of detected text file names
   uint8_t numFiles;        // total number of files detected. shouldn't be more than 30
   uint8_t offset;          // what is the number of files that were shifted UP off the screen for the cursor to move more than  10 files down
   uint8_t viewOffset;      // what is the offset of the cursor (and therefore selected file) from the first displayed file on the screen
   uint8_t selectedFile;    // what is the offset of the name of the selected file from the beginnning of the fileNames array
};

#endif