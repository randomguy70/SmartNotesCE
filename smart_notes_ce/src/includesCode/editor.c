#include <includes/editor.h>

#include <fontlibc.h>
#include <graphx.h>
#include <keypadc.h>
#include <fontlibc.h>
#include <fileioc.h>

#include <includes/file.h>
#include <includes/text.h>
#include <includes/characters.h>
#include <includes/ui.h>
#include <includes/colors.h>

static void dispEditorBK(char* name);
static void dispEditorText(struct editor* editor);
static enum state handleEditorKeyPresses(void);
int getLinePtrs(struct buffer* buffer);

enum state dispEditor(struct editor *editor) {
	
	enum state ret = show_editor;
	
	editor->curCol = 0;
	editor->curLine = 0;
	editor->editOffset = 0;
	
	editor->textBox.width = EDITOR_TEXT_BOX_WIDTH;
	editor->textBox.height = EDITOR_TEXT_BOX_HEIGHT;
	editor->textBox.x = SCRN_WIDTH/2 - EDITOR_TEXT_BOX_WIDTH/2;
	editor->textBox.y = SCRN_HEIGHT/2 - EDITOR_TEXT_BOX_HEIGHT/2;
	
	ti_var_t fileSlot = ti_Open(editor->fileName, "r+");
	if(!fileSlot)
	{
		return should_exit;
	}
	editor->file.size = ti_GetSize(fileSlot);
	ti_Read(editor->buffer.data, editor->file.size, 1, fileSlot);
	calculateLinePointers(&editor->textBox);
	
	while(true)
	{
		dispEditorBK(editor->fileName);
		dispEditorText(editor);
		
		ret = handleEditorKeyPresses();
		
		if(ret == should_exit || show_homescreen)
		{
			return ret;
		}
	}
}

// displays the editor background
static void dispEditorBK(char* fileName) {
	
	gfx_SetDraw(gfx_buffer);
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
	fontlib_DrawStringXY(fileName, 1, 2);
	
	// footer
	gfx_SetColor(MEDIUM_GREY);
	gfx_FillRectangle_NoClip(0, 220, SCRN_WIDTH, 20);
	gfx_SetColor(BLACK);
	gfx_HorizLine_NoClip(0, 220, SCRN_WIDTH);
		
	return;	
}

static void dispEditorText(struct editor* editor) {
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
	for(uint8_t i = 0; i<editor->buffer.numLines; i++) {
		
		uint8_t strLen = getByteDifference(editor->buffer.lines[i], editor->buffer.lines[i]);
		
		fontlib_DrawStringL(editor->buffer.lines[i], strLen);
		fontlib_Newline();
	}
	
	return;
}

static enum state handleEditorKeyPresses(void) {
	kb_Scan();
	
	if(kb_IsDown(kb_KeyClear)) {
		return should_exit;
	}
	
	if(kb_IsDown(kb_Key2nd)) {
		alert("Well well, it worked!!!");
		return show_editor;
	}
	
	return show_editor;
}

/*
int getLinePtrs(struct buffer* buffer) {
	
	char* readPos = buffer->data; // acts like a cursor in the buffer
	char* dataEnd = readPos + buffer->sig_chars;
	
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
	buffer->lines[linesRead] = readPos;
	
	while(readPos < dataEnd && linesRead < maxLinesViewable) {
		
		strWidth = fontlib_GetStringWidth(readPos);
		strLen   = getWordLen(readPos);
		
		if(readPos + strLen > buffer->data+buffer->sig_chars)
		{
			strLen = getByteDifference(readPos, buffer->data+buffer->sig_chars);
		}
		
		if(*readPos == NEW_LINE)
		{
			
			// record the address of the new line
			buffer->lines[linesRead++] = ++readPos;
			
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
				
				buffer->lines[linesRead++] = readPos++;
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
			buffer->lines[linesRead] = readPos;
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
	
	return buffer->numLines;
}

*/

