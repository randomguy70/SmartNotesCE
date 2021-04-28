#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <keypadc.h>
#include <compression.h>
#include <graphx.h>
#include <fileioc.h>

/*necessary information
-3 modes
   mode 1 = homescreen
   mode 2 = editor
   mode 3 = settings
-3 txt modes
   mode 1=Caps
   mode 2 = math
   mode 3 = lowercase

*/

/* functon prototypes */
//char * input_string(char*, char*, uint8_t, uint8_t, uint8_t);
int dispHomeScreen();
void exitFull();
void handleKeyPresses();
void dispButtons(uint8_t);
int dispFiles();
uint8_t waitForGetKey();
void archiveAll();
char inputChar();
const char* inputString();

/* main loop */
int main() {
   uint8_t mode = 1;
   //put mode at 0 to exit, or call function exitFull()
   while(mode) {
      if (mode == 1) {
         mode = dispHomeScreen();
      } else if (mode == 2) {

      } else if(mode == 3) {

      }
   }
}

// string input needs work. like a lot of work. in other words, pretend that you did not even notice this function (yet)
/*
char * input_string(uint8_t min_len, uint8_t max_len, uint8_t x, uint8_t y) {
   while(1) {

   }
   return string;
}
*/

int dispHomeScreen() {
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
   //uint8_t mode = 1; // 3 modes are homescreen, editor, and settings. DIFFERENT from txtMode
   
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
               gfx_SetColor(00;
               gfx_Rectangle_NoClip(100,90,121,40);
               //text
               gfx_SetTextFGColor(0);
               gfx_PrintStringXY("Are you sure?",112,100);
               gfx_PrintStringXY("Yes=2nd  No=Mode",102,115);
               gfx_Blit(1);

               while (!(K=54) || (K=55))
                  kb_Scan();
                  if (kb_IsDown(kb_Key2nd)) {
                     Delete(Str1
                     if (selectedNum>1)
                        selectedNum--;
                     }
                  }
               }

               ti_Delete(fileName);
               //delay(100);
               numFiles--;
               if(selectedNum>numFiles) {
                  selectedNum--;
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
      } else if(kb_IsDown(kb_KeyTrace)) {
         char buffer[9];
         inputString(buffer);
      }

      gfx_SwapDraw();
      gfx_Wait();
      //delay(30);
   }
   return 1;
}

int inputString(char* buffer, uint8_t length)
{
   uint8_t result;
   uint8_t cursorX;
   uint8_t fileSlot;

   while (!result) {
      kb_Scan();
      char character = '\0';

         if (kb_IsDown(kb_KeyClear)) {
            result = 2;
         }
         
         if (kb_IsDown(kb_KeyEnter) && 0<strLen<9) {
            fileSlot = ti_Open(&string,"w+");
            ti_Write("TXT",3,1,fileSlot);
         }
         
         if (kb_AnyKey() && strLen<9) {
            character = inputChar();
            if (character != NULL) {
               string = string+character;
               strLen++;
            }
         }
         
         //if (K=56) and (STRLEN>0)
         //   sub(Str4,0,STRLEN-1->Str4
         //}
         
         //display current string/new filename with outline box
         gfx_SetDraw(1);
         //big box fill
         gfx_SetColor(149);
         gfx_FillRectangle_NoClip(110,90,80,40);
         //big box outline
         gfx_SetColor(0);
         gfx_Rectangle_NoClip(110,90,80,40);
         //text box fill
         gfx_SetColor(255);
         gfx_FillRectangle_NoClip(115,110,70,13);
         //text box outline
         gfx_SetColor(0);
         gfx_Rectangle_NoClip(115,110,70,13);
         //display text
         gfx_SetTextFGColor(0);
         gfx_PrintStringXY("New file",120,95);
         gfx_PrintStringXY(&string,117,113);
         //display cursor
         cursorX = gfx_GetTextX()+1;
         cursorY = gfx_GetTextY()-4;
         gfx_VertLine_NoClip(cursorX,cursorY,8);
         gfx_Blit(1);
   }
}

char inputChar() {
   return 'h';
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

void handleKeyPresses(int mode)
{
   
}

void archiveAll() 
{
   uint8_t fileSlot;
   char * fileName;
   int search_pos = NULL;
   while (((fileName = ti_Detect(&search_pos, "TXT")) != NULL)) {
      fileSlot = ti_Open(fileName, "r");
      ti_SetArchiveStatus(1, fileSlot);
   }
   ti_CloseAll();
}

void exitFull() {
   archiveAll();
   gfx_End();
   abort();
};
