#ifndef TEXT_H
#define TEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "includes/buffer.h"

enum textMode {
	MATH = 1,
	CAPS,
	LOWER_CASE,
};

struct inputState {
	enum textMode textMode;
	bool alphaPrev;
};

struct textBox {
	int x, y, width, height;
	
	int curLine, curCol;
	
	int numLinesOnScreen;
	int lineOffset;
	
	uint8_t prevLinePtr;
	uint8_t prevLineLen;
	uint8_t visiblelineLengths[11];
};

uint8_t inputString(char* buffer, uint8_t maxLength, const char * title);

// inputChar() returns the last character inputted based on the value of the current os_GetSCS() value, and the current text mode.
// text Modes are:
// 1) Math related ascii characters, such as {} () */+-="?,. etc...
// 2) Capital ascii letters, such as ABC...
// 3) Lowercase ascii letters, such as abc...
char inputChar(enum textMode mode, uint8_t keyPressed);

// returns the number of characters in a word terminated with a null character or alternate stop code
int fontlib_GetStrLen(const char *string);

void fontlib_DrawStringXY(const char *str, int x, int y);
int fontlib_copyWord(char * dest, char * src);
int fontlib_copyWordL(char *dest, char *src, int chars);
int fontlib_strlen(char *string);

void updateInputMode(struct inputState *inputState);
void displayTextMode(int x, int y, enum textMode textMode);

int textBox_getVisibleLinePointers(struct textBox *textBox);

bool isValidWordChar(char character);

int getWordLen(char *start);
int getLineLen(char *start);

// returns num characters with NO word wrapping
int getMaxCharsPerLine(char *start, char *end);

#ifdef __cplusplus
}
#endif

#endif