#ifndef TEXT_H
#define TEXT_H

#define MATH                 1
#define CAPS                 2
#define LOWER_CASE           3

struct fileStruct;
struct wordStruct;
struct lineStruct;

//inputString() writes an inputted string into a given string buffer (array)
uint8_t inputString(char* buffer, uint8_t maxLength, const char * title);

// inputChar() returns the last character inputted based on the value of the last keypress and text mode.
// text Modes are:
// 1) Math related ascii characters, such as {} () +-=/"?,. etc...
// 2) Capital ascii letters, such as ABC...
// 3) Lowercase ascii letters, such as abc...
uint8_t inputChar(uint8_t txtMode, uint8_t keyPressed);

// writes a string from a given location into a given location. Keeps on copying until a null byte is hit. Returns the pointer to the destination
char* strcopy(char* dest, const char* src);

// deleteChar() seeks to a given offset in an os variable (such as a program or appvar) and deletes 1 byte from that offset by moving all the data past the given offset back 1 byte and resizing the variable to be 1 byte smaller
//uint8_t deleteChar(uint8_t slot, short offset);

// returns the number of characters in a word (a string terminated with a space or null character)
int fontlib_GetStrLen(const char *string);

void fontlib_DrawStringXY(char *str, int x, int y);

// copies a null or space terminated string into a given location
int copyWord(char * dest, char * src);

// copies a string ended either by the NULL terminator OR a given number of characters
int copyChars(char *dest, char *src, int chars);

#endif