#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <includes/file.h>

// contains properties of the file viewer
struct homescreen {
	struct file files[30];
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