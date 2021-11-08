#ifndef EDITOR_H
#define EDITOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <tice.h>
#include "includes/file.h"
#include "includes/buffer.h"
#include "includes/text.h"

struct editor {
	struct file file;
	struct buffer buffer;
	
	struct textBox textBox;
	
	unsigned int editOffset;
	unsigned int curLine;
	unsigned int curCol;
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