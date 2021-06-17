/*
#include <stdint.h>
#include <fileioc.h>
#include <keypadc.h>
#include <graphx.h>
//my includes
#include "includes/ui.h"
#include "includes/file.h"
#include "includes/text.h"
*/
#include "main.h"

uint8_t dispHomeScreen() {
   // set up struct for homescreen variables & data
   struct fileViewerStruct HS; // homescreen struct
   uint8_t numFilesShown = 0;          // number of files currently shown on screen. can't be more than 10
   HS.selectedFile = 0;
   // uint8_t viewerOffset = 0;
   HS.numFiles = getNumFiles("TXT");
   loadFileNames(HS.fileNames);

   while(1) {
      dispHomeScreenBG();
      dispHSButtons();
      dispFiles(&HS);
      gfx_PrintStringXY(HS.fileNames[0], 10, 10);

      // keypresses
      kb_Scan();
      // moving cursor
      if(kb_IsDown(kb_KeyDown) && HS.selectedFile<HS.numFiles) {
         HS.selectedFile++;
      }
      if(kb_IsDown(kb_KeyUp) && HS.selectedFile>0) { // move selected up
         HS.selectedFile--;
      }
      // quit program
      if(kb_IsDown(kb_KeyClear)) {
         gfx_End();
         ti_CloseAll();
         return 0;
      }
      // new file
      if(kb_IsDown(kb_KeyTrace)) {
         newFile();
      }
      // delete file
      if(kb_IsDown(kb_KeyZoom) && HS.numFiles) {
         checkIfDelete(HS.fileNames[HS.selectedFile]);
      }

      gfx_SwapDraw();
      gfx_Wait();
   }
}

uint8_t dispFiles(struct fileViewerStruct *HS) {
   uint8_t i;
   uint8_t fileSlot; // slot number of currently detected file
   uint8_t fileSize; // size of currently detected and drawn file
   uint8_t fileY = 61; // y coord of currently drawn file

   // go through all filenames in the fileName array, display them, open the files, and display data such as size & status. yes, yes, i know that i have it always display 10 files regardless of how many there actually are, but this is simpler and I reset the fileNames array every time a file is deleted/added so this (hopefully) won't affect anything negatively
   for(i=HS->offset; i<10; i++) {
      fileSlot = ti_Open(HS->fileNames[i],"r");
      fileSize = ti_GetSize(fileSlot);

      // display currently selected file with highlighting rect
      if (HS->selectedFile == i) {
         gfx_SetColor(0);
         gfx_FillRectangle_NoClip(35,fileY-5,250,15);
         gfx_SetTextFGColor(1);
      } else {
         gfx_SetTextFGColor(0);
      }
      // display detected file name & size & 
      gfx_PrintStringXY(HS->fileNames[i],40,fileY);
      gfx_SetTextXY(135,fileY);
      gfx_PrintInt(fileSize,4);
      gfx_SetTextFGColor(0);
      fileY += 15;
   }
   // display when no files were detected because you forgot to take notes :P
   if (HS->numFiles == 0) {
      gfx_SetTextFGColor(244);
      gfx_PrintStringXY("--NO FILES FOUND--)",93,80);
      gfx_PrintStringXY("That's too bad for you :(",93,100);
   }

   // return the number of files displayed, I guess. this really isn't necessary at all btw. if i run low on bytes, which is unlikely, will delete this. :P
   return i;
}

// homescreen for the fileViewer, rectangles, title, etc...
void dispHomeScreenBG() {
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
}

void dispHSButtons()
{
   int i = 0;
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

// text editor stuff
uint8_t dispEditor() {
   return 0;
}

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
}


// Congrats, you actually got to the bottom of this file! Did you actually read everything or did you just scroll down quickly? :P