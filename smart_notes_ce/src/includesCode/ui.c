#include "main.h"

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

void fontlib_DrawStringXY(char *str, int x, int y) {
	fontlib_SetCursorPosition(x, y);
	fontlib_DrawString(str);
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
	
	if (waitForInput() == true)
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

int drawScrollbar(struct scrollBar * scrollBar) {
	gfx_SetColor(scrollBar->colorIndex);
	
	for(uint8_t i=0; i<scrollBar->width; i++) {
		gfx_VertLine_NoClip(scrollBar->x + i, scrollBar->y, scrollBar->height);
	}
	
	return 0;
};

// Epsilon5++
bool waitForInput() {
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

// Congrats, you actually got to the bottom of this file! Did you actually read everything? :P
