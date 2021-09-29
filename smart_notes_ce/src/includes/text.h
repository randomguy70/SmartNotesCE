#ifndef TEXT_H
#define TEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#define EDITOR_HEADER_BAR_HEIGHT  20
#define EDITOR_FOOTER_BAR_HEIGHT  20
#define EDITOR_TEXT_BOX_Y         EDITOR_HEADER_BAR_HEIGHT
#define EDITOR_TEXT_BOX_WIDTH     LCD_WIDTH
#define EDITOR_TEXT_BOX_HEIGHT    (LCD_HEIGHT - (EDITOR_HEADER_BAR_HEIGHT + EDITOR_FOOTER_BAR_HEIGHT))

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
	int x, y;
	
	int width, height;
	
	char *startOfText;
	int dataSize;
	int offset;
	char *readPtr;
	
	int lineOffset;
	int numLines;
	uint8_t maxLinesViewable;
	
	uint8_t fontHeight;
};

uint8_t inputString(char* buffer, uint8_t maxLength, const char * title);

// inputChar() returns the last character inputted based on the value of the last keypress and text mode.
// text Modes are:
// 1) Math related ascii characters, such as {} () */+-="?,. etc...
// 2) Capital ascii letters, such as ABC...
// 3) Lowercase ascii letters, such as abc...
char inputChar(enum textMode mode, uint8_t keyPressed);

// writes a string from a given location into a given location. Keeps on copying until a null byte is hit. Returns the pointer to the destination
char* strcopy(char* dest, const char* src);

// returns the number of characters in a word terminated with a space or null character)
int fontlib_GetStrLen(const char *string);

void fontlib_DrawStringXY(const char *str, int x, int y);

// copies a null or space terminated string into a given location
int copyWord(char * dest, char * src);

// copies a string ended by the NULL terminator, OR by a space, OR by a given number of characters
int copyWordL(char *dest, char *src, int chars);

// returns the character length of a word terminated by 0 or a space
int getWordLen(char *src);

// calculates the maximum characters that would be allowed in a fontlib line
int getMaxCharsPerLine(char *src);

int getByteDifference(void *ptrOne, void *ptrTwo);

int drawSpace();

void updateInputMode(struct inputState *inputState);
void displayTextMode(int x, int y, enum textMode textMode);

int calculateEditorLinePointers(struct textBox *textBox);

#ifdef __cplusplus
}
#endif

#endif