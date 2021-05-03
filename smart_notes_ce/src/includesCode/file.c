#include <stdint.h>
#include <fileioc.h>
#include "includes/file.h"

void archiveAll() 
{
   uint8_t fileSlot;
   char * fileName;
   int* search_pos = NULL;
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

