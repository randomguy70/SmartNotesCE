#include "main.h"
#include "main.c"
#include <fontlibc.h>
#include <graphx.h>
#include <keypadc.h>
#include <fontlibc.h>

#include <includes/file.h>
#include <includes/editor.h>
#include <includes/text.h>
#include <includes/characters.h>
#include <includes/ui.h>

//// declarations
static void dispEditorBK(void);
static void dispEditorFG(void);
static uint8_t handleEditorKeyPresses(void);
int getLinePtrs(struct file *file);

uint8_t dispEditor(void) {
	
	editor.shouldRefresh = true;
	editor.curCol        = 0;
	editor.curLine       = 0;
	editor.editOffset    = 0;
	
	clipboard.data = NULL;
	clipboard.size = 0;
	
	loadFile(&(editor.file), editor.fileName);
	
	uint8_t keyPressed;
	
	while(true) {
		if(editor.shouldRefresh) {
			dispEditorBK();
			dispEditorFG();
		}
		
		keyPressed = handleEditorKeyPresses();
		
		if(keyPressed == QUIT)
			return QUIT;
		if(keyPressed == HOME)
			return HOME;
	}
}

// displays the editor background
static void dispEditorBK(void) {
	
	gfx_SetDraw(1);
	gfx_FillScreen(WHITE);
	
	// header rect
	gfx_SetColor(MEDIUM_GREY);
	gfx_FillRectangle_NoClip(0, 0, SCRN_WIDTH, 20);
	gfx_SetColor(BLACK);
	gfx_HorizLine_NoClip(0, 20, SCRN_WIDTH);
	
	// header txt
	fontlib_SetTransparency(true);
	fontlib_SetForegroundColor(BLACK);
	fontlib_SetAlternateStopCode(0); // the name might have spaces in it
	fontlib_DrawStringXY(editor.file.os_name, 1, 2);
	
	// footer
	gfx_SetColor(MEDIUM_GREY);
	gfx_FillRectangle_NoClip(0, 220, SCRN_WIDTH, 20);
	gfx_SetColor(BLACK);
	gfx_HorizLine_NoClip(0, 220, SCRN_WIDTH);
	
	gfx_Blit(1);
	
	return;	
}

static void dispEditorFG(void) {
	uint8_t fontHeight;
	uint8_t numLinesOnScreen;
	int windowHeight;
	int windowXMin;
	int windowYMin;
	
	windowXMin = fontlib_GetWindowXMin();
	windowYMin = fontlib_GetWindowYMin();
	
	fontHeight = fontlib_GetCurrentFontHeight();
	windowHeight = fontlib_GetWindowHeight();
	numLinesOnScreen = windowHeight / fontHeight;
	
	fontlib_SetCursorPosition(windowXMin, windowYMin);
	
	// I will change this to be relative to the screen offset, just using this for debugging as of now
	for(uint8_t i = 0; i<editor.buffer.numLines; i++) {
		
		uint8_t strLen = getByteDifference(editor.buffer.lines[i], editor.buffer.lines[i]);
		
		fontlib_DrawStringL(editor.buffer.lines[i], strLen);
		fontlib_Newline();
	}
	
	return;
}

static uint8_t handleEditorKeyPresses(void) {
	kb_Scan();
	
	if(kb_IsDown(kb_KeyClear)) {
		return QUIT;
	}
	
	if(kb_IsDown(kb_Key2nd)) {
		alert("Well well, it worked!!!");
		return CANCEL;
	}
	
	return CANCEL;
}

int getLinePtrs(struct file *file) {
	
	char *readPos = file->buffer; // acts like a cursor in the buffer
	
	uint8_t windowWidth  = fontlib_GetWindowWidth();
	uint8_t windowHeight = fontlib_GetWindowHeight();
	
	int txtX   = 0;
	int txtY   = 0;
	
	int linesRead = 0;
	uint8_t maxLinesViewable;

	int strWidth;    // pixel length of the word starting at readPos
	int strLen;      // character length of the word starting at readPos
	
	// fontlib things
	fontlib_SetAlternateStopCode(' ');
	fontlib_SetLineSpacing(2, 2);
	uint8_t fontHeight = fontlib_GetCurrentFontHeight();
	
	maxLinesViewable = windowHeight / fontHeight;
	
	// record the first line pointer
	file->linePtrs[linesRead] = readPos;
	
	while(readPos < file->dataEnd && linesRead < maxLinesViewable) {
		
		strWidth = fontlib_GetStringWidth(readPos);
		strLen   = getWordLen(readPos);
		
		if(readPos + strLen > file->dataEnd)
			strLen = getByteDifference(readPos, file->dataEnd);
		
		if(*readPos == NEW_LINE) {
			
			// record the address of the new line
			file->linePtrs[linesRead++] = ++readPos;
			
			fontlib_Newline();
			
			txtX = 0;
			txtY += fontHeight;
			
		}
		
		if(*readPos == SPACE) {
			
			if(txtX + SPACE_WIDTH < windowWidth) {
				txtX += SPACE_WIDTH;
				readPos++;
			}
			else {
				
				file->linePtrs[linesRead++] = readPos++;
				fontlib_Newline();
				txtX = 0;
				txtY += fontHeight;
			}
			
		}
		
		// get the pixel length of the next word
		strWidth = fontlib_GetStringWidth(readPos);
		
		// if the word can fit on to the end of the line	
		if(strWidth + txtX <= windowWidth) {
			readPos += fontlib_GetStrLen(readPos) + 1; // the one extra counts for the space or null terminator
			txtX += strWidth + SPACE_WIDTH;
		}
		
		// if something has already been printed on the line and the next word can't fit on to the end of the line
		if(strWidth + txtX > windowWidth && txtX > 0) {
			if(linesRead > maxLinesViewable) {
				break;
			}
			
			fontlib_Newline();
			linesRead++;
			txtX = 0;
			txtY += fontHeight;
			file->linePtrs[linesRead] = readPos;
			linesRead++;
		}
		
		// if nothing has been printed on to the line but the word is still too long for a whole line, then force wrap it
		if(txtX <= 0 && strWidth > windowWidth) {
			if(linesRead > maxLinesViewable) {
				break;
			}
			
			fontlib_DrawStringL(readPos, getMaxCharsPerLine(readPos));
			
			readPos+=getMaxCharsPerLine(readPos);
			fontlib_Newline();
			txtX = 0;
			txtY += fontHeight;
			linesRead++;
		}
		
	}
	
	return file->numLines;
}
