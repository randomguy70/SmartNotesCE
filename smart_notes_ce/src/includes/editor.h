#ifndef EDITOR_H
#define EDITOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <tice.h>
#include "includes/file.h"
#include "includes/buffer.h"

struct editor {
	char fileName[10];
	struct file file;
	struct textBox textBox;
	struct buffer buffer;
	
	bool shouldRefresh;
	
	unsigned int editOffset;
	unsigned int curLine;
	unsigned int curCol;
};

// main text editing loop
enum state dispEditor(struct editor* editor);

// gets the pointers to the start of each line in the file, as well as their byte offset from the text origin
int getLinePtrs(struct buffer *buffer);

#ifdef __cplusplus
}
#endif

#endif