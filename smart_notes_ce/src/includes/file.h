#ifndef FILE_H
#define FILE_H

void archiveAll();
uint8_t getNumFiles(const char* txt);
uint8_t loadFileNames(const char* txt, char** buffer);

#endif