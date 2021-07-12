#include "main.h"

// btw, HS stands for homescreen
uint8_t dispHomeScreen() {
   // set up struct for homescreen variables & data
   struct fileViewerStruct HS;
   HS.selectedFile = 0;
   HS.offset = 0;
   HS.numFiles = loadFiles(&HS);
	archiveAll();

   while(1) {
      dispHomeScreenBG();
      dispHSButtons();
      dispFiles(&HS);

      handleHSKeyPresses(&HS);

      // quit program
		if(os_GetCSC() == sk_Clear)
      	return 0;

      gfx_SwapDraw();
      gfx_Wait();
   }
}

// display the file names & info stored in the fileViewerStruct *HS
uint8_t dispFiles(struct fileViewerStruct *HS) {
   uint8_t i;          // starting increment of file display, ends up as the number of files displayed onscreen (should be <=10)
   uint8_t ii = 0;     // how many files have been displayed so far
   uint8_t fileSlot;   // slot number of currently detected file
   uint8_t fileSize;   // size of currently detected and drawn file
   int fileY = 61;

   for(i=HS->offset; i<10+HS->offset && i<HS->numFiles; i++) {
      fileSlot = ti_Open(HS->fileNames[i],"r+");
      fileSize = ti_GetSize(fileSlot);

      // display currently selected file with a scrollbar on top of it. also, invert the text color so you can see it's name. I was inspired to do this by hexaedit (a pretty amazing program), btw
      if (HS->selectedFile == i) {
         gfx_SetColor(0);
         gfx_FillRectangle_NoClip(36,fileY-5,248,15);
			gfx_SetColor(DARK_BLUE);
			thick_Rectangle(34,fileY-7,252, 19, 2);
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
      ii++;
   }
   // display when no files were detected because you forgot to take notes :P
   if (HS->numFiles == 0) {
      gfx_SetTextFGColor(244);
      gfx_PrintStringXY("--NO FILES FOUND--)",93,80);
      gfx_PrintStringXY("That's too bad for you :(",93,100);
   }

   // Return the number of files displayed, I guess. This really isn't necessary at all btw. If I run low on bytes in the stack, which is very unlikely (since I am writing in C && am not defining a ton of variables && am such an efficient programmer lol && never use globals && always use tons of small functions), I will delete this. :P
   return i;
}

// homescreen for the fileViewer, rectangles, title, etc...
void dispHomeScreenBG() {
   gfx_SetDraw(1);
   gfx_FillScreen(MEDIUM_GREY);
	
	// header
   gfx_SetColor(DARK_BLUE);
	thick_Rectangle(50, 1, 222, 30, 2);
	gfx_PrintStringXY("SMARTNOTES CE",115,5);
   gfx_PrintStringXY("VERSION 1.0 BY JOHNPAUL MALLOY",55,20);
	
	// box with file names
   gfx_SetColor(WHITE);
   gfx_FillRectangle_NoClip(36,56,248,150);
	gfx_SetColor(DARK_BLUE);
	thick_Rectangle(34, 54, 252, 154, 2);
	
   gfx_SetTextFGColor(BLACK);
   gfx_PrintStringXY("NAME",40,45);
   gfx_PrintStringXY("SIZE",135,45);
   gfx_PrintStringXY("STATUS",210,45);
}

void dispHSButtons()
{
   int i = 0;
   //button rects at bottom of screen
   for(i=0; i<320; i+=64) {
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

void handleHSKeyPresses(struct fileViewerStruct *HS) {
   kb_Scan();

   // move cursor down
   if(kb_IsDown(kb_KeyDown) && HS->selectedFile < HS->numFiles-1) {
      HS->selectedFile++;
      if(HS->selectedFile >= HS->offset+10){
         HS->offset++;
      }
   }

	// move cursor up
   if(kb_IsDown(kb_KeyUp) && HS->selectedFile>0) {
      HS->selectedFile--;
      if(HS->selectedFile < HS->offset){
         HS->offset--;
      }
   }

   // new file
   if(kb_IsDown(kb_KeyTrace)) {
		newFile();
		loadFiles(HS);
   }

   // delete file
   if((kb_IsDown(kb_KeyZoom) || kb_IsDown(kb_KeyDel)) && HS->numFiles>0) {
      checkIfDeleteSelected(HS);
   }
	
}

// text editor stuff
/*
uint8_t dispEditor(struct editorStruct * ES) {
   return 0;
}
*/

// cursor stuff
void animateCursor(struct cursorStruct *CS) {
   if(CS->cursorState > CS->invisibleTime) {
      drawCursor(CS->x, CS->y);
   }
   if(CS->cursorState >= CS->cyclesPerAnimation) {
      CS->cursorState = 0;
   }
   CS->cursorState++;
}

void drawCursor(int x, int y) {
   gfx_SetColor(DARK_BLUE);
   gfx_VertLine_NoClip(x, y, 11);
   gfx_VertLine_NoClip(x+1, y, 11);
}

int8_t textBox(const char *text, int boxWidth, int boxHeight, int boxX, int boxY) {
	uint8_t keyPressed = 0;
	
	// fontlibc setups (still in experimental phase)
	fontlib_SetWindow(boxX, boxY, boxWidth, boxHeight);

	// body rect of alert header
	gfx_SetColor(LIGHT_GREY);
	gfx_FillRectangle(boxX, boxY, boxWidth, boxHeight);

	// outline rect of alert header
	gfx_SetColor(DARK_BLUE);
	thick_Rectangle(boxX, boxY, boxWidth, boxHeight, 2);

	fontlib_SetForegroundColor(BLACK);
	fontlib_SetCursorPosition(boxX+3, boxY+2);
	fontlib_DrawString(text);
	
	// waits for keypress, if clear is pressed, then return 0, if enter or second is pressed, then return 1
	while(keyPressed != sk_Clear && keyPressed != sk_2nd && keyPressed != sk_Enter)
		keyPressed = os_GetCSC();
		
	if(keyPressed == sk_Clear) {
		return 0;
	}
	if(keyPressed == sk_2nd || keyPressed == sk_Enter) {
		return 1;
	}
	return 0;
}


void thick_Rectangle(int x, int y, int width, int height, uint8_t thickness) {
	uint8_t i;
	
	for(i=0; i<thickness; i++) {
		gfx_Rectangle(x+i, y+i, width-(2*i), height-(2*i));
	}
	
	return;
}

// gives an option whether or not to delete the selected file
uint8_t checkIfDeleteSelected(struct fileViewerStruct *HS )
{
	
	uint16_t width = 50;
	uint16_t height = 50;
	uint16_t x = (320/2)-(width/2);
	uint16_t y = (240/2)-(height/2);
	
	// the alert function uses drawing routines, so you have to set the draw buffer
	gfx_SetDraw(0);
	
	int result = textBox("Delete?", width, height, x, y);
	
	// obviously, to see the graphics, you have to swap the buffers
	gfx_SwapDraw();
	
	// if the user pressed enter or 2nd, delete the selected file
	if(result) {
		ti_Delete(HS->fileNames[HS->selectedFile]);
		loadFiles(HS);
		
		if(HS->selectedFile>0 && HS->numFiles>10) {
         HS->selectedFile--;		
         HS->offset--;
      }
		
		return 1;
	}
	return 0;
}

int chooseToQuit() {
	#define SIG_HOLD_TIME 100
	int holdTime = 0;
	kb_Scan();
	while(kb_IsDown(kb_KeyClear) && holdTime < SIG_HOLD_TIME) {
		kb_Scan();
		holdTime++;
	}
	
	if(holdTime >= SIG_HOLD_TIME)
		return 1;
		
	if(holdTime < SIG_HOLD_TIME)
		return 0;
		
	return 0;
}

// Congrats, you actually got to the bottom of this file! Did you actually read everything or did you just scroll down quickly? :P