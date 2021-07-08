#include "main.h"

// prepares for the program to exit
void cleanup();

int main() {
	// one-time setup things
	{
	gfx_Begin();
	gfx_SetPalette(palette, sizeof_palette, 0);
	gfx_SetTransparentColor(2);
	gfx_SetTextTransparentColor(2);
	gfx_SetTextBGColor(2);
	//kb_SetMode(MODE_3_CONTINUOUS);
	kb_DisableOnLatch();
	}

	// dispHomeScreen needs to return 0 to exit the program
	uint8_t mode = 1;
	do {
      mode = dispHomeScreen();
	} while (mode);

	gfx_End();
	archiveAll();
	ti_CloseAll();
	
	/*
	This command prevents the on-key error message. got this from commandblockguy, so thank you! ps, he added this feature to the toolchain, so it will be available next release https://github.com/CE-Programming/toolchain/commit/fb852c6360bb30ad79da9e65bdf363da38cfdf83
	*/
	(*(volatile uint8_t*)0xF00008) = 1;
	return 0;
}

void cleanup() {
	gfx_End();
	archiveAll();
	ti_CloseAll();
	
	/*
	This command prevents the on-key error message. got this from commandblockguy, so thank you! ps, he added this feature to the toolchain, so it will be available next release https://github.com/CE-Programming/toolchain/commit/fb852c6360bb30ad79da9e65bdf363da38cfdf83
	*/
	(*(volatile uint8_t*)0xF00008) = 1;
	return;
}
