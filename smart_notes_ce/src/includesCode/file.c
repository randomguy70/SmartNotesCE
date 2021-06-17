#include "main.h"

void archiveAll()
{
   uint8_t fileSlot;
   char * fileName;
   uint8_t *search_pos = NULL;
   
   ti_CloseAll();
   while (((fileName = ti_Detect(search_pos, "TXT")) != NULL)) {
      fileSlot = ti_Open(fileName, "r");
      ti_SetArchiveStatus(1, fileSlot);
   }
   ti_CloseAll();
}

uint8_t getNumFiles(const char* txt) {
   uint8_t result = '\0';
   uint8_t *search_pos = NULL;
   while (ti_Detect(&search_pos, txt) != NULL) {
      result++;
   }
   return result;
}

uint8_t newFile() {
   char buffer[9] = {0};
   if(inputString(buffer, 8) > 0) {
      ti_CloseAll();
      uint8_t file = ti_Open(buffer, "w+");
      ti_Write("TXT", 3, 1, file);
      return 1;
   }
   else{
      return 0;
   }
}

uint8_t loadFiles(struct fileViewerStruct *HS) {
   uint8_t numFiles = 0;
   uint8_t fileSlot;
   char *namePtr = NULL;
   uint8_t *search_pos = NULL;

   while ((namePtr = ti_Detect(&search_pos, "TXT")) != NULL) {
      strcpy(HS->fileNames[numFiles], namePtr);
      fileSlot = ti_Open(namePtr, "r+");
      ti_SetArchiveStatus(fileSlot, 1);
      HS->fileSizes[numFiles] = ti_GetSize(fileSlot);
      numFiles++;
   }
   HS->numFiles=numFiles;
   return numFiles;
}

uint8_t checkIfDelete(struct fileViewerStruct *HS ) {
   gfx_SetDraw(0);
   gfx_SetColor(DARK_BLUE);
   gfx_FillRectangle_NoClip(100,90,121,40); // main rect
   gfx_SetColor(RED);
   gfx_Rectangle_NoClip(100,90,121,40);// outline rect
   gfx_Rectangle_NoClip(101,91,119,38);// outline rect
   gfx_SetTextFGColor(RED);
   gfx_PrintStringXY("Are you sure?",112,100);
   gfx_PrintStringXY("Yes=2nd  No=Mode",102,115);
   uint8_t result = 0;
   uint8_t keyPressed;
   while ((keyPressed=os_GetCSC())!=sk_2nd && (keyPressed=os_GetCSC())!=sk_Clear) {
      if (keyPressed==sk_2nd) {
         ti_Delete(HS->fileNames[HS->selectedFile]);
         result = 1;;
      }
      if (keyPressed==sk_Clear) {
         result = 2;
      }
   }
   if(result==1) {
      return 1;
   }else {
      return 0;
   }
}