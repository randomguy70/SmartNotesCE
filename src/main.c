/*
////////////////////////////////////////
// { SmartNotes CE } { Beta } {}
// Author: JohnPaul Malloy (Randomguy)
// Description: A text editor for the TI 84 Plus CE
////////////////////////////////////////
*/

#include <keypadc.h>
#include <fileioc.h>
#include <fontlibc.h>
#include <stdbool.h>
#include <stdint.h>
#include <graphx.h>
#include <string.h>

#include "includes/editor.h"
#include "includes/homescreen.h"
#include "gfx/gfx.h"
#include "includes/ui.h"

static uint8_t setupFontlibc();
static void cleanup();

int main(void) {
		
	if(!setupFontlibc())
	{
		return 0;
	}
	
	gfx_Begin();
	gfx_SetPalette(palette, sizeof_palette, 0);
	gfx_SetTransparentColor(2);
	gfx_SetTextTransparentColor(2);
	gfx_SetTextBGColor(2);
	
	struct homescreen homescreen;
	struct editor editor;
	enum state state = show_homescreen;
	
	state = show_homescreen;
	
	while (true)
	{
		if(state == should_exit)
		{
			break;
		}
		
		if(state == show_homescreen)
		{
			state = dispHomeScreen(&homescreen);
		}
		
		if(state == show_editor)
		{
			strcpy(editor.file.os_name, homescreen.files[homescreen.selectedFile].os_name);
			state = dispEditor(&editor);
		}
	}
	
	cleanup();
	return 0;
}

static uint8_t setupFontlibc() {
	fontlib_font_t *my_font;
	
	// Get the first font present in the font pack
	my_font = fontlib_GetFontByIndex("DrSans", 0);
	if (!my_font) {
		gfx_Begin();
		gfx_SetTextFGColor(BLACK);
		gfx_SetTextXY(0, 1);
		gfx_PrintString("DRSANS appvar not found or invalid :(");
		gfx_SetTextXY(-1, 15);
		gfx_PrintString("It was in the same folder my program came in but");
		gfx_SetTextXY(0, 30);
		gfx_PrintString("if you can't find it, ask for help on Cemetech,");
		gfx_SetTextXY(0, 45);
		gfx_PrintString("or email me at onlychessfreeze0515@gmail.com.");
		
		// make sure that a key was pressed AFTER all the keys were released
		while(kb_AnyKey()){}
		while(!kb_AnyKey()){}
		
		gfx_End();
		return false;
	}
	
	fontlib_SetFont(my_font, 0);
	fontlib_SetBackgroundColor(2);
	fontlib_SetTransparency(true);
	
	return true;
	
}

static void cleanup() {
	gfx_End();
	archiveAll();		
}