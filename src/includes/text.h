#ifndef TEXT_H
#define TEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "includes/buffer.h"

struct word
{
	char *ptr;
	int len;
	int width;
	char stopCode;
};

enum textMode
{
	MATH = 1,
	CAPS,
	LOWER_CASE,
};

struct inputState
{
	enum textMode textMode;
	bool alphaPrev;
};

struct textBox
{
	int x, y, width, height;
	
	int curLine, curCol;
	
	int numLinesOnScreen;
	int lineOffset;
	
	uint8_t prevLinePtr;
	uint8_t prevLineLen;
	uint8_t visiblelineLengths[11];
};

/** Opens a window for entering text
 * @param buffer buffer to store inputted string in. Allocate at least (maxLength + 1) bytes to prevent overflow!
 * @param maxLength the maximum number of characters allowed to be inputted.
 * @param title a short string used as a title at the top of the window (don't do more than 15 characters)
 * @param restrictFirstChar used for inputting file names because TI-OS doesn't like names to begin with certain characters
**/
uint8_t inputString(char* buffer, uint8_t maxLength, const char * title, bool restrictFirstChar);

/** returns the ASCII character value converted from an os_GetCSC() value
 * @param mode The text mode (Numbers, Capital letters, Lowercase, etc...)
 * @param keyPressed The os_CSC key value to change into ASCII value
**/
char inputChar(enum textMode mode, uint8_t keyPressed);

void updateInputMode(struct inputState *inputState);
void displayTextMode(int x, int y, enum textMode textMode);

int textBox_getVisibleLinePointers(struct textBox *textBox);

bool isValidWordChar(char character);

struct word getWord(char *ptr, struct fileBuffer *buffer);
int getWordLen(char *start);
int getLineLen(char *start, int maxWidth);

void fontlib_DrawStringXY(const char *str, int x, int y);
void fontlib_DrawStringLXY(const char *str, int length, int x, int y);
int drawLine(char *start, int x, int y);

// returns num characters with NO word wrapping
int getMaxCharsPerLine(char *start, char *end);

#ifdef __cplusplus
}
#endif

#endif