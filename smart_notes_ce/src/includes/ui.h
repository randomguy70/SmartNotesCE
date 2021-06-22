#ifndef UI_H
#define UI_H

struct fileViewerStruct HS;
// displays homescreen and deals with all the homescreen functions and options
uint8_t dispHomeScreen();

// displays the buttons for the homescreen
void dispHSButtons();

// displays the homescreen background
void dispHomeScreenBG();

// handles the keypresses in the fileViewer
void handleHSKeyPresses(struct fileViewerStruct *HS);

// archives appvars and quits the program
void exitFull();

// displays files at a given offset using the data in the homescreen struct (HS)
uint8_t dispFiles(struct fileViewerStruct *HS);

// contains properties of the file viewer
struct fileViewerStruct {
   char fileNames[30][9];   // array of detected text file names
   int fileSizes[30];       // array of detected text file sizes
   uint8_t numFiles;        // total number of files detected. shouldn't be more than 30
   uint8_t offset;          // what is the number of files that were shifted UP off the screen for the cursor to move more than  10 files down
   uint8_t viewOffset;      // what is the offset of the cursor (and therefore selected file) from the first displayed file on the screen
   uint8_t selectedFile;    // what is the offset of the name of the selected file from the beginnning of the fileNames array
};

// contains properties of a cursor
struct cursorStruct {
   uint8_t cursorState; // whether or not the cursor is visible (for blinking). is incremented until a certain value, and then reset based on how fast the main loop is.
   uint8_t cyclesPerAnimation; // the total number of cycles per an animation (blink)
   uint8_t invisibleTime; // how many cycles the cursor should be invisible for
   int row; // current text row the cursor is in
   int column; // current text column the cursor is in
   int x; // current x coord of cursor
   int y; // current y coord of cursor
};

// draws a cursor given the properties in a given cursor struct
void animateCursor(struct cursorStruct *CS);


#endif