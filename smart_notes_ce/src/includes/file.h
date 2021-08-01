#ifndef FILE_H
#define FILE_H

/* -------------- txt file byte format -------------- 



*/

// text editing specific things
#define TXT_STR "TXT" // bytes written to the beginning of a text file to distinguish them from other files
#define START_OF_TEXT  50  // Offset of the text data in files; in other words, the number of bytes at the beginning of the file to ignore.
#define MIN_FILE_SIZE (3+START_OF_TEXT)

struct lineStruct;
struct file;

// archives all files with "TXT" as the first 3 bytes
void archiveAll();

// detects "TXT" appvars and returns the number found
uint8_t getNumFiles(const char *txt);

// creates a new text file with an inputted name (a text file is an appvar with TXT as the first 3 bytes, sortof like an extension a file has on a computer)
uint8_t newFile(void);

// deletes the appvar with the given name
bool checkIfDeleteFile(char *name);

// renames a file with an inputted name
bool renameFile(const char *name);

// contains data about a certain (open) file
struct file {
	// general information
	
	uint8_t slot;
	char *os_name;
	char *full_name;
	int size;
	
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

// Copies the data from a file into an array. The array should always be the maximum possible size of a file
int fileToArray(const char *name, char *array);

// copies the data from an array into a file
int arrayToFile(char *array, const char *name, int bytes);

#endif