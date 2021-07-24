#include "main.h"


// btw, HS stands for homescreen
uint8_t dispHomeScreen(struct fileViewerStruct *HS) {
	// initialise / reset variables
   HS->selectedFile = 0;
   HS->offset = 0;
   HS->numFiles = loadFiles(HS);
	HS->shouldQuit = false;
	archiveAll();
	
	uint8_t result = 1;

   while(true) {
      dispHomeScreenBG(HS);
      dispHSButtons();
		
      dispFiles(HS);
		
		gfx_Wait();
		gfx_SwapDraw();
		
      result = handleHomeScrnKeyPresses(HS);
		
		if(result == QUIT)
			return QUIT;
		if(result == OPEN)
			return OPEN;
			
   }
}

// display the file names & info stored in the fileViewerStruct *HS
uint8_t dispFiles(struct fileViewerStruct *HS) {
   uint8_t i;          // starting increment of file display, ends up as the number of files displayed onscreen (should be <=10)
   uint8_t ii = 0;     // how many files have been displayed so far
   uint8_t fileSlot;   // slot number of currently detected file
   uint8_t fileSize;   // size of currently detected and drawn file
   int fileY = 61;

   for(i=HS->offset, HS->numFilesDisplayed = 0; i < 10+HS->offset && i  <HS->numFiles; i++, HS->numFilesDisplayed++) {
      fileSlot = ti_Open(HS->fileNames[i],"r+");
      fileSize = ti_GetSize(fileSlot);

      // display currently selected file with a scrollbar on top of it
      if (HS->selectedFile == i) {
			// draw scrollbar & leave some pixels at the edge of the window for the scrollbar
         gfx_SetColor(LIGHT_GREY);
         gfx_FillRectangle_NoClip(36,fileY-5,242,15);
			gfx_SetColor(BLACK);
			gfx_Rectangle_NoClip(36,fileY-5,242,15);

      }
		
      gfx_SetTextFGColor(BLACK);
		
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
void dispHomeScreenBG(struct fileViewerStruct * HS) {
	int width;
	
	// scrollbar math
	uint8_t scrollbarHeight = 148 * HS->numFilesDisplayed / HS->numFiles;
	
	// just making sure that the scrollbar is a reasonable size...
	if(scrollbarHeight>148)
		scrollbarHeight = 148;
	if(scrollbarHeight<10)
		scrollbarHeight = 10;
	
	int scrollbarX = 280;
	int scrollbarY = (150 - (scrollbarHeight) + 1) * HS->selectedFile / (HS->numFiles-1) + 56;
	
   gfx_SetDraw(1);
	
	// lined-paper background
   gfx_FillScreen(PAPER_YELLOW);
	gfx_SetColor(LIGHT_BLUE);
	gfx_SetTextXY(1, 1);
	for(uint8_t i = 0; i<11; i++) {
		gfx_HorizLine_NoClip(0, i*20, SCRN_WIDTH);
		gfx_HorizLine_NoClip(0, i*20+1, SCRN_WIDTH);
	}

	// name and credits (to me :P)
	gfx_SetTextFGColor(DARK_BLUE);
	width = gfx_GetStringWidth("SmartNotes CE");
	gfx_PrintStringXY("SMARTNOTES CE", (SCRN_WIDTH/2)-(width/2), 8);
	
	gfx_SetTextFGColor(BLACK);
	width = gfx_GetStringWidth("VERSION 1.0 BY Randomguy");
   gfx_PrintStringXY("Version 1.0 BY Randomguy", (SCRN_WIDTH/2)-(width/2), 27);
	
	// box with file names
   gfx_SetColor(WHITE);
   gfx_FillRectangle_NoClip(36,56,248,150);
	gfx_SetColor(LIGHT_BLUE);
	thick_Rectangle(34, 54, 252, 154, 2);
	
	// scrollbar border
	gfx_SetColor(LIGHT_BLUE);
	gfx_VertLine_NoClip(scrollbarX-2, 56, 150);
	gfx_VertLine_NoClip(scrollbarX-1, 56, 150);
	
	// scrollbar
	gfx_SetColor(LIGHT_GREY);
	gfx_FillRectangle_NoClip(scrollbarX, scrollbarY, 4, scrollbarHeight);
	gfx_SetColor(BLACK);
	gfx_Rectangle_NoClip(scrollbarX, scrollbarY, 4, scrollbarHeight);
	
	// print labels for displayed file data columns
   gfx_SetTextFGColor(BLACK);
   gfx_PrintStringXY("NAME",40,45);
   gfx_PrintStringXY("SIZE",135,45);
   gfx_PrintStringXY("STATUS",210,45);
}

void dispHSButtons() {
	
	gfx_sprite_t * sprites[5];
	sprites[0] = open;
	sprites[1] = new_icon;
	sprites[2] = quit;
	sprites[3] = trash;
	sprites[4] = more;
	
   for(int i = 1, ii = 0; i < 320; i+=64, ii++) {
		// button rects
      gfx_SetColor(LIGHT_GREY);
      gfx_FillRectangle_NoClip(i+1, 215, 60, 24);
		gfx_SetColor(BLACK);
      gfx_Rectangle_NoClip(i+1, 215, 60, 24);
		
		// sprites
		if(i == 1)
			gfx_TransparentSprite_NoClip(sprites[ii], i+4, 218);
		else
			gfx_TransparentSprite_NoClip(sprites[ii], i+3, 217);
	}
	
	// text
	gfx_SetTextFGColor(0);
	
   gfx_PrintStringXY("Open" ,  27, 224);
   gfx_PrintStringXY("New"  ,  90, 224);
   gfx_PrintStringXY("Quit" , 157, 224);
   gfx_PrintStringXY("Del"  , 220, 224);
   gfx_PrintStringXY("Other", 271, 224);
	
}

uint8_t handleHomeScrnKeyPresses(struct fileViewerStruct *HS) {
   kb_Scan();

   // move cursor down
   if(kb_IsDown(kb_KeyDown) && HS->selectedFile < HS->numFiles-1) {
      HS->selectedFile++;
      if(HS->selectedFile >= HS->offset+10){
         HS->offset++;
      }
   }

	// move cursor up
   if (kb_IsDown(kb_KeyUp) && HS->selectedFile>0) {
      HS->selectedFile--;
      if(HS->selectedFile < HS->offset){
         HS->offset--;
      }
   }

	// open file
	if(kb_IsDown(kb_KeyYequ)) {
		// if there aren't any files to open...
		if(HS->numFiles <= 0) {
			alert("There aren't any files to open (obviously).");
			return CANCEL;
		}
		// otherwise...open the selected file.
		return OPEN;
	}
	
   // new file
   if (kb_IsDown(kb_KeyWindow)) {
		if(HS->numFiles >= 30 ) {
			alert("You can't have more than 30 files, my note-crazy friend!");
			return CANCEL;
		}
		newFile();
		loadFiles(HS);
		return true;
   }
	
	// quit program
	sk_key_t key = os_GetCSC();
	
	if (key == sk_Clear || key == sk_Zoom)
		return QUIT;
	
	// delete file
   if ((kb_IsDown(kb_KeyTrace) || kb_IsDown(kb_KeyDel))) {
		// make sure there is a file to delete
		if(HS->numFiles == 0) {
			alert("There aren't any files to delete!");
			return CANCEL;
		}
		// if there is at least 1 file...
		checkIfDeleteFile(HS->fileNames[HS->selectedFile]);
		loadFiles(HS);
		// check if you need to shift the cursor
		if(HS->numFiles > 0 && HS->selectedFile >= HS->numFiles)
			HS->selectedFile--;
			
		return CANCEL;
   }
	
	// other (opens fun menu with sprites)
	if(kb_IsDown(kb_KeyGraph)) {
		
		struct menu *menu = loadHomeScreenOtherMenu();
		
		uint8_t result = displayMenu(menu);
			
		/* --Options--
			back, rename, hide, settings, help (start at 1 as an offset because 0 signifies quit)
		*/
		switch(result) {
			
			// quit
			case QUIT:
				return QUIT;
				
			// back
			case 1:
				return CANCEL;
			
			// rename
			case 2:
				if(HS->numFiles>0) {
					if(renameFile(HS->fileNames[HS->selectedFile]))
						loadFiles(HS);
						return 1;
				}
				// if it didn't return, then there aren't any files to rename...
				alert("There aren't any files to rename!");
				return 1;
				
			// hide
			case 3: 
				// hideFile(char* name); // haven't defined this yet btw...
				return 1;
				
			// settings
			case 4:
				// displaySettings(); // i haven't defined this either...
				return 1;
				
			// if the user simply wants to close the menu
			default:
				return CANCEL;
			
		}
		
	}

	// if the user doesn't want to quit, then return 1
	return 1;
}

// cursor stuff
void animateCursor(struct cursorStruct *CS) {
   if(CS->cursorState > CS->invisibleTime) {
      drawCursor(CS);
   }
   if(CS->cursorState >= CS->cyclesPerAnimation) {
      CS->cursorState = 0;
   }
   CS->cursorState++;
}

void drawCursor(struct cursorStruct * cursor) {
   gfx_SetColor(DARK_BLUE);
   gfx_VertLine_NoClip(cursor->x, cursor->y, 11);
   gfx_VertLine_NoClip(cursor->x+1, cursor->y, 11);
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
bool checkIfDeleteFile(char *name) {	
	char message[100] = {"Are you sure you want to delete "};
	strcat(message, name);
	strcat(message, "?");
	
	if(alert(message) == true) {
		ti_Delete(name);
		return 1;
	}
	
	return 0;
}

bool renameFile(const char *name) {
	
	char newNameBuffer[10] = {0};
	char message[25] = {"Rename "};
	
	strcat(message, name);
	
	if(inputString(newNameBuffer, 8, message) > 0) {
		ti_Rename(name, newNameBuffer);
		return true;
	}
	
	return false;
}

bool alert(char *txt) {
	
	// put this first to get the font's height and use that to calculate the window height to display 5 lines
	fontlib_SetLineSpacing(0, 0);
	uint8_t fontHeight = fontlib_GetCurrentFontHeight();
	
	// window vars
	uint8_t maxLines = 4;
	int width = 150; 
	int height = (2*fontHeight)+(maxLines*fontHeight); // height of the window. has 2 extra line spaces for a header
	int x = SCRN_WIDTH/2 - width/2;
	int y = SCRN_HEIGHT/2 - height/2;
	
	// text vars
	int headerX;
	int headerY;
	
	char *readPos = txt;
	int txtX = x;
	int txtY = y + fontHeight;
	int strWidth;
	
	// other
	uint8_t linesPrinted = 0;
	int messageLen = strlen(txt);
	int charsRead = 0;
	
	// font stuff
	fontlib_SetWindow(x, y, width, height);
	fontlib_SetAlternateStopCode(' ');
	fontlib_SetCursorPosition(txtX, txtY);
	fontlib_SetBackgroundColor(LIGHT_GREY);
	fontlib_SetTransparency(true);
	
	gfx_SetDraw(1);
	
	// box with outline
	gfx_SetColor(LIGHT_GREY);
	gfx_FillRectangle_NoClip(x, y, width, height);
	gfx_SetColor(LIGHT_BLUE);
	thick_Rectangle(x-2, y-2, width + 4, height + 4, 2);
	
	// header text
	const char *headerTxt = "Warning";
	headerY = y-1;
	headerX = (SCRN_WIDTH/2) - (fontlib_GetStringWidth(headerTxt)/2);
	fontlib_SetCursorPosition(headerX, headerY);
	fontlib_SetForegroundColor(RED);
	fontlib_DrawString(headerTxt);
	
	// header line
	uint8_t length = 100;
	gfx_SetColor(LIGHT_BLUE);
	gfx_HorizLine(SCRN_WIDTH/2-length/2, y+fontHeight-1, length);
	gfx_HorizLine(SCRN_WIDTH/2-length/2, y+fontHeight, length);
	
	// reset the cursor position
	fontlib_SetCursorPosition(txtX, txtY);
	fontlib_SetForegroundColor(BLACK);
	
	while(linesPrinted < maxLines-1 && charsRead <= messageLen) {
	
		strWidth = fontlib_GetStringWidth(readPos);
	
		// if the string is short enough to be displayed... then display it!
		if(strWidth + txtX < width + x) {
			fontlib_DrawString(readPos);
			fontlib_ShiftCursorPosition(2, 0);
			charsRead += (fontlib_GetLastCharacterRead()+1) - readPos;
			readPos = fontlib_GetLastCharacterRead()+1;
			txtX += strWidth;
		}
		
		// if the word won't fit on to the end of the line... then create a new line
		if(txtX > x && strWidth + txtX > x + width) {
			if(linesPrinted < maxLines) {
				fontlib_Newline();
				txtX = x;
				txtY += fontHeight;
				linesPrinted++;
			} else {
				break;
			}
		}
		
		// if some person was fooling around and the current word is too long for a whole line by itself... then print it until it hits the endge of the window (the default) and create a new line
		if(txtX == x && strWidth > width) {
			
			fontlib_DrawString(readPos);
			
			charsRead += (fontlib_GetLastCharacterRead()+1) - readPos;
			readPos = fontlib_GetLastCharacterRead()+1;
			
			// new line
			if(linesPrinted > maxLines)
				break;
				
			fontlib_Newline();
			txtX = x;
			txtY += fontHeight;
			linesPrinted++;
		}
	}
	
	gfx_BlitRectangle(1, x-2, y-2, width+4, height+4);
		
	if (checkForInput() == true)
		return 1;
	
	return 0;
}

int displayMenu(struct menu * menu) {
	int offset = 0;
	int selected = 0;
	uint8_t spacing = 22;
	uint8_t maxOnScrn = 5;
	uint8_t width = 120;
	int height = spacing * maxOnScrn + 4;
	
	while(true) {
		gfx_SetDraw(1);
		
		// box
		gfx_SetColor(WHITE);
		gfx_FillRectangle_NoClip(menu->x, menu->y, width, height);
		
		// outline
		gfx_SetColor(LIGHT_BLUE);
		thick_Rectangle(menu->x, menu->y, width, height, 2);
		
		// draw the entries' sprites and text
		
		gfx_SetTextFGColor(BLACK);
		
		for(uint8_t i = offset; i < menu->numOptions && i < maxOnScrn; i++) {
		
			// rectangle selecting box
			if(i == selected) {
				// fill box
				gfx_SetColor(LIGHT_GREY);
				gfx_FillRectangle_NoClip(menu->x+2, menu->y + i * spacing + 2, width - 4, spacing);
				
				// outline box
				gfx_SetColor(BLACK);
				gfx_Rectangle_NoClip(menu->x+2, menu->y + i * spacing + 2, width - 4, spacing);
			}
			
			// text
			gfx_PrintStringXY(menu->entry[i].str, (menu->x + 30), menu->y + (i * spacing) + 10); // add 10 to center the text
			
			// sprites
			if(menu->hasSprites)
				gfx_TransparentSprite_NoClip(menu->entry[i].sprite, menu->x + 4, menu->y + i * spacing + 5);
			
		}
		
		gfx_BlitRectangle(1, menu->x, menu->y, width, height); // I might change this to just blitting the menu rect, but who knows if anybody is even going to ever read this comment...
		
		// keyPresses
		
		// move selecter bar down
		kb_Scan();
		if(kb_IsDown(kb_KeyDown) && selected < menu->numOptions -1) {
			selected++;
			if(selected > offset+maxOnScrn){
         	offset++;
      	}
			delay(100);
		}
		
		// move selecter bar up
		if(kb_IsDown(kb_KeyUp) && selected>0) {
			selected--;
			if(selected < offset){
         	offset--;
      	}
			delay(100);
		}
		
		// select an option
		if(kb_IsDown(kb_KeyEnter) || kb_IsDown(kb_Key2nd)) {
			return selected + 1;
		}
		
		// quit the menu
		if(os_GetCSC() == sk_Clear) {
			return CANCEL;
		}
		
	}
	
	return 0;
};

/*
loads the data into the struct for the homescreen menu that is triggered by the "other" (literally) jacobly recommended allocating space for a struct instead of doing that in the keypress function
*/
static const struct menu *loadHomeScreenOtherMenu(void) {
	static const struct menu menu = { 
		.title = "Options",
		.x = 200, .y = 100,
		.numOptions = 5,
		.hasSprites = true,
		
		// array of menu entries
		.entry = {
			{"Back", left_arrow, left_arrow_height},
			{"Rename", rename, rename_height},
			{"(un)Hide", hide, hide_height},
			{"Settings", settings, settings_height},
			{"Help", help, help_height},
		},
		
	};
	
	return &menu;
};

int drawScrollbar(struct scrollBar * scrollBar) {
	gfx_SetColor(scrollBar->colorIndex);
	
	for(uint8_t i=0; i<scrollBar->width; i++) {
		gfx_VertLine_NoClip(scrollBar->x + i, scrollBar->y, scrollBar->height);
	}
	
	return 0;
};

// Epsilon5++
bool checkForInput() {
	while (kb_AnyKey()) kb_Scan();

	while(true) {
		kb_Scan();
		
		if(kb_IsDown(kb_Key2nd) || kb_IsDown(kb_KeyEnter)){
			while (kb_AnyKey()) kb_Scan();
			return true;
		}
		
		if(kb_IsDown(kb_KeyClear)){
			while (kb_AnyKey()) kb_Scan();
			return false;
		}
	}
	
	return false;
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

// Congrats, you actually got to the bottom of this file! Did you actually read everything? :P
