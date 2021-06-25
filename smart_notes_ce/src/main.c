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
	(*(volatile uint8_t*)0xF00008) = 1; // This is only necessary right before the main() returns 0, I just added this because the program compiled several bytes smaller when I added this here, probably due to the way the compression works. Also, why not?
 
   //set mode at 0 to exit
   while(mode) {
      mode = dispHomeScreen();
   }
	kb_ClearOnLatch(); // this is to prevent the error message (hopefully)		
	(*(volatile uint8_t*)0xF00008) = 1; // this prevents the on-key error message. got this from commandblockguy!
   return 0;
}
