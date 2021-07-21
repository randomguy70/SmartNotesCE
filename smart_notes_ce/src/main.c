#include "main.h"

// checks for font appvar(s), loads font, and deals with technical font details
static uint8_t setupFontlibc();

// prepares for the program to exit
static void cleanup();

int main(void) {
	// one-time setup things
	{
	
	// this has to go before the gfx setup, in case it has to quit
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
	char hello[300] = {"Hello this is me randomguy testing how well my word wrapping window works if it works well I will implement it right away This is really great and I didn't expect it to go so well so soon I am amazed at how powerful a mere calculator can be Thank you DrDnar"};

	alert(hello);
	
	// dispHomeScreen needs to return 0 to exit the program, else, it returns 1
	uint8_t mode;
	do {
      mode = dispHomeScreen();
	} while (mode);

	cleanup();
	return 0;
}

static uint8_t setupFontlibc() {
	fontlib_font_t *my_font;
	
	// Get the first font present in the font pack
   my_font = fontlib_GetFontByIndex("DRSANS", 0);
	if (!my_font) {
			gfx_Begin();
			gfx_SetTextXY(0, 1);
      	gfx_PrintString("DRSANS appvar not found or invalid :(");
			gfx_SetTextXY(-1, 15);
			gfx_PrintString("It was in the same folder my program came in but");
			gfx_SetTextXY(0, 30);
			gfx_PrintString("if you can't find it, ask for help on Cemetech,");
			gfx_SetTextXY(0, 45);
			gfx_PrintString("or email me at onlychessfreeze0515@gmail.com.");
			while(!os_GetCSC()){};
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
