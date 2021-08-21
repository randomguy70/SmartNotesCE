#ifndef EDITOR_H
#define EDITOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <includes/file.h>

//// declarations
struct editor;

//// definitions
struct editor {
	char fileName[10];    // hmmm??? maybe the name of the open file?!!!
	struct file file;     // contains data about the opened file in the editor
	struct buffer buffer;
	
	bool shouldRefresh;   // whether or not to calculate the line pointers and re-display the text
	
	unsigned int editOffset;    // cursor offset in the text array that is being edited
	unsigned int curLine;       // line that cursor is on
	unsigned int curCol;        // column that cursor is after
};

// main text editing loop
enum state dispEditor(void);

// gets the pointers to the start of each line in the file, as well as their byte offset from the text origin
int getLinePtrs(struct buffer* buffer);

#ifdef __cplusplus
}
#endif

#endif