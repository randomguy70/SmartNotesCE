#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <graphx.h>

#include "main.h"
#include "includes/colors.h"


#define WINDOW_TITLE_BAR_HEIGHT  10
#define WINDOW_BORDER_THICKNESS   2

#define MENU_ENTRY_SPACING       20
#define MAX_MENU_ENTRIES_VISIBLE  7

enum window_type {
	DISPLAY_TEXT_TYPE,
	OPTION_TYPE,
};

enum button_types {
	return_val,  // returns a value
	action,      // executes a function
	menu,        // opens another menu
};

struct window {
	enum window_type type;
	
	int x, y, width, height;
	const char *title;
	char *body;
	
	enum color titleBarColor;
	enum color titleTextColor;
	enum color bodyColor;
	enum color bodyTextColor;
	enum color windowOutlineColor;
	
	bool hasOptions;
	char optionText[5][10];
	uint8_t numOptions;
	uint8_t selectedOption;
};

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

struct menu_entry {
	const char *string;
	gfx_sprite_t *sprite;
};

// thank you mateo(again)
struct menu {
	const char *title;
	int x, y, width;
	uint8_t numOptions;
	uint8_t selected;
	uint8_t offset;
	bool hasSprites;
	struct menu_entry entry[];
};

struct scrollBar{
	int x;
	int y;
	uint8_t width;
	uint8_t height;
	uint8_t colorIndex;
};

struct cursor {
	uint8_t animation_cycles_completed;
	uint8_t cycles_per_animation;
	uint8_t invisibleTime; // how many cycles of the animation time the cursor should be invisible for
	unsigned int row; // current text row the cursor is in
	unsigned int column; // current text column the cursor is in
	unsigned int x; // current x coord of cursor
	unsigned int y; // current y coord of cursor
};

void drawWindow(struct window* window);

void updateCursor(struct cursor *cursor);

void drawCursor(struct cursor * cursor);

/*
prints a message window with wordwrap
 -returns true if the user presses enter or 2nd
 -returns false if the user presses clear
*/
bool alert(char *txt);

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
