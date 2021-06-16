#include "main.h"

/* main loop */
void main() {
   uint8_t mode = 1;
   gfx_Begin();
   gfx_SetPalette(palette, 18, 0);
   gfx_SetTransparentColor(2);
   gfx_SetTextTransparentColor(2);
   gfx_SetTextBGColor(2);
   kb_SetMode(MODE_3_CONTINUOUS);
   //put mode at 0 to exit, or call function exitFull()
   while(mode) {
      if (mode == 1) {
         mode = dispHomeScreen();
      }
   }
}