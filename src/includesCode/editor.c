#include "main.h"
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

static void dispEditorBK();
static void dispEditorText(struct editor* editor);
static enum state handleEditorKeyPresses(void);
int getLinePtrs(struct buffer* buffer);

enum state dispEditor(struct editor *editor) {
	
	enum state ret = show_editor;
	
	editor->curLine = 0;
	editor->curCol = 0;
	editor->editOffset = 0;
	
	initialiseEditor(editor);
	
	getTextBoxLinePointers(editor);
	
	while(true)
	{
		dispEditorBK(editor);
		dispEditorText(editor);
		
		ret = handleEditorKeyPresses();
		
		if(ret == should_exit || show_homescreen)
		{
			return ret;
		}
	}
}

static int initialiseEditor(struct editor *editor)
{
	fileToBuffer(editor->file.os_name, &(editor->buffer));
	
	
	editor->textBox.width = EDITOR_TEXT_BOX_WIDTH;
	editor->textBox.height = EDITOR_TEXT_BOX_HEIGHT;
	editor->textBox.x = SCRN_WIDTH/2 - EDITOR_TEXT_BOX_WIDTH/2;
	editor->textBox.y = SCRN_HEIGHT/2 - EDITOR_TEXT_BOX_HEIGHT/2;
	
	editor->textBox.maxLinesOnScreen = EDITOR_MAX_LINES_VIEWABLE;
	editor->textBox.lineOffset = 0;
	 
	return 1;
}

static int getTextBoxLinePointers(struct textBox *textBox)
{
	char *end = textBox->startOfText + textBox->textLength;
	char *readPos = textBox->startOfText;
	char *curLine = textBox->startOfText;
	int linesCounted = 0;
	// pixel width
	int curLineWidth = 0;
	// character count
	int curLineLen = 0;
	// pixel width
	int curWordWidth = 0;
	// character count
	int curWordLen = 0;
	
	fontlib_SetAlternateStopCode(0);
	
	textBox->maxLinesOnScreen = 5;
	
	// initialise the first line
	textBox->linePointers[linesCounted] = curLine;
	
	while(true)
	{
		START:
		
		curWordLen = getWordLen(readPos, end);
		curWordWidth = fontlib_GetStringWidthL(readPos, curWordLen);
		
		// if it reaches the end of the data => return
		if(readPos >= end || linesCounted > textBox->maxLinesOnScreen)
		{
			break;
		}
		
		// make sure it doesn't overflow
		if (readPos + curWordLen >= end)
		{
			curWordLen = end - readPos;
		}
		
		// if word fits => append it
		if(curLineWidth + curWordWidth < textBox->width)
		{
			readPos += curWordLen;
			curLineLen += curWordLen;
			curLineWidth += curWordWidth;
			goto START;
		}
		
		// if word doesn't fit and there is stuff on the line already => new line
		if((curLineWidth + curWordWidth < textBox->width) && (curLineLen > 0))
		{
			curLine += curLineLen;
			curLineLen = 0;
			curLineWidth = 0;
			textBox->linePointers[++linesCounted] = curLine;
			textBox->numLines++;
			
			goto START;
		}
		
		// if the word is too big for an entire line => force wrap it
		if((curLineWidth + curWordWidth < textBox->width) && (curLineLen == 0))
		{
			curLineLen = getMaxCharsPerLine(curLine, end);
			curLine += curLineLen;
			textBox->linePointers[++linesCounted] = curLine;
			goto START;
		}
	}
	
	textBox->numLinesOnScreen = linesCounted;
	return linesCounted;
}

// displays the editor background
static void dispEditorBK(struct editor *editor)
{
	
	gfx_SetDraw(gfx_buffer);
	gfx_FillScreen(WHITE);
	
	// header rectangle
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
		
	return;	
}

static void dispEditorText(struct editor *editor)
{
	uint8_t lineLen;
	char *line;
	
	fontlib_SetForegroundColor(BLACK);
	
	for(int i=0; i<editor->textBox.numLinesOnScreen; i++)
	{
		lineLen = editor->textBox.lineLengths[i];
		line = editor->textBox.linePointers[i];
		
		fontlib_SetCursorPosition(10, i*20 + 25);
		fontlib_DrawStringL(line, lineLen);
	}
	
	return;
}

static enum state handleEditorKeyPresses(void)
{
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