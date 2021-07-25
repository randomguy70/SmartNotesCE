#ifndef EDITOR_H
#define EDITOR_H

#include "main.h"

//// declarations
struct editor;

//// definitions
struct editor {
	char fileName[10]; // name of the open file!!!
	bool running; // Whether or not to repeat the editor loop. If 0, then the editor will quit at the first opportunity
	bool refresh; // whether or not to go through the editor loop again
	struct file file; // contains data about the opened file in the editor
};

// main text editing loop
uint8_t dispEditor(struct editor *editor);

#endif