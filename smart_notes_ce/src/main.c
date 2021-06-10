#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>
#include <math.h>
#include <keypadc.h>
#include <compression.h>
#include <graphx.h>
#include <fileioc.h>

// my headers
#include "gfx/gfx.h"
#include "includes/data.h"
#include "includes/key.h"
#include "includes/ui.h"
#include "includes/text.h"
#include "includes/file.h"

// defines
#define BLACK        0
#define WHITE        1
#define TRANSPARENT  2
#define DARK_GREY    3
#define MEDIUM_GREY  4
#define LIGHT_GREY   5
#define DARK_BLUE    6

/*necessary information
-3 modes
   mode 1 = homescreen
   mode 2 = editor
   mode 3 = settings
-3 txt modes
   txtMode 1 = math
   txtMode 2 = caps
   txtMode 3 = lowercase

*/

/* functon prototypes */

/* main loop */
int main() {
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
   return 0;
}
