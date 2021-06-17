#ifndef FILE_H
#define FILE_H

void archiveAll();
// detects "TXT" appvars and returns the number found
uint8_t getNumFiles(const char* txt);

// creates a new text file with an inputted name (a text file is an appvar with TXT as the first 3 bytes, sortof like an extension a file has on a computer)
uint8_t newFile();

// contains organized file metadata
struct fileStruct{
   uint8_t slot; // the current slot the file is stored in
   int characters; // the total number of characters in the file
   int numLines; // the total number of lines in the file
   int lineOffsets[200]; // array containing the offsets of the beginning of each line. for now, there can only be 200 lines per file
};

// detects all txt appvars and writes their names into a given 2d string buffer, returns the number of files found
uint8_t loadFiles(struct fileViewerStruct *HS);

// check if user wants to delete a file, and deleted the selected file if so
uint8_t checkIfDelete(struct fileViewerStruct *HS );

#endif