#ifndef FILE_H
#define FILE_H

struct lineStruct;

// archives all files with "TXT" as the first 3 bytes
void archiveAll();

// detects "TXT" appvars and returns the number found
uint8_t getNumFiles(const char* txt);

// creates a new text file with an inputted name (a text file is an appvar with TXT as the first 3 bytes, sortof like an extension a file has on a computer)
uint8_t newFile(void);

// contains organized file data
struct fileStruct {
   uint8_t slot; // the current slot the file is stored in
	char * txtDat; // pointer to the start of the text data in the file
	char linesArray[200][40];
	uint16_t lineLengths[200];
	uint8_t curLine; // the current line being edited
	unsigned int numLines; // the total number of lines in the file
	unsigned int lineOffset; // the number of lines pushed up off the screen in order for the user to scroll down
};

// detects all txt appvars and writes their names into a given 2d string buffer, returns the number of files found
uint8_t loadFiles(struct fileViewerStruct *HS);

// organize file into an array of lines in a fileStruct
int loadFile(struct fileStruct * file);


#endif