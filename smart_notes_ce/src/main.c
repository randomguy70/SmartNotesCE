#include "main.h"

// prepares for the program to exit
void cleanup();

int main() {
	// one-time setup things
	{
	
	// necessary gfx stuff
	gfx_Begin();
	gfx_SetPalette(palette, sizeof_palette, 0);
	gfx_SetTransparentColor(2);
	gfx_SetTextTransparentColor(2);
	gfx_SetTextBGColor(2);
		
	// fontlib stuff (advanced text features)
	fontlib_SetAlternateStopCode(' ');
	fontlib_SetBackgroundColor(2);
	fontlib_SetTransparency(TRUE);
	}

	// dispHomeScreen needs to return 0 to exit the program
	uint8_t mode;
	do {
      mode = dispHomeScreen();
	} while (mode);

	cleanup();
	return 0;
}

void cleanup() {
	gfx_End();
	archiveAll();
	ti_CloseAll();
	
	/*
	This command prevents the on-key error message. got this from commandblockguy, so thank you! P.S he added this feature to the toolchain, so it will be available next release https://github.com/CE-Programming/toolchain/commit/fb852c6360bb30ad79da9e65bdf363da38cfdf83
	*/
	(*(volatile uint8_t*)0xF00008) = 1;
	return;
}
