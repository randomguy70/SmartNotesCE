#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <graphx.h>

#include "main.h"

enum color {
	BLACK = 0,
	WHITE = 1,
	TRANSPARENT = 2,
	DARK_GREY = 3,
	MEDIUM_GREY = 4,
	LIGHT_GREY = 5,
	DARK_BLUE = 6,
	RED = 7,
	LIGHT_BLUE = 8,
	PAPER_YELLOW = 9,
};

struct window {
	int x, y, width, height;
	char *title;
	char *body;
	enum color title_text_color;
	enum color body_text_color;
	enum color window_background_color;
	enum color window_outline_color;
};

enum button_types {
	return_val,  // returns a value
	action,      // executes a function
	menu,        // opens another menu
};

// contains the settings data, should be mostly booleans & small integers
struct settings {
	// user-specific
	unsigned int password;
	
	// general
	bool ask_before_save;
	bool auto_save_state;
	unsigned int undo_levels;
	
	// graphics
	bool show_line_bars;
	bool cursor_blinks;
	uint8_t cursor_width;
	enum color txtFG;
	enum color txtBG;
	enum color windowBG;
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
 * window things
**/
void drawWindow(struct window* window);

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
 * @param menu pointer to a struct containing the sprite and text data for the menu
 * */
int displayMenu(struct menu * menu);

// draws a rectangle with a given thickness
void thick_Rectangle(int x, int y, int width, int height, uint8_t thickness);

// waits for the user to press enter or second to affirm a decision, or clear to cancel
// -returns true if the user pressed enter or second
// -returns false if the user pressed clear
bool waitForInput();

#ifdef __cplusplus
}
#endif

#endif
