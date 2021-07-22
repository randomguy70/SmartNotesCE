#include "main.h"


// btw, HS stands for homescreen
uint8_t dispHomeScreen() {
   // set up struct for homescreen variables & data
   struct fileViewerStruct HS;
   HS.selectedFile = 0;
   HS.offset = 0;
   HS.numFiles = loadFiles(&HS);
	HS.shouldQuit = false;
	archiveAll();
	
	uint8_t result = 1;

   while(true) {
      dispHomeScreenBG(&HS);
      dispHSButtons();
		
      dispFiles(&HS);
		
		gfx_Wait();
		gfx_SwapDraw();
		
      result = handleHomeScrnKeyPresses(&HS);
		
		if(result == QUIT)
			return QUIT;
			
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

   // new file
   if (kb_IsDown(kb_KeyWindow) && HS->numFiles<30) {
		newFile();
		loadFiles(HS);
   }
	
	// quit program
	sk_key_t key = os_GetCSC();
	
	if (key == sk_Clear || key == sk_Zoom)
		return QUIT;
	
	// delete file
   if ((kb_IsDown(kb_KeyTrace) || kb_IsDown(kb_KeyDel)) && HS->numFiles>0) {
      checkIfDeleteSelected(HS);
		loadFiles(HS);
   }
	
	// other (opens fun menu with sprites)
	if(kb_IsDown(kb_KeyGraph)) {
		
		struct menu *menu = loadHomeScreenOtherMenu();
		
		uint8_t result = displayMenu(menu);
			
		/* --Options--
			back, rename, hide, settings, help (start at 1 as an offset because 0 signifies quit)
		*/
		switch(result) {
			
			// 
			case QUIT:
				return QUIT;
				
			// back
			case 1:
				return 1;
			
			// rename
			case 2:
				renameFile(HS->fileNames[HS->selectedFile]);
				loadFiles(HS);
				return 2;
				
			// hide
			case 3: 
				// hideFile(char* name); // haven't defined this yet btw...
				return 3;
				
			// settings
			case 4:
				// displaySettings();
				return 4;
				
			// quit
			default:
				return QUIT;
			
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
uint8_t checkIfDeleteSelected(struct fileViewerStruct *HS ) {
	uint8_t keyPressed = 0;
	
	// the alert function uses drawing routines, so you have to set the draw buffer
	gfx_SetDraw(0);
	
	uint16_t width = 200;
	uint16_t height = 50;
	
	// text box (literally a rectangle)
	gfx_SetColor(MEDIUM_GREY);
	gfx_FillRectangle((320/2)-(width/2), (240/2)-(height/2), width, height);
	
	gfx_SetColor(DARK_BLUE);
	thick_Rectangle((320/2)-(width/2), (240/2)-(height/2), width, height, 2);
	
	gfx_SetTextFGColor(RED);
	gfx_PrintStringXY("ALERT!", (320/2)-(gfx_GetStringWidth("ALERT!")/2), (240/2)-(height/2) + 5);
	
	// bar under "ALERT!"
	gfx_SetColor(DARK_BLUE);
	gfx_HorizLine((SCRN_WIDTH/2)-(100/2), (240/2)-(height/2) + 14, 100);
	gfx_HorizLine((SCRN_WIDTH/2)-(100/2), (240/2)-(height/2) + 15, 100);
	
	gfx_SetTextFGColor(BLACK);
	gfx_PrintStringXY("Are you sure you want to ", (320/2)-(width/2) + 13, (240/2)-(height/2) + 18);
	gfx_PrintStringXY("delete the file ", (320/2)-(width/2) + 13, (240/2)-(height/2) + 33);
	
	gfx_SetTextFGColor(DARK_BLUE);
	gfx_PrintString(HS->fileNames[HS->selectedFile]);
	
	gfx_SetTextFGColor(BLACK);
	gfx_PrintChar('?');
	
	// obviously, to see the graphics, you have to swap the buffers
	gfx_SwapDraw();
	
	// wait for keypress
	while(keyPressed != sk_Clear && keyPressed != sk_2nd && keyPressed != sk_Enter) {
		keyPressed = os_GetCSC();
	}
	
	// if the user doesn't want to delete the file...
	if(keyPressed == sk_Clear) {
		return 0;
	}
	
	// if the user wants to delete the file...
	if(keyPressed == sk_2nd || keyPressed == sk_Enter) {
		ti_Delete(HS->fileNames[HS->selectedFile]);
		
		if(HS->selectedFile>0 && HS->numFiles>10) {
         HS->selectedFile--;		
         HS->offset--;
      }
		
		return 1;
	}
	
	return 0;
}

bool renameFile(const char *name) {
	uint16_t width = 200;
	uint16_t height = 50;
	
	gfx_SetDraw(1);
	
	// text box (literally a rectangle)
	gfx_SetColor(MEDIUM_GREY);
	gfx_FillRectangle((320/2)-(width/2), (240/2)-(height/2), width, height);
	
	// draw a blue outline for the text box
	gfx_SetColor(DARK_BLUE);
	thick_Rectangle((320/2)-(width/2), (240/2)-(height/2), width, height, 2);
	
	// print "alert", because this is an alert...
	gfx_SetTextFGColor(RED);
	gfx_PrintStringXY("ALERT!", (320/2)-(gfx_GetStringWidth("ALERT!")/2), (240/2)-(height/2) + 5);
	
	// bar under "ALERT!"
	gfx_SetColor(DARK_BLUE);
	gfx_HorizLine((SCRN_WIDTH/2)-(100/2), (240/2)-(height/2) + 14, 100);
	gfx_HorizLine((SCRN_WIDTH/2)-(100/2), (240/2)-(height/2) + 15, 100);
	
	// print message relative to the given width
	gfx_SetTextFGColor(BLACK);
	gfx_PrintStringXY("Are you sure you want to ", (SCRN_WIDTH/2)-(width/2) + 13, (SCRN_HEIGHT/2)-(height/2) + 18);
	gfx_PrintStringXY("rename the file ", (SCRN_WIDTH/2)-(width/2) + 13, (SCRN_HEIGHT/2)-(height/2) + 33);
	
	// print the file's name
	gfx_SetTextFGColor(DARK_BLUE);
	gfx_PrintString(name);
	
	// add a question mark
	gfx_SetTextFGColor(BLACK);
	gfx_PrintChar('?');
	
	gfx_SwapDraw();
	
	sk_key_t keyPressed = 0;
	while(keyPressed != sk_Clear && keyPressed != sk_2nd && keyPressed != sk_Enter) {
		keyPressed = os_GetCSC();
	}
	
	if(keyPressed == sk_Clear) {
		return false;
	}
	
	char inputNameBuffer[10] = {0};
	char message[25] = {"Rename "};
	
	strcat(message, name);
	strcat(message, "?");
	
	if(inputString(inputNameBuffer, 8, message) > 0) {
		ti_Rename(name, inputNameBuffer);
		return true;
	}
	
	return false;
}

bool alert(char *txt) {
	// put this first to get the font's height and use that to calculate the window height to display 5 lines
	fontlib_SetLineSpacing(2, 2);
	uint8_t fontHeight = fontlib_GetCurrentFontHeight();
	
	// window vars
	uint8_t maxLines = 4;
	int width = 150; 
	int height = (2*fontHeight)+(maxLines*fontHeight); // height of the window. has 2 extra line spaces for a header
	int x = SCRN_WIDTH/2 - width/2;
	int y = SCRN_HEIGHT/2 - height/2;
	
	// text vars
	char* readPos = txt;
	int txtX = x;
	int txtY = y + 2*fontHeight;
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
	fontlib_SetTransparency(0);
	
	gfx_SetDraw(1);
	
	// box with outline
	gfx_SetColor(LIGHT_GREY);
	gfx_FillRectangle_NoClip(x, y, width, height);
	gfx_SetColor(LIGHT_BLUE);
	thick_Rectangle(x-2, y-2, width + 4, height + 4, 2);
	
	while(linesPrinted < maxLines && charsRead < messageLen) {
	
		strWidth = fontlib_GetStringWidth(readPos);
		
		// quit printing if the message is ended, need to modify this to count the strlen too
		if(readPos == NULL)
			break;
			
		// if the first character of the read line is the new line code..., then create a new line OBVS
		if(readPos == NEW_LINE) {
			if(linesPrinted<maxLines) {
				fontlib_Newline();
				txtY += fontHeight;
				txtX = x;
				readPos++;
				charsRead++;
				linesPrinted++;
			} else {
				break;
			}
		}
		
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
			
			// draw it
			fontlib_DrawString(readPos);
			charsRead += (fontlib_GetLastCharacterRead()+1) - readPos;
			readPos = fontlib_GetLastCharacterRead()+1;
			
			// new line
			if(linesPrinted >= maxLines)
				break;
				
			fontlib_Newline();
			txtX = x;
			txtY += fontHeight;
			linesPrinted++;
		}
	}
	
	gfx_Blit(1);
	while(!os_GetCSC());
	return 1;
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
		
		// gfx_Wait();
		gfx_Blit(1); // I might change this to just blitting the menu rect, but who knows if anybody is even going to ever read this comment...
		
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
			return 0;
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
