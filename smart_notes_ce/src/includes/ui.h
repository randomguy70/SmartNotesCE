#ifndef UI_H
#define UI_H

struct cursorStruct CS;
struct fileViewerStruct HS;
struct editorStruct ES;
struct fileStruct file;
struct message message;

// displays homescreen and deals with all the homescreen functions and options
uint8_t dispHomeScreen();

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
	bool QUIT; // whether or not the program should quit
	
	kb_key_t lastKeyPressed;
	int holdTime;
};

// contains the settings data, mostly booleans
struct settingsStruct {
	uint8_t saveState;
	char lastFileOpened[9];
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

struct message {
	bool hasHeader;
	char * strings[3]; // there are up to three separate strings allowed in a message: the header, the body, and the footer.
	char * options [5]; // put the option strings here, ex: "Delete?", "Cancel", "Quit"
	/*
	Example:
	-----------------------------
	|         Alert!            |
	-----------------------------
	|  Are you sure you want    |
	|  to do that?              |
	|                           |
	|                           |
	-----------------------------
	|   Yes   |  No  |  Cancel  |
	-----------------------------
	
	"Alert" and "Are you sure you want to do that?" are the strings
	the options are "Yes", "No", and "Cancel"
	If Yes is chosen, then 0 will be returned, if No is chosen, then 1 will be returned, if Cancel is chosen, then 2 will be returned
	*/
};

struct menu {
	uint8_t numOptions; // the max number is 10
	bool hasSprites; // whether or not the menu has decorative sprites to make the gui more enjoyable
	
	gfx_sprite_t * sprites[10];
	char strings[10][15];
	
	unsigned int xMin;
	unsigned int yMin;
	unsigned int width;
	unsigned int height;
};

struct scrollBar{
	int x;
	int y;
	uint8_t width;
	uint8_t height;
	uint8_t colorIndex;
};

// contains properties of the editor
struct editorStruct {
   struct cursorStruct cursor;
   
};

// handles all editor key presses
void handleEditorKeyPresses();

/*
// main text editing loop
uint8_t dispEditor(struct editorStruct * ES);
*/

// displays the background graphics for the editor, such as the title, text box, buttons, etc...
void dispEditorBK();

// prints the text in a given file starting at a given line offset
void printText(struct fileStruct * file);

// draws a cursor given the properties in a given cursor struct
void animateCursor(struct cursorStruct *CS);

// draws a cursor at a given x and y location
void drawCursor(struct cursorStruct * cursor);


/** displays a given alert text string in a text box of given proportions. will add text wrapping (and later, scrolling if there isn't enough room in the text box)
*@param text pointer to the alert message
*@param boxLength pixel length of the text box which will display the alert message
*@param boxHeight pixel height of the text box which will display the alert message
*@param boxX the x coord of the upper left corner of the text box
*@param boxY the Y coord of the upper left corner of the text box
**/

int8_t textBox(const char *text, int boxWidth, int boxHeight, int boxX, int boxY);

int displayMessage(struct message * message);

/** displays a menu with sprites
 * @param menu a struct containing the sprite and text data for the menu
 * @param xPos x position of top right corner of menu box
 * @param yPos y position of top right corner of menu box
 * */
uint8_t displayMenu(struct menu * menu, int xPos, int yPos);

// draws a rectangle with a given thickness
void thick_Rectangle(int x, int y, int width, int height, uint8_t thickness);

// check if user wants to delete a file, and deleted the selected file if so
uint8_t checkIfDeleteSelected(struct fileViewerStruct *HS );

// renames the selected file with an inputted string
uint8_t renameSelected(struct fileViewerStruct *HS);

// returns 1 if the user is pressing clear for a significant duration
int chooseToQuit();


#endif