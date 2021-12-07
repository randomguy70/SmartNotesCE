#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <includes/file.h>

#define MAX_FILES_LOADABLE  30
#define MAX_FILES_VIEWABLE  10
#define FILE_SPACING			 15

#define FILE_VIEWER_BORDER_RADIUS   9
#define FILE_VIEWER_HEADER_HEIGHT   20
#define FILE_VIEWER_WIDTH           260
#define FILE_VIEWER_HEIGHT          (FILE_SPACING * MAX_FILES_VIEWABLE) + FILE_VIEWER_HEADER_HEIGHT
#define FILE_VIEWER_X               (LCD_WIDTH / 2) - (FILE_VIEWER_WIDTH / 2)
#define FILE_VIEWER_Y               30

#define NUM_HOMESCREEN_BUTTONS 5

// contains properties of the file viewer
struct homescreen {
	struct file files[MAX_FILES_LOADABLE];
   uint8_t numFiles;
	uint8_t numFilesDisplayed;
   uint8_t offset;             // the number of files that were scrolled up off the screen
   uint8_t selectedFile;
};

// displays homescreen and deals with all the homescreen functions and options
enum state dispHomeScreen(struct homescreen* homescreen);

#ifdef __cplusplus
}
#endif

#endif