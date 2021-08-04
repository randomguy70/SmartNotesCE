#ifndef EDITOR_H
#define EDITOR_H

#include <includes/file.h>

//// declarations
struct editor;

//// definitions
struct editor {
	char *fileName;    // hmmm??? maybe the name of the open file?!!!
	struct file file;  // contains data about the opened file in the editor
	
	bool isRunning;      // Whether or not to repeat the editor loop. If 0, then the editor will quit at the first opportunity
	bool shouldRefresh;      // whether or not to go through the editor loop again (calculate line ptrs, display text again)
	
	int editOffset;    // cursor offset in the text array that is being edited
	int selectedChars; // Number of characters highlighted / selected (to perform actions upon these characters, they can be found directly after the cursor offset in the text array). Default is 0
	int curLine;       // line that cursor is on
	int curCol;        // column that cursor is after
};

// main text editing loop
uint8_t dispEditor(struct editor *editor);

// gets the pointers to the start of each line in the file, as well as their byte offset from the text origin
int getLinePtrs(char *array);

#endif