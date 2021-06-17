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
#define BLACK        0  // defines the fixed-entry offset of this color in my custom palette
#define WHITE        1  // defines the fixed-entry offset of this color in my custom palette
#define TRANSPARENT  2  // defines the fixed-entry offset of this color in my custom palette
#define DARK_GREY    3  // defines the fixed-entry offset of this color in my custom palette
#define MEDIUM_GREY  4  // defines the fixed-entry offset of this color in my custom palette
#define LIGHT_GREY   5  // defines the fixed-entry offset of this color in my custom palette
#define DARK_BLUE    6  // defines the fixed-entry offset of this color in my custom palette
#define RED          7  // defines the fixed-entry offset of this color in my custom palette

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