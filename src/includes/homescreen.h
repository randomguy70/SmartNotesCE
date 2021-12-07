#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <includes/file.h>

#define MAX_FILES_LOADABLE  30
#define MAX_FILES_VIEWABLE  9
#define FILE_SPACING			 15

#define FILE_VIEWER_BODY_HEIGHT       MAX_FILES_VIEWABLE * FILE_SPACING
#define FILE_VIEWER_BORDER_RADIUS     9
#define FILE_VIEWER_BORDER_THICKNESS  1
#define FILE_VIEWER_HEADER_HEIGHT     20
#define FILE_VIEWER_FOOTER_HEIGHT     FILE_VIEWER_BORDER_RADIUS
#define FILE_VIEWER_WIDTH             250
#define FILE_VIEWER_HEIGHT            (FILE_SPACING * MAX_FILES_VIEWABLE) + FILE_VIEWER_HEADER_HEIGHT + FILE_VIEWER_FOOTER_HEIGHT
#define FILE_VIEWER_X                 (LCD_WIDTH / 2) - (FILE_VIEWER_WIDTH / 2)
#define FILE_VIEWER_Y                 LCD_HEIGHT / 2 - 82 // I had to hard code FILE_VIEWER_HEIGHT == 82 because the compiler apparently couldn't do it

#define FILE_VIEWER_DIVIDOR_WIDTH     4
#define FILE_VIEWER_DIVIDOR_X         FILE_VIEWER_X + (FILE_VIEWER_WIDTH / 2) - (FILE_VIEWER_DIVIDOR_WIDTH / 2)
#define FILE_VIEWER_DIVIDOR_Y         FILE_VIEWER_Y + FILE_VIEWER_HEADER_HEIGHT
#define FILE_VIEWER_DIVIDOR_HEIGHT    FILE_VIEWER_HEIGHT - FILE_VIEWER_HEADER_HEIGHT

#define STARTING_FILE_X               FILE_VIEWER_X + FILE_VIEWER_BORDER_THICKNESS
#define STARTING_FILE_Y               FILE_VIEWER_Y + FILE_VIEWER_HEADER_HEIGHT
#define STARTING_FILE_WIDTH           (FILE_VIEWER_WIDTH / 2) - FILE_VIEWER_BORDER_THICKNESS - (FILE_VIEWER_DIVIDOR_WIDTH / 2)

#define STARTING_RECENTS_X            FILE_VIEWER_DIVIDOR_X + FILE_VIEWER_DIVIDOR_WIDTH
#define STARTING_RECENTS_Y            FILE_VIEWER_Y + FILE_VIEWER_HEADER_HEIGHT
#define STARTING_RECENTS_WIDTH        (FILE_VIEWER_WIDTH / 2) - FILE_VIEWER_BORDER_THICKNESS - (FILE_VIEWER_DIVIDOR_WIDTH / 2)

#define NUM_HOMESCREEN_BUTTONS        5

// contains properties of the file viewer
struct homescreen {
	struct file files[MAX_FILES_LOADABLE];
   uint8_t numFiles;
	uint8_t numFilesDisplayed;
   uint8_t offset;             // the number of files that were scrolled up off the screen
   uint8_t selectedFile;
   bool wasScrolled;
};

// displays homescreen and deals with all the homescreen functions and options
enum state dispHomeScreen(struct homescreen* homescreen);

#ifdef __cplusplus
}
#endif

#endif