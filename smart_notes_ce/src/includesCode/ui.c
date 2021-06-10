#include <stdint.h>
#include <fileioc.h>
#include <keypadc.h>
#include <graphx.h>
//my includes
#include "includes/ui.h"
#include "includes/file.h"
#include "includes/text.h"

uint8_t dispHomeScreen() {
   uint8_t numFilesShown = 0;          // number of files currently shown on screen. can't be more than 10
   uint8_t numFiles = 0;               // total number of txt files detected
   char selectedName[9];         // pointer to name of selected file
   uint8_t selectedNum = 0;            // number of currently selected file
   uint8_t viewerOffset = 0;
   numFiles = getNumFiles("TXT");
   char fileNames[20][9] = {0};           // array of all the names of text files. there can only be 20 at the most (might change that if someone asks)
   loadFileNames(fileNames);

   while(1) {
      // graphics
      gfx_SetDraw(1);
      gfx_FillScreen(4);
      gfx_SetColor(0);
      gfx_Rectangle_NoClip(50,1,222,30);
      gfx_Rectangle_NoClip(35,55,250,152);
      gfx_SetColor(1);
      gfx_FillRectangle_NoClip(36,56,248,150);
      gfx_SetTextFGColor(0);
      gfx_PrintStringXY("SMARTNOTES CE",115,5);
      gfx_PrintStringXY("VERSION 1.0 BY JOHNPAUL MALLOY",55,20);
      gfx_PrintStringXY("NAME",40,45);
      gfx_PrintStringXY("SIZE",135,45);
      gfx_PrintStringXY("STATUS",210,45);

      // display the txt files and buttons
      numFilesShown = dispFiles(viewerOffset, selectedNum, selectedName);
      gfx_PrintStringXY(fileNames[0], 10, 10);
      dispButtons(1);

      // handle keypresses, should probably make this a function
      kb_Scan();
      // move selected down
      if(kb_IsDown(kb_KeyDown) && selectedNum<numFiles-1) {
         if(selectedNum == viewerOffset + 9) {
            viewerOffset++;
            selectedNum++;
         } else {
            selectedNum++;
         }
      }
      if(kb_IsDown(kb_KeyUp) && selectedNum>0) { // move selected up
         if(selectedNum == viewerOffset) {
            selectedNum--;
            viewerOffset--;
         }
         else {
            selectedNum--;
         }
      }
      if(kb_IsDown(kb_KeyClear)) { // quit program
         gfx_End();
         return 0;
      }
      if(kb_IsDown(kb_KeyZoom)) { //  delete file
         ti_Delete(selectedName);
      }
      if(kb_IsDown(kb_KeyTrace)) { // new file
         char buffer[9] = {0};
         if(inputString(buffer, 8) > 0) {
            newFile(buffer);
            numFiles++;
         }
      }
      if(kb_IsDown(kb_KeyZoom) && numFiles) {
         deleteFile(selectedName);
      }
      gfx_SetColor(6);
      gfx_SetTextXY(10,10);
      gfx_PrintInt(numFiles,1);
      gfx_SetTextXY(10, 25);
      gfx_PrintInt(selectedNum, 1);
      gfx_SetTextXY(10, 35);
      gfx_PrintInt(viewerOffset, 1);
      gfx_SwapDraw();
      gfx_Wait();
   }
}

uint8_t dispFiles(uint8_t offset, uint8_t selectedNum, char* selectedName) {
   uint8_t i;
   uint8_t numFilesShown = 0; // number of files currently drawn on the file viewer at once
   char* fileName;
   uint8_t fileSlot; // slot number of currently detected file
   uint8_t fileSize; // size of currently detected and drawn file
   uint8_t fileY = 61;
   uint8_t numFile = 0; // number of currently detected and drawn file

   // jump to offset in file viewer (this is for scrolling purposes)
   void *search_pos = NULL;
   if(offset) {
      while((fileName = ti_Detect(&search_pos, "TXT") != NULL && numFile<offset-1)) {
         numFile++;
      }
   }

   // display files starting at offset
   while (((fileName = ti_Detect(&search_pos, "TXT")) != NULL) && (numFilesShown<10)) {
      
         // get file info, get file info, get file info...
         fileSlot = ti_Open(fileName,"r");
         fileSize = ti_GetSize(fileSlot);

         // display currently selected file with highlighting rect
         if (selectedNum == numFile) {
            gfx_SetColor(0);
            gfx_FillRectangle_NoClip(35,fileY-5,250,15);
            gfx_SetTextFGColor(1);
            // write name of selected file to given buffer
            i=0;
            while(*(selectedName+i) != '\0' || fileName[i] != '\0') {
               selectedName[i] = fileName[i];
               i++;
            }
         } else {
            gfx_SetTextFGColor(0);
         }

         // display detected file name & size & 
         gfx_PrintStringXY(fileName,40,fileY);
         gfx_SetTextXY(135,fileY);
         gfx_PrintInt(fileSize,4);
         gfx_SetTextFGColor(0);

         // update search-dependant vars
         numFile++;
         numFilesShown++;
         fileY += 15;
      }
      // finish detecting the rest of the files to get an accurate count of how many there are, but don't display them because they would go off the screen
      while((fileName = ti_Detect(&search_pos, "TXT")) != NULL) {
      numFile++;
      }
      // display when no appvars are found
      if (numFile == 0) {
         gfx_SetTextFGColor(244);
         gfx_PrintStringXY("--NO FILES FOUND--)",93,80);
         gfx_PrintStringXY("That's too bad for you :(",93,100);
      }
   return numFilesShown;
}

void dispButtons(uint8_t mode)
{
   int i = 0;
   if(mode == 1) {
      //button rects at bottom of screen
      for(i=0; i<320; i=i+64) {
         gfx_SetColor(0);
         gfx_Rectangle_NoClip(i,220,62,19);
         gfx_SetColor(42);
         gfx_FillRectangle_NoClip(i+1,221,60,17);
      }

      //button text
      gfx_SetTextFGColor(0);
      gfx_PrintStringXY("Open",16,227);
      gfx_PrintStringXY("Rename",73,227);
      gfx_PrintStringXY("Delete",137,227);
      gfx_PrintStringXY("New",211,227);
      gfx_PrintStringXY("Other",270,227);
   }

};

// text editor stuff
uint8_t dispEditor() {
   return 0;
}

// other
uint8_t checkIfDelete(char *name) {
   gfx_SetDraw(1);
   gfx_SetColor(4);
   gfx_FillRectangle_NoClip(100,90,121,40); // main rect
   gfx_SetColor(6);
   gfx_Rectangle_NoClip(100,90,121,40);// outline rect
   gfx_Rectangle_NoClip(101,91,119,38);// outline rect
   //text
   gfx_SetTextFGColor(6);
   gfx_PrintStringXY("Are you sure?",112,100);
   gfx_PrintStringXY("Yes=2nd  No=Mode",102,115);
   gfx_Blit(1);
   while (1) {
      kb_Scan();
      if (kb_IsDown(kb_2nd)) {
         ti_Delete(name);
         delay(200);
         return 1;;
      } else if (kb_IsDown(kb_Clear)) {
         return 0;
      }
   }
}

void exitFull() {
   archiveAll();
   gfx_End();
   exit(0);
}

