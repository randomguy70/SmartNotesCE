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

#define SAVE_STATE_APPVAR_NAME "SMNTSSV"
#define SETTINGS_APPVAR_NAME   "SMNTSTNG"
#define USER_INFO_APPVAR_NAME  "SMNTUI"
	
// text editing specific things
#define HEADER_STR           "TXT"                // header string of all text files
#define TEXT_OFFSET          50                   // Offset of the text data in files
#define MIN_FILE_SIZE        TEXT_OFFSET
#define MAX_FILE_NAME_SIZE   18

#define FILE_BUFFER_SIZE     50000

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
	ti_var_t slot;
	char os_name[10];    // os name of the file
	char full_name[19];  // full name of the file, can be up to 18 digits (different from os name!!!)
	size_t size;
};

struct buffer {
	char data[FILE_BUFFER_SIZE];
	size_t sig_chars;
	uint16_t numLines;
	char lines[200];
};

// check if user wants to delete a file, and deleted the selected file if so
bool checkIfDeleteFile(char *name);

// renames a file with an inputted string
bool renameFile(const char *name);

// get a file's data (usually before opening it)
int loadFile(struct file *file, char *name);

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