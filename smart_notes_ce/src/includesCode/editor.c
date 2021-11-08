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
	ti_var_t file;
	
	file = ti_Open(editor->file.os_name, "r");
	
	if(!file)
	{
		return 0;
	}
	
	editor->buffer.size = ti_GetSize(file);
	editor->file.size = editor->buffer.size;
	
	ti_Read(editor->buffer.data, editor->buffer.size, 1, file);
	ti_Close(file);
	
	editor->textBox.startOfText = editor->buffer.data;
	editor->textBox.textLength = editor->buffer.size;
	
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
	
	// initialise the first line
	textBox->linePointers[linesCounted] = curLine;
	
	while(true)
	{
		START:
		
		curWordLen = getWordLen(readPos, end);
		curWordWidth = fontlib_GetStringWidthL(readPos, curWordLen);
		
		// if it reaches the end of the data => return
		if(readPos >= end)
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
	
	return linesCounted;
}

// displays the editor background
static void dispEditorBK(struct editor *editor)
{
	
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
	fontlib_DrawStringXY(editor->file.os_name, 1, 2);
	
	// footer
	gfx_SetColor(MEDIUM_GREY);
	gfx_FillRectangle_NoClip(0, 220, SCRN_WIDTH, 20);
	gfx_SetColor(BLACK);
	gfx_HorizLine_NoClip(0, 220, SCRN_WIDTH);
		
	return;	
}

static void dispEditorText(struct editor* editor)
{
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