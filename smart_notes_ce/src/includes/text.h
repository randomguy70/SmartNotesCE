#ifndef TEXT_H
#define TEXT_H

//inputString() writes an inputted string into a given string buffer (array)
uint8_t inputString(char* buffer, uint8_t maxLength);

// inputChar() returns the last character inputted based on the value of the last keypress and text mode. to use this function you need to have my 3 ascii data appvars (mathasci.8xp, letteras.8xp, and lowerasc.8xp) on-calc
//text Modes are:
// 1) math related ascii characters, such as {}, (), +-=/"?,. etc...
// 2) capital ascii letters, such as ABC...
// 3) lowercase ascii letters, such as abc...
uint8_t inputChar(uint8_t txtMode, uint8_t keyPressed);

// writes a string from a given location into a given location. Keeps on copying until a null byte is hit. Eeturns the number of characters copied
uint8_t copyString(char* inputStr, char* outputBuffer);

// deleteChar() seeks to a given offset in an os variable (such as a program or appvar) and deletes 1 byte from that offset by moving all the data past the given offset back 1 byte and resizing the variable to be 1 byte smaller
//uint8_t deleteChar(uint8_t slot, short offset);

#endif