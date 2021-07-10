#ifndef MAIN_H
#define MAIN_H

// standard includes. i don't want to have to type them out at the beginning of every last .c file, so i just put include main.h instead. I don't care what MateoC says if he comes with some obscure rule about includes that says i am in error :P....
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>

#include <tice.h>
#include <string.h>

#include <keypadc.h>
#include <compression.h>
#include <graphx.h>
#include <fileioc.h>
#include <fontlibc.h>

// my includes
#include "gfx/gfx.h"
#include "includes/key.h"
#include "includes/ui.h"
#include "includes/text.h"
#include "includes/file.h"

// defines
#define FALSE 0
#define TRUE !(FALSE)

#define BLACK         0  // defines the fixed-entry offset of black in my custom palette
#define WHITE         1  // defines the fixed-entry offset of white in my custom palette
#define TRANSPARENT   2  // defines the fixed-entry offset of transparent color in my custom palette
#define DARK_GREY     3  // defines the fixed-entry offset of dark grey in my custom palette
#define MEDIUM_GREY   4  // defines the fixed-entry offset of medium grey in my custom palette
#define LIGHT_GREY    5  // defines the fixed-entry offset of light grey in my custom palette
#define DARK_BLUE     6  // defines the fixed-entry offset of electric blue in my palette. I was too lazy to change the name.
#define RED           7  // defines the fixed-entry offset of red in my custom palette
#define LIGHTER_BLUE  8  // defines the fixed-entry offset of blue in my custom palette

#define MATH          1
#define CAPS          2
#define LOWER_CASE    3

#define TEXT_ORIGIN   10 // the text in a file begins at offset of 10 bytes from the beginning

#endif