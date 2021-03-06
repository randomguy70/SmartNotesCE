#ifndef MAIN_H
#define MAIN_H

// defines
#define VERSION 1
#define RELEASE_STRING "V.1.0"
#define AUTHOR "Randomguy"

#define HOME            1  // Integer return signifying a request to go to the homescreen (file viwer)
#define QUIT            0  // Integer return signifying a quit request.
#define CANCEL        100  // Integer return signifying a cancel / close window request. NOT the same as QUIT.
#define OPEN          101  // Integer return signigying a file open request. Should lead to a given file being opened in the editor.
#define CLOSE         102  // Integer return signigying request to close the file currently opened in the editor. Should reset the editor variables and either quit the program or open the file menu.
#define BACK          103  // Integer return signifying a request to go back 1 scope level / return level

enum state {
	show_homescreen,
	show_editor, 
	show_settings,
	should_exit,
};

#endif
