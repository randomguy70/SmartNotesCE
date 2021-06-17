#ifndef MAIN_H
#define MAIN_H

// standard includes
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>
#include <math.h>
#include <keypadc.h>
#include <compression.h>
#include <graphx.h>
#include <fileioc.h>
#include <fontlibc.h>

// my includes
#include "gfx/gfx.h"
#include "includes/data.h"
#include "includes/key.h"
#include "includes/ui.h"
#include "includes/text.h"
#include "includes/file.h"

// defines
#define BLACK        0  // equals the fixed-entry offset in my palette for this color
#define WHITE        1  // equals the fixed-entry offset in my palette for this color
#define TRANSPARENT  2  // equals the fixed-entry offset in my palette for this color
#define DARK_GREY    3  // equals the fixed-entry offset in my palette for this color
#define MEDIUM_GREY  4  // equals the fixed-entry offset in my palette for this color
#define LIGHT_GREY   5  // equals the fixed-entry offset in my palette for this color
#define DARK_BLUE    6  // equals the fixed-entry offset in my palette for this color
#define RED          7  // equals the fixed-entry offset in my palette for this color

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

#endif