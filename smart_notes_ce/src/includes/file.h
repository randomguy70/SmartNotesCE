#ifndef FILE_H
#define FILE_H

void archiveAll();
// detects "TXT" appvars and returns the number found
uint8_t getNumFiles(const char* txt);

// creates a new text file with a given name (an appvar with "TXT as the first 3 bytes")
uint8_t newFile(char* name);

// gives a choice whether or not to delete a file with a given name
uint8_t deleteFile(char* name);

#endif