#ifndef MAIN_H
#define MAIN_H

// standard includes. i don't want to have to type them out at the beginning of every last .c file, so i just put include main.h instead. I don't care what MateoC says if he comes with some obscure rule about includes that says i am in error :P....
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
#include <string.h>

// my includes
#include "gfx/gfx.h"
#include "includes/data.h"
#include "includes/key.h"
#include "includes/ui.h"
#include "includes/text.h"
#include "includes/file.h"

// defines
#define BLACK         0  // defines the fixed-entry offset of black in my custom palette
#define WHITE         1  // defines the fixed-entry offset of white in my custom palette
#define TRANSPARENT   2  // defines the fixed-entry offset of transparent color in my custom palette
#define DARK_GREY     3  // defines the fixed-entry offset of dark grey in my custom palette
#define MEDIUM_GREY   4  // defines the fixed-entry offset of medium grey in my custom palette
#define LIGHT_GREY    5  // defines the fixed-entry offset of light grey in my custom palette
#define DARK_BLUE     6  // defines the fixed-entry offset of dark blue in my custom palette
#define RED           7  // defines the fixed-entry offset of red in my custom palette
#define LIGHTER_BLUE  8  // defines the fixed-entry offset of blue in my custom palette

/* necessary but random info
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