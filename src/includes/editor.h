#ifndef EDITOR_H
#define EDITOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <tice.h>
#include "includes/file.h"
#include "includes/buffer.h"
#include "includes/text.h"

#define EDITOR_MAX_LINES_VIEWABLE 11

#define EDITOR_HEADER_BAR_X       0
#define EDITOR_HEADER_BAR_Y       0
#define EDITOR_HEADER_BAR_HEIGHT  25
#define EDITOR_HEADER_BAR_WIDTH   LCD_WIDTH

#define EDITOR_FOOTER_BAR_X       0
#define EDITOR_FOOTER_BAR_Y       LCD_HEIGHT - EDITOR_HEADER_BAR_HEIGHT - 1
#define EDITOR_FOOTER_BAR_HEIGHT  25
#define EDITOR_FOOTER_BAR_WIDTH   LCD_WIDTH

#define EDITOR_TEXT_BOX_X         0
#define EDITOR_TEXT_BOX_Y         EDITOR_HEADER_BAR_HEIGHT
#define EDITOR_TEXT_BOX_WIDTH     LCD_WIDTH
#define EDITOR_TEXT_BOX_HEIGHT    LCD_HEIGHT - (EDITOR_HEADER_BAR_HEIGHT + EDITOR_FOOTER_BAR_HEIGHT)

struct textBox;

struct editor {
	struct file file;
	struct buffer buffer;
	struct textBox textBox;
};

// main text editing loop
enum state dispEditor(struct editor* editor);

static int initialiseEditor(struct editor *editor);

// get the pointers to the lines on screen
static int getTextBoxLinePointers(struct textBox *textBox);

// gets the pointers to the start of each line in the file, as well as their byte offset from the text origin
int getLinePtrs(struct buffer *buffer);

#ifdef __cplusplus
}
#endif

#endif