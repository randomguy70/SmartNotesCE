#include "main.h"

// checks for font appvar(s), loads font, and deals with technical font details
static uint8_t setupFontlibc();

// prepares for the program to exit
static void cleanup();

void main(void) {
	// one-time setup things
	{
	
	// this has to go before the gfx setup, in case it has to quit
	if(!setupFontlibc()) {
		return;
	}
	
	// necessary gfx stuff
	gfx_Begin();
	gfx_SetPalette(palette, sizeof_palette, 0);
	gfx_SetTransparentColor(2);
	gfx_SetTextTransparentColor(2);
	gfx_SetTextBGColor(2);
	
	}

	// dispHomeScreen needs to return 0 to exit the program, else, it returns 1
	uint8_t mode;
	do {
      mode = dispHomeScreen();
	} while (mode);

	cleanup();
	return;
}

static uint8_t setupFontlibc() {
	fontlib_font_t *my_font;
	
	// Get the first font present in the font pack
   my_font = fontlib_GetFontByIndex("DRSANS", 0);
	if (!my_font) {
        gfx_PrintStringXY("DRSANS appvar not found or invalid", 0, 0);
		  while(!os_GetCSC()){};
        return 0;
   }
	
   // Use font for whatever
   fontlib_SetFont(my_font, 0);
	fontlib_SetAlternateStopCode(' ');
	fontlib_SetBackgroundColor(2);
	fontlib_SetTransparency(true);
	
	return 1;
	
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

