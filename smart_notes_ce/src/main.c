/*
////////////////////////////////////////
// { SmartNotes CE } { 1.0 }
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

// checks for font appvar(s), loads font, and deals with technical font details
static uint8_t setupFontlibc();

// prepares for the program to exit
static void cleanup();
static bool setupAppvars();
static bool formatSaveStateAppvar(char *name);
static bool formatSettingsAppvar(char *name);
static bool formatUserInfoAppvar(char *name);

void main() {
	
	ti_CloseAll();
	
	if(!setupFontlibc())
		return;
	
	// necessary gfx stuff
	gfx_Begin();
	gfx_SetPalette(palette, sizeof_palette, 0);
	gfx_SetTransparentColor(2);
	gfx_SetTextTransparentColor(2);
	gfx_SetTextBGColor(2);

	// checks for the data appvars. Creates new ones if necessary
	setupAppvars();
	
	struct homescreen homescreen;
	struct editor editor;
	enum state state = show_homescreen;
	enum state prevState = state;

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
			if(prevState != state) {
				strcpy (editor.fileName, homescreen.files[homescreen.selectedFile].os_name);
				prevState = state;
			}
			state = dispEditor(&editor);
		}
	}

	// cleanup();
	gfx_End();
	(*(volatile uint8_t*)0xF00008) = 1;
	return;
}

static uint8_t setupFontlibc() {
	fontlib_font_t *my_font;
	
	// Get the first font present in the font pack
   my_font = fontlib_GetFontByIndex("DRSANS", 0);
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
      	return 0;
   }
	
   // Use font for whatever
   fontlib_SetFont(my_font, 0);
	fontlib_SetBackgroundColor(2);
	fontlib_SetTransparency(true);
	
	return true;
	
}

static bool setupAppvars() {
	
	ti_var_t saveStateAppvarSlot;
	ti_var_t settingsAppvarSlot;
	ti_var_t userInfoAppvarSlot;
	
	saveStateAppvarSlot = ti_Open(SAVE_STATE_APPVAR_NAME, "r");
	settingsAppvarSlot  = ti_Open(SETTINGS_APPVAR_NAME  , "r");
	userInfoAppvarSlot  = ti_Open(USER_INFO_APPVAR_NAME , "r");
	
	// make sure they all exist
	if(!saveStateAppvarSlot)
		formatSaveStateAppvar(SAVE_STATE_APPVAR_NAME);
	if(!settingsAppvarSlot)
		formatSettingsAppvar(SETTINGS_APPVAR_NAME);
	if(!userInfoAppvarSlot)
		formatUserInfoAppvar(USER_INFO_APPVAR_NAME);
	
	ti_Close(saveStateAppvarSlot);
	ti_Close(settingsAppvarSlot);
	ti_Close(userInfoAppvarSlot);

	return true;
}

static bool formatSaveStateAppvar(char *name) {
	
	ti_var_t saveStateAppvarSlot = ti_Open(name, "w");
	ti_Resize(100, saveStateAppvarSlot);
	ti_Close(saveStateAppvarSlot);
	
	return true;
}

static bool formatSettingsAppvar(char *name) {
	ti_var_t settingsAppvarSlot = ti_Open(name, "w");
	ti_Resize(100, settingsAppvarSlot);
	ti_Close(settingsAppvarSlot);
	
	return true;
}

static bool formatUserInfoAppvar(char *name) {
	if(!fileExists(name)) {
		ti_var_t uiSlot = ti_Open(name, "w");
		ti_Resize(100, uiSlot);
		ti_Close(uiSlot);
		
		return true;
	}
	
	return true;
}

static void cleanup() {
	gfx_End();
	archiveAll();
	
	/*
		prevents the on-key error message
	*/
	
	(*(volatile uint8_t*)0xF00008) = 1;
	
	return;
}
