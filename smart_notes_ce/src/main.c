#include "main.h"

/* main loop */
int main() {
   uint8_t mode = 1;
   gfx_Begin();
   gfx_SetPalette(palette, sizeof_palette, 0);
   gfx_SetTransparentColor(2);
   gfx_SetTextTransparentColor(2);
   gfx_SetTextBGColor(2);
   kb_SetMode(MODE_3_CONTINUOUS);
	kb_DisableOnLatch();
 
	struct fileViewerStruct HS; // homescreen
	struct editorStruct ES; // editor struct 
	struct settingsStruct settings; // settings struct

	//set mode at 0 to exit
   while(mode) {
      mode = dispHomeScreen(&HS, &ES, &settings);
   }
	(*(volatile uint8_t*)0xF00008) = 1; // this prevents the on-key error message. got this from commandblockguy, so thank you!
   return 0;
}
