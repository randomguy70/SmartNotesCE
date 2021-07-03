#include "main.h"

/* main loop */
int main() {
   gfx_Begin();
   gfx_SetPalette(palette, sizeof_palette, 0);
   gfx_SetTransparentColor(2);
   gfx_SetTextTransparentColor(2);
   gfx_SetTextBGColor(2);
   kb_SetMode(MODE_3_CONTINUOUS);
	kb_DisableOnLatch();

	//set mode at 0 to exit
	uint8_t mode = 1;
   while(mode) {
      mode = dispHomeScreen();
   }
	gfx_End();
	(*(volatile uint8_t*)0xF00008) = 1; // this prevents the on-key error message. got this from commandblockguy, so thank you!
   return 0;
}
