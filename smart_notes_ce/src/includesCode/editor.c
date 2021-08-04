#include "main.h"
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
static void dispEditorBK(struct editor *editor);
static uint8_t handleEditorKeyPresses(struct editor *editor);
int getLinePtrs(struct file *file);

uint8_t dispEditor(struct editor *editor) {
	editor->isRunning = true;
	editor->curCol = 0;
	editor->curLine = 0;
	editor->editOffset = 0;
	editor->selectedChars = 0;
	editor->shouldRefresh = 1;
	
	uint8_t action; // action triggered by keypresses
	
	loadFile(&(editor->file), editor->fileName);
	
	while(editor->shouldRefresh) {
		dispEditorBK(editor);
		
		action = handleEditorKeyPresses(editor);
		
		if(action == QUIT)
			return QUIT;
		if(action == HOME)
			return HOME;
	}
}

// displays the editor background
static void dispEditorBK(struct editor *editor) {
	
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
	fontlib_DrawStringXY(editor->file.os_name, 1, 2);
	
	// footer
	gfx_SetColor(MEDIUM_GREY);
	gfx_FillRectangle_NoClip(0, 220, SCRN_WIDTH, 20);
	gfx_SetColor(BLACK);
	gfx_HorizLine_NoClip(0, 220, SCRN_WIDTH);
	
	gfx_Blit(1);
	
	waitForInput();
	return;
	
}

static uint8_t handleEditorKeyPresses(struct editor *editor) {
	kb_Scan();
	
	editor->isRunning = true; // this is only to prevent the unused parameter warning
	
	if(kb_IsDown(kb_KeyClear)) {
		return QUIT;
	}
	
	if(kb_IsDown(kb_Key2nd)) {
		alert("Well well, it worked!!!");
		return CANCEL;
	}
	
	return CANCEL;
}

int getLinePtrs(char *array) {
	
	char *readPos = file->txtPtr;
	int linesRead = 0;
	// int charsRead = 0;
	
	int windowWidth = SCRN_WIDTH;
	
	int txtX = 0; // used to determine how long each line should be, relative to the given window dimensions
	int txtY = 0; // used to determine how long each line should be, relative to the given window dimensions

	fontlib_SetAlternateStopCode(' ');
	fontlib_SetLineSpacing(2, 2);
	uint8_t fontHeight = fontlib_GetCurrentFontHeight();
	int strWidth;
	
	while(true) {
		
		// new line character
		if(readPos == NEW_LINE) {
			file->linePtrs[linesRead] = readPos;
			fontlib_Newline();
			linesRead++;
			readPos++;
			txtX = 0;
			txtY += fontHeight;
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
			fontlib_Newline();
			linesRead++;
			txtX = 0;
			txtY += fontHeight;
			file->linePtrs[linesRead] = readPos;
		}
		
		// if nothing has been printed on to the line but the word is still too long for a whole line, then character-wrap i
		if(txtX <= 0 && strWidth > windowWidth) {
		}
	}
	
}
