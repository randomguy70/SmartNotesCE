#include "main.h"

/* main loop */
int main() {
   uint8_t mode = 1;
   gfx_Begin();
   gfx_SetPalette(palette, 18, 0);
   gfx_SetTransparentColor(2);
   gfx_SetTextTransparentColor(2);
   gfx_SetTextBGColor(2);
   kb_SetMode(MODE_3_CONTINUOUS);
	kb_DisableOnLatch();
 
   //set mode at 0 to exit
	// I will define the 3 main structs here eventually to save the state of each when you switch modes (ex: if you are in the file displayer and you switch to settings, and then you switch back to the file displayer, your selection bar will be at the same position as it was before you switched)
   while(mode) {
      mode = dispHomeScreen();
   }
	(*(volatile uint8_t*)0xF00008) = 1; // this prevents the on-key error message. got this from commandblockguy, so thank you!
   return 0;
}
