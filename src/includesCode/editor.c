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
static void refreshEditorGraphics(struct editor *editor);
static void dispEditorBK();
static void displayEditorButtons(void);
static enum state handleEditorKeyPresses(struct editor *editor);
static unsigned int getVisibleLinePtrs(struct editor *editor);

enum state dispEditor(struct editor *editor) {
	
	enum state ret;
	
	// initialiseEditor(editor);
	
	while(true)
	{
		kb_Scan();
		refreshEditorGraphics(editor);
		
		ret = handleEditorKeyPresses(editor);
		
		if(ret == should_exit)
		{
			return should_exit;
		}
		if(ret == show_homescreen)
		{
			return show_homescreen;
		}
	}
	return ret;
}

static int initialiseEditor(struct editor *editor)
{
	// fileToBuffer(editor->file.os_name, &(editor->buffer));
	
	editor->textBox.curLine = 0;
	editor->textBox.curCol = 0;
	editor->textBox.lineOffset = 0;
	
	return 1;
}

// XXX 
static unsigned int getVisibleLinePtrs(struct editor *editor)
{
	unsigned int numVisibleLines = 0;
	int lineLen;
	
	editor->linePtr[0] = editor->buffer.data + editor->buffer.dataLenBeforeCursor - 1;
	
	return numVisibleLines;
}

static void refreshEditorGraphics(struct editor *editor)
{
	gfx_SetDraw(gfx_buffer);
	dispEditorBK(editor);
	displayEditorButtons();
	gfx_SwapDraw();
}

// displays the editor background
static void dispEditorBK(struct editor *editor)
{
	int strWidth;
	
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
	
	return;
}

static void displayEditorButtons(void)
{
	int i, x;
	
	// footer
	gfx_SetColor(LIGHT_GREY);
	gfx_FillRectangle_NoClip(EDITOR_FOOTER_BAR_X, EDITOR_FOOTER_BAR_Y, EDITOR_FOOTER_BAR_WIDTH,  EDITOR_FOOTER_BAR_HEIGHT );
	
	gfx_SetColor(BLACK);
	gfx_HorizLine_NoClip(EDITOR_FOOTER_BAR_X, EDITOR_FOOTER_BAR_Y, EDITOR_FOOTER_BAR_WIDTH);
	
	const char *text[5] = {"Home", "Edit", "Save", "Unused", "File"};
	const uint8_t keys[5] = {kb_Yequ, kb_Window, kb_Zoom, kb_Trace, kb_Graph};
	
	fontlib_SetForegroundColor(BLACK);
	
	for(i = 0; i < NUM_EDITOR_BUTTONS; i++)
	{
		if(kb_Data[1])
		{
			if(kb_Data[1] & keys[i])
			{
				gfx_SetColor(LIGHT_BLUE);
				gfx_FillRectangle_NoClip(i * EDITOR_BUTTON_SPACING, LCD_HEIGHT - 24, EDITOR_BUTTON_SPACING, 24);
			}
		}
		
		x = (i * EDITOR_BUTTON_SPACING) + (EDITOR_BUTTON_SPACING / 2) - (fontlib_GetStringWidth(text[i]) / 2);
		fontlib_DrawStringXY(text[i], x, LCD_HEIGHT - 19);
	}
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
static enum state handleEditorKeyPresses(struct editor *editor)
{
	if(kb_IsDown(kb_KeyClear))
	{
		while(kb_IsDown(kb_KeyClear)) kb_Scan();
		return should_exit;
	}
	
	if(kb_IsDown(kb_Key2nd))
	{
		while(kb_IsDown(kb_Key2nd)) kb_Scan();
		return show_homescreen;
	}
	
	return show_editor;
}