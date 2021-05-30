#ifndef UI_H
#define UI_H

// displays homescreen and deals with all the homescreen functions and options
uint8_t dispHomeScreen();

// displays the buttons and button text on the homescreen
void dispButtons(uint8_t mode);

// archives appvars and quits the program
void exitFull();

// displays files at a given offset & writes the name of the selected file to a given string buffer
uint8_t dispFiles(uint8_t offset, uint8_t selectedNum, char* selectedName);

#endif