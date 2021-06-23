#ifndef UI_H
#define UI_H

struct cursorStruct CS;
struct fileViewerStruct HS;
struct editorStruct ES;
// displays homescreen and deals with all the homescreen functions and options
uint8_t dispHomeScreen();

// displays the buttons for the homescreen
void dispHSButtons();

// displays the homescreen background
void dispHomeScreenBG();

// handles the keypresses in the fileViewer
void handleHSKeyPresses(struct fileViewerStruct *HS);

// displays files at a given offset using the data in the homescreen struct (HS)
uint8_t dispFiles(struct fileViewerStruct *HS);

// contains properties of the file viewer
struct fileViewerStruct {
   char fileNames[30][9];   // array of detected text file names
   int fileSizes[30];       // array of detected text file sizes
   uint8_t numFiles;        // total number of files detected. shouldn't be more than 30
   uint8_t offset;          // what is the number of files that were shifted UP off the screen for the cursor to move more than  10 files down
   uint8_t selectedFile;    // what is the offset of the name of the selected file from the beginnning of the fileNames array
};

// contains properties of a cursor
struct cursorStruct {
   uint8_t cursorState; // number of cycles completed so far in 1 animation. Is incremented until it is == cyclesPerAnimation, and then reset.
   uint8_t cyclesPerAnimation; // the total number of cycles that should be completed per an animation (blink), also called the speed :P
   uint8_t invisibleTime; // how many cycles the cursor should be invisible for
   int row; // current text row the cursor is in
   int column; // current text column the cursor is in
   int x; // current x coord of cursor
   int y; // current y coord of cursor
};

// contains properties of the editor
struct editorStruct {
   struct cursorStruct cursor;
   uint8_t fileSlot; // slot of open file
   
};

void handleEditorKeyPresses(struct editorStruct* ES);

uint8_t dispEditorBK();

// draws a cursor given the properties in a given cursor struct
void animateCursor(struct cursorStruct *CS);

// draws a cursor at a given x and y location
void drawCursor(int x, int y);
#endif