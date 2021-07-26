#ifndef FILE_H
#define FILE_H

struct lineStruct;
struct file;

// archives all files with "TXT" as the first 3 bytes
void archiveAll();

// detects "TXT" appvars and returns the number found
uint8_t getNumFiles(const char *txt);

// creates a new text file with an inputted name (a text file is an appvar with TXT as the first 3 bytes, sortof like an extension a file has on a computer)
uint8_t newFile(void);

struct file {
	// general information
	
	uint8_t slot;
	char os_name[10];
	char *full_name;
	int size;
	
	// window display information
	
	int lineDisplayOffset; // how many lines have been shifted off the screen so that the user could scroll down
	
	// text-specific information
	
	char *txtPtr; // pointer to the start of ascii data in the file compatable with my program
	char *txtEnd; // pointer to the end of the printable ascii data in the file
	
	int numLines; // number of lines in the file
	char *linePtrs[200]; // pointers to the start of each line (for cursor and text insertion purposes)
	
	// editing specific information
	
	char *editPos;
	int selectedChars; // number of characters highlighted / selected. default is 0
	int curLine; // line that cursor is on
	int curCol; // column that cursor is after
};

// check if user wants to delete a file, and deleted the selected file if so
bool checkIfDeleteFile(char *name);

// renames a file with an inputted string
bool renameFile(const char *name);

// get a file's data (usually before opening it)
int loadFile(struct file *file, char *name);

// gets the pointers to the start of each line in the file, as well as their byte offset from the text origin
int getLinePtrs(struct file *file);

#endif