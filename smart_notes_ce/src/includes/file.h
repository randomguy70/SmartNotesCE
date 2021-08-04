#ifndef FILE_H
#define FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/* -------------- file byte format -------------- 
bytes 1-3: "TXT"
bytes 4-14: reserved for extra characters in the file's name
bytes 15-50: reserved for other data (not decided what yet, but probably will use them in the future)
bytes 50->end of the file: text data
*/

// text editing specific things
#define TXT_STR "TXT" // bytes written to the beginning of a text file to distinguish them from other files
#define START_OF_TEXT  50  // Offset of the text data in files; in other words, the number of bytes at the beginning of the file to ignore.
#define MIN_FILE_SIZE (3+START_OF_TEXT)
#define MAX_FILE_NAME_SIZE 18

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
	
	// general info
	
	uint8_t slot;
	char os_name[10];    // os name of the file
	char full_name[19];  // full name of the file, can be up to 18 digits (different from os name!!!)
	int size;            // byte size of the file BEFORE its data was copied into an array and edited
	
	// text-specific info
	
	char *buffer;        // array of the text data copied from the file for faster editing
	int charsInBuffer;   // number of text characters stored in the buffer
	int bufferSize;      // the current byte size of the buffer
	
	char *txtStart;      // pointer to the start of ascii data in the open file (not in the buffer)
	char *txtEnd;        // pointer to the end of the printable ascii data in the open file (not in the buffer)
	
	int numLines;        // number of lines in the file
	char *linePtrs[200]; // Pointers to the start of each line (these lines are all contained in an array, not a file)
	
};

// check if user wants to delete a file, and deleted the selected file if so
bool checkIfDeleteFile(char *name);

// renames a file with an inputted string
bool renameFile(const char *name);

// get a file's data (usually before opening it)
int loadFile(struct file *file, const char *name);

// copies the data in the buffer into the actual file itself
int saveFile(struct file *file);

// frees the mem used by the given file struct
bool closeFile(struct file *file);

// Copies the data from a file into an array. The array should always be the maximum possible size of a file
int fileToArray(const char *name, char *array);

// copies the data from an array into a file
int arrayToFile(char *array, const char *name, int bytes);

// copies 10 characters or a string from the given file, appending that on to its os name. starts copying from an offset of 3 bytes in the file's data
uint8_t getFullName(char *fullNameBuffer, char *osName);

#ifdef __cplusplus
}
#endif

#endif