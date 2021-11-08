#ifndef TEXT_H
#define TEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#define EDITOR_MAX_LINES_VIEWABLE 15
#define EDITOR_HEADER_BAR_HEIGHT  20
#define EDITOR_FOOTER_BAR_HEIGHT  20
#define EDITOR_TEXT_BOX_Y         EDITOR_HEADER_BAR_HEIGHT
#define EDITOR_TEXT_BOX_WIDTH     LCD_WIDTH
#define EDITOR_TEXT_BOX_HEIGHT    LCD_HEIGHT - (EDITOR_HEADER_BAR_HEIGHT + EDITOR_FOOTER_BAR_HEIGHT)

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
	
	char *startOfText;
	int textLength;
	
	int numLines;
	int numLinesOnScreen;
	int maxLinesOnScreen;
	int lineOffset;
	
	// holds the pointers to the lines on screen
	char *linePointers[EDITOR_MAX_LINES_VIEWABLE];
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

int drawSpace();

void updateInputMode(struct inputState *inputState);
void displayTextMode(int x, int y, enum textMode textMode);

int textBox_getVisibleLinePointers(struct textBox *textBox);

// returns character of next word terminated by space , NULL, or a new line
int getWordLen(char *word, char *end);

// returns num characters with NO word wrapping
int getMaxCharsPerLine(char *start, char *end);

#ifdef __cplusplus
}
#endif

#endif