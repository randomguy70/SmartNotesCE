#ifndef MAIN_H
#define MAIN_H

// standard includes. i don't want to have to type them out at the beginning of every last .c file, so i just put include main.h instead. I don't care what MateoC says if he comes with some obscure rule about includes that says i am in error :P....
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#include <tice.h>
#include <string.h>

#include <keypadc.h>
#include <compression.h>
#include <graphx.h>
#include <fileioc.h>
#include <fontlibc.h>

// my includes
#include "gfx/gfx.h"
#include "includes/key.h"
#include "includes/ui.h"
#include "includes/text.h"
#include "includes/file.h"
#include "includes/specialChars.h"
#include "includes/homescreen.h"
#include "includes/editor.h"

// defines
#define VERSION 1
#define RELEASE_STRING "V.1.0"
#define AUTHOR "Randomguy"

#define SCRN_WIDTH    320
#define SCRN_HEIGHT   240

// text editing specific things
#define TXT_STR "TXT" // bytes written to the beginning of a text file to distinguish them from other files
#define START_OF_TEXT  50  // Offset of the text data in files; in other words, the number of bytes at the beginning of the file to ignore.
#define MIN_FILE_SIZE (3+START_OF_TEXT)

#define BLACK           0  // black
#define WHITE           1  // white
#define TRANSPARENT     2  // gfx transparent color (would otherwise be hot pink :P)
#define DARK_GREY       3  // dark grey
#define MEDIUM_GREY     4  // medium grey
#define LIGHT_GREY      5  // light grey
#define DARK_BLUE       6  // electric blue. I was too lazy to change the name...
#define RED             7  // red
#define LIGHT_BLUE      8  // light blue
#define PAPER_YELLOW    9  // color of yellow paper stickypads
  
#define HOME            1  // Integer return signifying a request to go to the homescreen (file viwer)
#define QUIT            0  // Integer return signifying a quit request.
#define CANCEL        100  // Integer return signifying a cancel / close window request. NOT the same as QUIT.
#define OPEN          101  // Integer return signigying a file open request. Should lead to a given file being opened in the editor.
#define CLOSE         102  // Integer return signigying request to close the file currently opened in the editor. Should reset the editor variables and either quit the program or open the file menu.
#define BACK          103  // Integer return signifying a request to go back 1 scope level / return level

struct window {
	uint8_t fillIndex;
	uint8_t outlineIndex;
};
struct homeScrn {
	uint8_t BGFillIndex;
	bool hasLines;
	uint8_t lineIndex;
	
};
struct editorScrn {
	uint8_t BGFillIndex;
	bool hasLines;
	uint8_t lineIndex;
};
struct buttons{
	uint8_t fillIndex;
	uint8_t outlineIndex;
	uint8_t pressedFillIndex;
	uint8_t pressedOutlineIndex;
};

struct globalPalette{
	uint8_t cursorIndex;
	
	struct window window;
	struct homeScrn homeScrn;
	struct editorScrn editorScrn;
	struct buttons buttons;
};

#define MATH          1
#define CAPS          2
#define LOWER_CASE    3

#define TEXT_ORIGIN   10 // the text in a file begins at offset of 10 bytes from the beginning

#endif
