#ifndef FILE_H
#define FILE_H

struct lineStruct;
void archiveAll();
// detects "TXT" appvars and returns the number found
uint8_t getNumFiles(const char* txt);

// creates a new text file with an inputted name (a text file is an appvar with TXT as the first 3 bytes, sortof like an extension a file has on a computer)
uint8_t newFile();

// contains organized file data
struct fileStruct {
   uint8_t slot; // the current slot the file is stored in
	char * txtDat; // pointer to the start of the text data in the file
	char linesArray[200][40];
	uint8_t curLine; // the current line being edited
	unsigned int numLines; // the total number of lines in the file
};

// detects all txt appvars and writes their names into a given 2d string buffer, returns the number of files found
uint8_t loadFiles(struct fileViewerStruct *HS);

// organize file into a linked list of lines with organized data
int loadFile(struct fileStruct * file);

// adds a new line to the end of the linked line list and returns the pointer to the new line struct
struct lineStruct * appendLine(struct lineStruct * end);

// check if user wants to delete a file, and deleted the selected file if so
uint8_t checkIfDelete(struct fileViewerStruct *HS );

#endif