#include "main.h"

// checks for font appvar(s), loads font, and deals with technical font details
static uint8_t setupFontlibc();

// prepares for the program to exit
static void cleanup();
static bool setupAppvars();
static bool formatSaveStateAppvar(char *name);
static bool formatSettingsAppvar(char *name);
static bool formatUserInfoAppvar(char *name);


int main(void) {
	// one-time setup things
	{
	if(!setupFontlibc()) {
		return 0;
	}
	
	// necessary gfx stuff
	gfx_Begin();
	gfx_SetPalette(palette, sizeof_palette, 0);
	gfx_SetTransparentColor(2);
	gfx_SetTextTransparentColor(2);
	gfx_SetTextBGColor(2);
	}
	
	// checks for the data appvars and creates & formats new ones if necessary
	setupAppvars();
	
	// define main structs
	struct fileViewerStruct homeScrn;
	struct editor editor;
	
	// dispHomeScreen needs to return 0 to exit the program, else, it returns 1
	uint8_t mode = HOME;
	
	do {
		if(!mode)
			break;
			
		if(mode == HOME)
      	mode = dispHomeScreen(&homeScrn);
			
		if(mode == OPEN)
			mode = dispEditor(&editor);
			
	} while (true);

	cleanup();
	return 0;
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
			while(!os_GetCSC());
			gfx_End();
      	return 0;
   }
	
   // Use font for whatever
   fontlib_SetFont(my_font, 0);
	fontlib_SetAlternateStopCode(' ');
	fontlib_SetBackgroundColor(2);
	fontlib_SetTransparency(true);
	
	return 1;
	
}

static bool setupAppvars() {
	char *saveStateAppvarName = "SMNTSSV";
	char *settingsAppvarName = "SMNTSTNG";
	char *userInfoAppvarName = "SMNTUI";
	
	uint8_t saveStateAppvarSlot;
	uint8_t settingsAppvarSlot;
	uint8_t userInfoAppvarSlot;
	
	saveStateAppvarSlot = ti_Open(saveStateAppvarName, "r+");
	settingsAppvarSlot = ti_Open(settingsAppvarName, "r+");
	userInfoAppvarSlot = ti_Open(userInfoAppvarName, "r+");
	
	// make sure they all exist
	if(!saveStateAppvarSlot)
		formatSaveStateAppvar(saveStateAppvarName);
	if(!settingsAppvarSlot)
		formatSaveStateAppvar(settingsAppvarName);
	if(!userInfoAppvarSlot)
		formatUserInfoAppvar(userInfoAppvarName);
	
	return true;
}

static bool formatSaveStateAppvar(char *name) {
	uint8_t saveStateAppvarSlot = ti_Open(name, "w+");
	ti_Resize(100, saveStateAppvarSlot);
	return true;
}

static bool formatSettingsAppvar(char *name) {
	uint8_t settingsAppvarSlot = ti_Open(name, "w+");
	ti_Resize(100, settingsAppvarSlot);
	return true;
}

static bool formatUserInfoAppvar(char *name) {
	uint8_t uiSlot = ti_Open(name, "w+");
	ti_Resize(100, uiSlot);
	return true;
}

static void cleanup() {
	gfx_End();
	archiveAll();
	ti_CloseAll();
	
	/*
	This command prevents the on-key error message. got this from commandblockguy, so thank you! P.S he added this feature to the toolchain, so it will be available next release https://github.com/CE-Programming/toolchain/commit/fb852c6360bb30ad79da9e65bdf363da38cfdf83
	*/

	(*(volatile uint8_t*)0xF00008) = 1;
	return;
}