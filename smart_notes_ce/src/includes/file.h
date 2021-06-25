#ifndef FILE_H
#define FILE_H

struct lineStruct;
void archiveAll();
// detects "TXT" appvars and returns the number found
uint8_t getNumFiles(const char* txt);

// creates a new text file with an inputted name (a text file is an appvar with TXT as the first 3 bytes, sortof like an extension a file has on a computer)
uint8_t newFile();

// contains organized file metadata
struct fileStruct{
   uint8_t slot; // the current slot the file is stored in
	char * txtDat; // pointer to the start of the text data in the file
	struct lineStruct * firstLinePtr; // pointer to the first line. since the lines form a linked list, all lines can be accessed through this.
	struct lineStruct * linePtrs[200]; // contains pointers to all of the lines, in case you need to access them without going through all the *next pointers. only 200 lines are allowed as of now, & that takes up 200*3 bytes in mem by itself
	uint8_t curLine; // the current line being edited
	unsigned int numLines; // the total number of lines in the file
};

// detects all txt appvars and writes their names into a given 2d string buffer, returns the number of files found
uint8_t loadFiles(struct fileViewerStruct *HS);

// organize file into a linked list of lines with organized data
int loadFile(struct fileStruct * file, uint8_t slot);

// adds a new line to the end of the linked line list and returns the pointer to the new line struct
struct lineStruct * appendLine(struct lineStruct * end);

// deletes all lines in the linked list
void deleteAllLines(struct lineStruct * start);

// check if user wants to delete a file, and deleted the selected file if so
uint8_t checkIfDelete(struct fileViewerStruct *HS );

#endif