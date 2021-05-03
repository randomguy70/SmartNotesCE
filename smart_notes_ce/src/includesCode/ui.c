#include <stdint.h>
#include <fileioc.h>
#include <keypadc.h>
#include "gfx/gfx.h"

uint8_t dispHomeScreen() {
   uint8_t i; // for loop var
   uint8_t numFiles = 0; // total number of files detected
   uint8_t numFilesShown = 0; // number of files currently shown on screen. can't be more than 10
   uint8_t fileSlot; // slot of currently detected file
   uint8_t numFile = 0; // number of currently detected file
   uint8_t fileY = 61; // y coord for currently detected file
   char * fileName; // pointer to name of currently detected file
   int fileSize = 0; // size of currently detected file
   uint8_t selectedNum = 0; // number of currently selected file
   char * selectedName; // pointer to name of selected file

   gfx_Begin();
   kb_SetMode(MODE_3_CONTINUOUS);

   // main homescreen loop. displays files, options, etc...
   while(1) {
      numFiles = 0; // total number of files detected
      numFilesShown = 0; // number of files currently shown on screen. can't be more than 10
      numFile = 0; // number of currently detected file
      fileY = 61; // y coord for currently detected file

      gfx_SetDraw(1);

      // SCREEN SETUP
      gfx_FillScreen(149);
      gfx_SetColor(0);
      gfx_Rectangle_NoClip(50,1,222,30);
      gfx_Rectangle_NoClip(35,55,250,152);
      gfx_SetColor(255);
      gfx_FillRectangle_NoClip(36,56,248,150);
      gfx_SetTextFGColor(0);
      gfx_PrintStringXY("SMARTNOTES CE",115,5);
      gfx_PrintStringXY("VERSION 1.0 BY JOHNPAUL MALLOY",55,20);
      gfx_PrintStringXY("NAME",40,45);
      gfx_PrintStringXY("SIZE",135,45);
      gfx_PrintStringXY("STATUS",210,45);

      // displays the button options (delete, rename, settings, open, etc...)
      // dispButtons(mode);

      // close all open slots
      ti_CloseAll();
      dispButtons(1);

      // vat offset for appvar detection loop
      void *search_pos = NULL;
      
      // loop to detect and display files
      while (((fileName = ti_Detect(&search_pos, "TXT")) != NULL) && (numFilesShown<10)) {

         // get file info, archive all text file appvars detected et cetera...
         fileSlot = ti_Open(fileName,"r");
         fileSize = ti_GetSize(fileSlot);
         ti_SetArchiveStatus(1,fileSlot);

         // display currently selected file with highlighting rect
         if (selectedNum == numFile) {
            // check if should be deleted
            kb_Scan();
            if(kb_IsDown(kb_KeyZoom)) {
               gfx_SetDraw(1);
               gfx_SetColor(149);
               gfx_FillRectangle_NoClip(100,90,121,40);
               gfx_SetColor(0);
               gfx_Rectangle_NoClip(100,90,121,40);
               //text
               gfx_SetTextFGColor(0);
               gfx_PrintStringXY("Are you sure?",112,100);
               gfx_PrintStringXY("Yes=2nd  No=Mode",102,115);
               gfx_Blit(1);

               uint8_t result = 0;
               while (!result) {
                  kb_Scan();
                  if (kb_IsDown(kb_2nd)) {
                     ti_Delete(selectedName);
                     if (selectedNum>1) {
                        selectedNum--;
                     }
                     numFiles--;
                     delay(100);
                     result = 1;
                  } else if (kb_IsDown(kb_Clear)) {
                     result = 2;
                  }
               }
            }
            gfx_SetColor(0);
            gfx_FillRectangle_NoClip(35,fileY-5,250,15);
            gfx_SetTextFGColor(149);
         } else {
            gfx_SetTextFGColor(0);
         }

         // display detected file name & size
         gfx_PrintStringXY(fileName,40,fileY);
         gfx_SetTextXY(135,fileY);
         gfx_PrintInt(fileSize,4);
         
         // update search-dependant vars
         numFiles++;
         numFile++;
         numFilesShown++;
         fileY += 15;
      }

      //show number of files in top right corner
      gfx_SetTextXY(300,10);
      gfx_PrintInt(numFiles,1);

      // display when no appvars are found
      if (numFiles == 0) {
         gfx_SetTextFGColor(244);
         gfx_PrintStringXY("--NO FILES FOUND--",93,80);
      }

      //keypresses
      kb_Scan();

      // move selected down
      if(kb_IsDown(kb_KeyDown) && selectedNum < (numFiles-1)) {
         selectedNum++;
      } else if(kb_IsDown(kb_KeyUp) && selectedNum > 0) {
         selectedNum--;
      } else if(kb_IsDown(kb_KeyClear)) {
         exitFull();
      } else if(kb_IsDown(kb_KeyZoom) && (numFiles >0)) {
         ti_Delete(selectedName);
      } else if(kb_IsDown(kb_KeyTrace)) { // new file
         char buffer[9];
         for(i=0; i<9; i++) {
            buffer[i] = 0;
         }
         if(inputString(buffer, 8)==1) {
            ti_Open(buffer, "w+");
         }
      }

      gfx_SwapDraw();
      gfx_Wait();
   }
   return 1;
}

void dispButtons(uint8_t txtMode)
{
   int i = 0;
   if(txtMode == 1) {
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

void exitFull() {
   archiveAll();
   gfx_End();
   abort();
}

