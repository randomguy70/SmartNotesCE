#ifndef UI_H
#define UI_H

#include "main.h"

#define BUTTON_TYPE_TRIGGER_ACTION  1 // Returns the value of the selected option when the button is pressed
#define BUTTON_TYPE_TRIGGER_MENU    2 // Opens a menu when the button is pressed (a menu is an array of buttons). However, there aren't recursive menus and buttons (yet).

//////////////////////////////////////////////
//				struct prototypes
//////////////////////////////////////////////

struct fileViewerStruct HS;
struct editor;
struct file;

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

// draws a rectangle with a given thickness
void thick_Rectangle(int x, int y, int width, int height, uint8_t thickness);

// waits for the user to press enter or second to affirm a decision, or clear to cancel
// -returns true if the user pressed enter or second
// -returns false if the user pressed clear
bool waitForInput();

#endif
