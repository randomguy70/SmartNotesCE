#include <stdint.h>
#include <fileioc.h>
#include "includes/file.h"

void archiveAll()
{
   uint8_t fileSlot;
   char * fileName;
   void* search_pos = NULL;
   while (((fileName = ti_Detect(search_pos, "TXT")) != NULL)) {
      fileSlot = ti_Open(fileName, "r");
      ti_SetArchiveStatus(1, fileSlot);
   }
   fileSlot = ti_Open("MATH", "r");
   ti_SetArchiveStatus(1, fileSlot);
   fileSlot = ti_Open("CAPS", "r");
   ti_SetArchiveStatus(1, fileSlot);
   ti_CloseAll();
}

// write the names of all detected files starting with a given string into a given 2d buffer array and return the number of files detected
uint8_t loadFileNames(const char* txt, char** buffer) {
   uint8_t result=0;
   uint8_t i;
   char* fileName;
   void *search_pos = NULL;
   while ((fileName=ti_Detect(&search_pos, txt)) != NULL) {
      // delete previous contents of each string
      for(i=0; i<9; i++) {
         buffer[result][i] = 0;
      }
      buffer[result][0]=fileName;
      result++;
   }
   return result;
}

uint8_t getNumFiles(const char* txt) {
   uint8_t result;
   uint8_t i;
   void *search_pos = NULL;
   while (ti_Detect(&search_pos, txt) != NULL) {
      result++;
   }
   return result;
}