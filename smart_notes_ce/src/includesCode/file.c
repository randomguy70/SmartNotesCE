#include <stdint.h>
#include <fileioc.h>
#include <keypadc.h>
#include <graphx.h>
#include "includes/file.h"
#include "includes/text.h"

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
uint8_t loadFileNames(char** buffer) {
   uint8_t result=0;
   char* fileName;
   void *search_pos = NULL;
   while ((fileName=ti_Detect(&search_pos, "txt")) != NULL) {
      copyString(fileName, &buffer[result][0]);
      result++;
   }
   return result;
}

uint8_t getNumFiles(const char* txt) {
   uint8_t result = '\0';
   void *search_pos = NULL;
   while (ti_Detect(&search_pos, txt) != NULL) {
      result++;
   }
   return result;
}

uint8_t newFile(char* name) {
   ti_CloseAll();
   uint8_t file = ti_Open(name, "w+");
   ti_Write("TXT", 3, 1, file);
}

uint8_t deleteFile(char* name) {
   gfx_SetDraw(1);
   gfx_SetColor(4);
   gfx_FillRectangle_NoClip(100,90,121,40);
   gfx_SetColor(6);
   gfx_Rectangle_NoClip(99,89,123,42);
   gfx_Rectangle_NoClip(100,90,121,40);
   //text
   gfx_SetTextFGColor(6);
   gfx_PrintStringXY("Are you sure?",112,100);
   gfx_PrintStringXY("Yes=2nd  No=Mode",102,115);
   gfx_Blit(1);
   
   while (1) {
      kb_Scan();
      if (kb_IsDown(kb_2nd)) {
         ti_Delete(name);
         delay(100);
         return 1;
      }
      if (kb_IsDown(kb_Clear)) {
         return 0;
      }
   }
}

