#include "main.h"
#include <fontlibc.h>
#include <keypadc.h>
#include <graphx.h>
#include <fileioc.h>
#include <string.h>

#include "includes/text.h"
#include "includes/ui.h"
#include "tice.h"
#include "includes/key.h"

void drawWindow(struct window* window) {
	const unsigned int titleBarHeight = 15;
	const unsigned int outlineThickness = 2;
	
	// title bar
	gfx_SetColor(window->titleBarColor);
	gfx_FillRectangle_NoClip(window->x, window->y, window->width, titleBarHeight);
	
	fontlib_SetForegroundColor(window->titleTextColor);
	fontlib_DrawStringXY(window->title, (window->x + window->width/2)- (fontlib_GetStringWidth(window->title)/2), window->y + outlineThickness);
	
	// body
	gfx_SetColor(window->bodyColor);
	gfx_FillRectangle_NoClip(window->x, window->y + titleBarHeight, window->width, window->height - titleBarHeight);
	
	// outline
	gfx_SetColor(window->windowOutlineColor);
	thick_Rectangle(window->x, window->y, window->width, window->height, outlineThickness);
}

// cursor stuff
void updateCursor(struct cursor* cursor) {
	cursor->animation_cycles_completed++;
	
	if(cursor->animation_cycles_completed > cursor->cycles_per_animation) {
		cursor->animation_cycles_completed = 0;
	}
}

void drawCursor(struct cursor* cursor) {
	if(cursor->animation_cycles_completed >= (cursor->cycles_per_animation - cursor->invisibleTime)) {
		gfx_SetColor(LIGHT_BLUE);
		gfx_VertLine_NoClip(cursor->x, cursor->y, 13);
		gfx_VertLine_NoClip(cursor->x+1, cursor->y, 13);
	}
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
	{
		keyPressed = get_single_key_pressed();
	}
		
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

bool alert(char *txt) {
	
	// put this first to get the font's height and use that to calculate the window height to display 5 lines
	fontlib_SetLineSpacing(0, 0);
	uint8_t fontHeight = fontlib_GetCurrentFontHeight();
	
	// window vars
	uint8_t maxLines = 4;
	int width = 150; 
	int height = (2*fontHeight)+(maxLines*fontHeight); // height of the window. has 2 extra line spaces for a header
	int x = LCD_WIDTH/2 - width/2;
	int y = LCD_HEIGHT/2 - height/2;
	
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
	headerX = (LCD_WIDTH/2) - (fontlib_GetStringWidth(headerTxt)/2);
	fontlib_SetCursorPosition(headerX, headerY);
	fontlib_SetForegroundColor(RED);
	fontlib_DrawString(headerTxt);
	
	// header line
	uint8_t length = 100;
	gfx_SetColor(LIGHT_BLUE);
	gfx_HorizLine(LCD_WIDTH/2-length/2, y+fontHeight-1, length);
	gfx_HorizLine(LCD_WIDTH/2-length/2, y+fontHeight, length);
	
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

int drawScrollbar(struct scrollBar * scrollBar)
{
	gfx_SetColor(scrollBar->colorIndex);
	
	for(uint8_t i=0; i<scrollBar->width; i++) {
		gfx_VertLine_NoClip(scrollBar->x + i, scrollBar->y, scrollBar->height);
	}
	
	return 0;
};

// Epsilon5++
bool waitForInput()
{
	while (kb_AnyKey()) kb_Scan();

	while(true)
	{
		kb_Scan();
		
		if(kb_IsDown(kb_Key2nd) || kb_IsDown(kb_KeyEnter))
		{
			while (kb_AnyKey()) kb_Scan();
			return true;
		}
		
		if(kb_IsDown(kb_KeyClear))
		{
			while (kb_AnyKey()) kb_Scan();
			return false;
		}
	}
	
	return false;
}
