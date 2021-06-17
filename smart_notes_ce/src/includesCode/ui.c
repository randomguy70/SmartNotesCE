#include "main.h"

// btw, HS stands for homescreen
uint8_t dispHomeScreen() {
   // set up struct for homescreen variables & data
   struct fileViewerStruct HS;
   HS.selectedFile = 0;
   HS.offset       = 0;
   HS.viewOffset   = 0;
   HS.numFiles = loadFiles(&HS);

   while(1) {
      dispHomeScreenBG();
      dispHSButtons();
      dispFiles(&HS);

      // keypresses
      kb_Scan();
      // moving cursor
      if(kb_IsDown(kb_KeyDown) && HS.selectedFile<HS.numFiles-1) {
         HS.selectedFile++;
         HS.viewOffset++;
         if(HS.selectedFile>HS.offset+HS.viewOffset){
            HS.viewOffset--;
            HS.offset++;
         }
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
         loadFiles(&HS);
      }
      // delete file
      if(kb_IsDown(kb_KeyZoom) && HS.numFiles>0) {
         checkIfDelete(&HS);
         loadFiles(&HS);
      }

      gfx_SwapDraw();
      gfx_Wait();
   }
}

uint8_t dispFiles(struct fileViewerStruct *HS) {
   uint8_t i;
   uint8_t fileSlot;   // slot number of currently detected file
   uint8_t fileSize;   // size of currently detected and drawn file
   int fileY = 61;

   for(i=HS->offset; i<10 && i<numFiles; i++) {
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
      fileY+=15;
   }
   // display when no files were detected because you forgot to take notes :P
   if (HS->numFiles == 0) {
      gfx_SetTextFGColor(244);
      gfx_PrintStringXY("--NO FILES FOUND--)",93,80);
      gfx_PrintStringXY("That's too bad for you :(",93,100);
      gfx_PrintStringXY("Were you being a naughty student and not taking notes? :P", 75, 120);
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

// Congrats, you actually got to the bottom of this file! Did you actually read everything or did you just scroll down quickly? :P