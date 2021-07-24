#ifndef EDITOR_H
#define EDITOR_H

#include "main.h"

struct editor {
	bool running; // Whether or not to repeat the editor loop. If 0, then the editor will quit at the first opportunity
	bool refresh; // whether or not to go through the editor loop again
	struct file; // contains data about the opened file in the editor
};

// main text editing loop
uint8_t dispEditor(struct editor *editor);

#endif