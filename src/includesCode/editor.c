#include <fontlibc.h>
#include <graphx.h>
#include <keypadc.h>
#include <fontlibc.h>
#include <fileioc.h>

#include "main.h"
#include "includes/editor.h"
#include "includes/file.h"
#include "includes/text.h"
#include "includes/characters.h"
#include "includes/ui.h"
#include "includes/colors.h"

static int initialiseEditor(struct editor *editor);
static void dispEditorBK();
// static void dispEditorText(struct editor* editor);
static enum state handleEditorKeyPresses(void);
static unsigned int getVisibleLinePtrs(struct editor *editor);

enum state dispEditor(struct editor *editor) {
	
	enum state ret = show_editor;
	
	initialiseEditor(editor);
	
	// getVisibleLinePtrs(editor);
	
	while(true)
	{
		
		gfx_SetDraw(gfx_buffer);
		dispEditorBK(editor);
		gfx_Blit(gfx_buffer);
		
		kb_Scan();
		
		if(kb_IsDown(kb_KeyClear))
		{
			while(kb_AnyKey()) kb_Scan();
			return show_homescreen;
		}
		
		if(ret == should_exit)
		{
			return should_exit;
		}
	}
	
	return ret;
}

static int initialiseEditor(struct editor *editor)
{
	fileToBuffer(editor->file.os_name, &(editor->buffer));
	
	editor->textBox.curLine = 0;
	editor->textBox.curCol = 0;
	editor->textBox.lineOffset = 0;
	
	return 1;
}

// XXX 
static unsigned int getVisibleLinePtrs(struct editor *editor)
{
	unsigned int numVisibleLines = 0;
	
	return numVisibleLines;
}

// displays the editor background
static void dispEditorBK(struct editor *editor)
{
	int strWidth; // used for centering strings
	
	gfx_FillScreen(WHITE);
	
	// header rectangle fill
	gfx_SetColor(LIGHT_GREY);
	gfx_FillRectangle_NoClip(EDITOR_HEADER_BAR_X, EDITOR_HEADER_BAR_Y, EDITOR_HEADER_BAR_WIDTH, EDITOR_HEADER_BAR_HEIGHT);
	
	// header rect. outline
	gfx_SetColor(BLACK);
	gfx_HorizLine_NoClip(EDITOR_HEADER_BAR_X, EDITOR_HEADER_BAR_Y + EDITOR_HEADER_BAR_HEIGHT - 1, EDITOR_HEADER_BAR_WIDTH);
	
	// header text
	fontlib_SetWindowFullScreen();
	fontlib_SetTransparency(true);
	fontlib_SetForegroundColor(BLACK);
	fontlib_SetAlternateStopCode(0);
	
	strWidth = fontlib_GetStringWidth(editor->file.os_name);
	fontlib_DrawStringXY(editor->file.os_name, EDITOR_HEADER_BAR_X + (EDITOR_HEADER_BAR_WIDTH / 2) - (strWidth / 2), 5);
	
	// footer
	gfx_SetColor(LIGHT_GREY);
	gfx_FillRectangle_NoClip(EDITOR_FOOTER_BAR_X, EDITOR_FOOTER_BAR_Y, EDITOR_FOOTER_BAR_WIDTH,  EDITOR_FOOTER_BAR_HEIGHT );
	gfx_SetColor(BLACK);
	gfx_HorizLine_NoClip(EDITOR_FOOTER_BAR_X, EDITOR_FOOTER_BAR_Y, EDITOR_FOOTER_BAR_WIDTH);
		
	return;
}

// XXX Display Editor Text
/*
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
*/

// XXX
static enum state handleEditorKeyPresses(void)
{
	if(kb_IsDown(kb_KeyClear)) {
		return should_exit;
	}
	
	if(kb_IsDown(kb_Key2nd)) {
		return show_homescreen;
	}
	
	return show_editor;
}