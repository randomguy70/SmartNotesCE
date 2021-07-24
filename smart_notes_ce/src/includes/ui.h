#ifndef UI_H
#define UI_H

#include "main.h"

#define BUTTON_TYPE_TRIGGER_ACTION  1 // Returns the value of the selected option when the button is pressed
#define BUTTON_TYPE_TRIGGER_MENU    2 // Opens a menu when the button is pressed (a menu is an array of buttons). However, there aren't recursive menus and buttons (yet).

// struct cursorStruct CS;
struct fileViewerStruct HS;
struct editor *editor;
struct fileStruct file;
struct message message;

/**
 * homescreen stuff
**/

// displays homescreen and deals with all the homescreen functions and options
uint8_t dispHomeScreen(struct fileViewerStruct *HS);

// displays the buttons for the homescreen
void dispHSButtons();

// displays the homescreen background
void dispHomeScreenBG(struct fileViewerStruct * HS);

// handles the keypresses in the fileViewer
uint8_t handleHomeScrnKeyPresses(struct fileViewerStruct *HS);

// displays files at a given offset using the data in the homescreen struct (HS)
uint8_t dispFiles(struct fileViewerStruct *HS);

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

// contains data about an opened file
struct file {
	uint8_t slot;
	char *os_name;
	char *full_name;
	int size;
	
	char *txtPtr; // pointer to the start of ascii data in the file compatable with my program
	char *txtEnd; // pointer to the end of the printable ascii data in the file
	
	int numLines; // number of lines in the file
	char *linePtr[200]; // pointers to the start of each line (for cursor and text insertion purposes)
	
};

struct editor {
	struct file file; // contains data about the opened file in the editor
	struct cursorStruct cursor; 
};

// contains the settings data, should be mostly booleans & small integers
struct settingsStruct {
	int password;
	bool autoSaveFiles;
	bool autoSaveState;
	bool showLines;
	bool cursorBlinks;
	char recents[];
};

// this is what it looks like. thank you mateo :P
struct menu_entry {
	const char * str;
	gfx_sprite_t * sprite;
	uint8_t spriteHeight;
	// void (*func_ptr)(void); 
};

// thank you mateo(again)
struct menu {
	const char * title; // title of menu
	int x, y; // x and y coord of menu window
	uint8_t numOptions;
	bool hasSprites; // whether or not the menu entries have sprites
	struct menu_entry entry[]; // contains all of the menu entries
};

struct scrollBar{
	int x;
	int y;
	uint8_t width;
	uint8_t height;
	uint8_t colorIndex;
};

// handles all editor key presses
void handleEditorKeyPresses();

// main text editing loop
// uint8_t dispEditor(struct editorStruct *editor);

// displays the background graphics for the editor, such as the title, text box, buttons, etc...
void dispEditorBK();

// prints the text in a given file starting at a given line offset
void printText(struct fileStruct * file);


///////////////////////////////////////
/**
 * cursor things
**/

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

// draws a cursor given the properties in a given cursor struct
void animateCursor(struct cursorStruct *CS);

// draws a cursor at a given x and y location
void drawCursor(struct cursorStruct * cursor);


//////////////////////////////////////////////
/**
 * text window /menu things
**/

/*
prints a message window with wordwrap
 -returns true if the user presses enter or 2nd
 -returns false if the user presses clear
*/
bool alert(char *txt);

// prints word-wrapped text inside of given boundaries
int8_t textBox(const char *text, int boxWidth, int boxHeight, int boxX, int boxY);

/** displays a menu with sprites
 * @param menu a struct containing the sprite and text data for the menu
 * */
int displayMenu(struct menu * menu);

// loads the menu for the "other" button in the homescreen fileViewer and returns its adress
static const struct menu *loadHomeScreenOtherMenu(void);


///////////////////////////////////////

/**
 * graphics routines
**/

// draws a rectangle with a given thickness
void thick_Rectangle(int x, int y, int width, int height, uint8_t thickness);

// waits for the user to press enter or second to affirm a decision, or clear to cancel
// -returns true if the user pressed enter or second
// -returns false if the user pressed clear
bool checkForInput();

// returns 1 if the user is pressing clear for a significant duration
int chooseToQuit();


#endif
