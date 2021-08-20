#include "main.c"
#include "main.h"

#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>
#include <string.h>

#include <includes/homescreen.h>
#include <includes/text.h>
#include <includes/file.h>
#include <includes/ui.h>

//// declarations
static void dispHomeScreenBG(void);
static uint8_t dispFiles(void);
static void dispHSButtons(void);
static uint8_t handleHomeScrnKeyPresses(void);
static uint8_t loadFiles(void);
static struct menu *loadHomeScreenOtherMenu(void);

// btw, HS stands for homescreen
void dispHomeScreen(void) {
	
   homescreen.selectedFile = 0;
   homescreen.offset = 0;
	homescreen.shouldQuit = false;
	
	loadFiles();
	archiveAll();

   while(true) {
      dispHomeScreenBG();
      dispHSButtons();
		
      dispFiles();
		
		gfx_Wait();
		gfx_SwapDraw();
		
      handleHomeScrnKeyPresses();
		
		if(state == should_exit)
			return;
		if(state == show_homescreen)
			return;
   }
}

// display the file names & info stored in the fileViewerStruct *HS
static uint8_t dispFiles(void) {
   uint8_t i;          // starting increment of file display, ends up as the number of files displayed onscreen (should be <=10)
   uint8_t ii = 0;     // how many files have been displayed so far
   uint8_t fileSlot;   // slot number of currently detected file
   uint8_t fileSize;   // size of currently detected and drawn file
   int fileY = 61;

   for(i=homescreen.offset, homescreen.numFilesDisplayed = 0; i < 10+homescreen.offset && i  <homescreen.numFiles; i++, homescreen.numFilesDisplayed++) {
      fileSlot = ti_Open(homescreen.fileNames[i],"r+");
      fileSize = ti_GetSize(fileSlot);
		ti_Close(fileSlot);

      // display currently selected file with a scrollbar on top of it
      if (homescreen.selectedFile == i) {
			// draw scrollbar & leave some pixels at the edge of the window for the scrollbar
         gfx_SetColor(LIGHT_GREY);
         gfx_FillRectangle_NoClip(36,fileY-5,242,15);
			gfx_SetColor(BLACK);
			gfx_Rectangle_NoClip(36,fileY-5,242,15);

      }
		
      gfx_SetTextFGColor(BLACK);
		
      gfx_PrintStringXY(homescreen.fileNames[i],40,fileY);
      gfx_SetTextXY(135,fileY);
      gfx_PrintInt(fileSize,4);
      gfx_SetTextFGColor(0);
      fileY+=15;
      ii++;
   }
   // display when no files were detected because you forgot to take notes :P
   if (homescreen.numFiles == 0) {
      gfx_SetTextFGColor(244);
      gfx_PrintStringXY("--NO FILES FOUND--)",93,80);
      gfx_PrintStringXY("That's too bad for you :(",93,100);
   }

   // Return the number of files displayed, I guess. This really isn't necessary at all, but maybe somebody somewhere cares somewhat for some reason...
   return i;
}

// homescreen for the fileViewer, rectangles, title, etc...
static void dispHomeScreenBG(void) {
	int width;
	
	// scrollbar math
	uint8_t scrollbarHeight = 148 * homescreen.numFilesDisplayed / homescreen.numFiles;
	
	// just making sure that the scrollbar is a reasonable size...
	if(scrollbarHeight>148)
		scrollbarHeight = 148;
	if(scrollbarHeight<10)
		scrollbarHeight = 10;
	
	int scrollbarX = 280;
	int scrollbarY = (150 - (scrollbarHeight) + 1) * homescreen.selectedFile / (homescreen.numFiles-1) + 56;
	
   gfx_SetDraw(1);
	
	// lined-paper background
   gfx_FillScreen(PAPER_YELLOW);
	gfx_SetColor(LIGHT_BLUE);
	gfx_SetTextXY(1, 1);
	for(uint8_t i = 0; i<11; i++) {
		gfx_HorizLine_NoClip(0, i*20, SCRN_WIDTH);
		gfx_HorizLine_NoClip(0, i*20+1, SCRN_WIDTH);
	}

	// name and credits (to me :P)
	gfx_SetTextFGColor(DARK_BLUE);
	width = gfx_GetStringWidth("SmartNotes CE");
	gfx_PrintStringXY("SMARTNOTES CE", (SCRN_WIDTH/2)-(width/2), 8);
	
	gfx_SetTextFGColor(BLACK);
	width = gfx_GetStringWidth("VERSION 1.0 BY Randomguy");
   gfx_PrintStringXY("Version 1.0 BY Randomguy", (SCRN_WIDTH/2)-(width/2), 27);
	
	// box with file names
   gfx_SetColor(WHITE);
   gfx_FillRectangle_NoClip(36,56,248,150);
	gfx_SetColor(LIGHT_BLUE);
	thick_Rectangle(34, 54, 252, 154, 2);
	
	// scrollbar border
	gfx_SetColor(LIGHT_BLUE);
	gfx_VertLine_NoClip(scrollbarX-2, 56, 150);
	gfx_VertLine_NoClip(scrollbarX-1, 56, 150);
	
	// scrollbar
	gfx_SetColor(LIGHT_GREY);
	gfx_FillRectangle_NoClip(scrollbarX, scrollbarY, 4, scrollbarHeight);
	gfx_SetColor(BLACK);
	gfx_Rectangle_NoClip(scrollbarX, scrollbarY, 4, scrollbarHeight);
	
	// print labels for displayed file data columns
   gfx_SetTextFGColor(BLACK);
   gfx_PrintStringXY("NAME",40,45);
   gfx_PrintStringXY("SIZE",135,45);
   gfx_PrintStringXY("STATUS",210,45);
	
	return;
}

static void dispHSButtons(void) {
	
	gfx_sprite_t * sprites[5];
	sprites[0] = open;
	sprites[1] = new_icon;
	sprites[2] = quit;
	sprites[3] = trash;
	sprites[4] = more;
	
   for(int i = 1, ii = 0; i < 320; i+=64, ii++) {
		// button rects
      gfx_SetColor(LIGHT_GREY);
      gfx_FillRectangle_NoClip(i+1, 215, 60, 24);
		gfx_SetColor(BLACK);
      gfx_Rectangle_NoClip(i+1, 215, 60, 24);
		
		// sprites
		if(i == 1)
			gfx_TransparentSprite_NoClip(sprites[ii], i+4, 218);
		else
			gfx_TransparentSprite_NoClip(sprites[ii], i+3, 217);
	}
	
	// text
	gfx_SetTextFGColor(0);
	
   gfx_PrintStringXY("Open" ,  27, 224);
   gfx_PrintStringXY("New"  ,  90, 224);
   gfx_PrintStringXY("Quit" , 157, 224);
   gfx_PrintStringXY("Del"  , 220, 224);
   gfx_PrintStringXY("Other", 271, 224);
	
}

static uint8_t handleHomeScrnKeyPresses(void) {
   kb_Scan();

   // move cursor down
   if(kb_IsDown(kb_KeyDown) && homescreen.selectedFile < homescreen.numFiles-1) {
      homescreen.selectedFile++;
      if(homescreen.selectedFile >= homescreen.offset+10){
         homescreen.offset++;
      }
   }

	// move cursor up
   if (kb_IsDown(kb_KeyUp) && homescreen.selectedFile>0) {
      homescreen.selectedFile--;
      if(homescreen.selectedFile < homescreen.offset){
         homescreen.offset--;
      }
   }

	// open file
	if(kb_IsDown(kb_KeyYequ)) {
		// if there aren't any files to open...
		if(homescreen.numFiles <= 0) {
			alert("There aren't any files to open (obviously).");
			return CANCEL;
		}
		// otherwise...open the selected file.
		state = show_editor;
		return OPEN;
	}
	
   // new file
   if (kb_IsDown(kb_KeyWindow)) {
		if(homescreen.numFiles >= 30 ) {
			alert("You can't have more than 30 files, my note-crazy friend!");
			return CANCEL;
		}
		
		newFile();
		loadFiles();
		return true;
   }
	
	// quit program
	if (kb_IsDown(kb_KeyClear) || kb_IsDown(kb_KeyZoom)) {
		state = should_exit;
		return QUIT;
	}
	
	// delete file
   if ((kb_IsDown(kb_KeyTrace) || kb_IsDown(kb_KeyDel))) {
		// make sure there is a file to delete
		if(homescreen.numFiles == 0) {
			alert("There aren't any files to delete!");
			return CANCEL;
		}
		// if there is at least 1 file...
		checkIfDeleteFile(homescreen.fileNames[homescreen.selectedFile]);
		loadFiles();
		// check if you need to shift the cursor
		if(homescreen.numFiles > 0 && homescreen.selectedFile >= homescreen.numFiles)
			homescreen.selectedFile--;
			
		return CANCEL;
   }
	
	// other (opens fun menu with sprites)
	if(kb_IsDown(kb_KeyGraph)) {
		
		struct menu *menu = loadHomeScreenOtherMenu();
		
		uint8_t result = displayMenu(menu);
			
		/* --Options--
			back, rename, hide, settings, help (start at 1 as an offset because 0 signifies quit)
		*/
		switch(result) {
			
			// quit
			case QUIT:
				return QUIT;
				
			// back
			case 1:
				return CANCEL;
			
			// rename
			case 2:
				if(homescreen.numFiles>0) {
					if(renameFile(homescreen.fileNames[homescreen.selectedFile])) {
						loadFiles();
						return 1;
					}
				}
					
				// if it didn't return, then there aren't any files to rename...
				alert("There aren't any files to rename!");
				return 1;
				
			// hide
			case 3: 
				// hideFile(char* name); // haven't defined this yet btw...
				return 1;
				
			// settings
			case 4:
				// displaySettings(); // i haven't defined this either...
				return 1;
				
			// if the user simply wants to close the menu
			default:
				return CANCEL;
			
		}
		
	}

	// if the user doesn't want to quit, then return 1
	return 1;
}

//loads the data into the struct for a homescreen menu

static uint8_t loadFiles(void) {
   uint8_t numFiles = 0;
   ti_var_t fileSlot = 0; // slot of currently detected file
   char *namePtr = NULL;
   void *search_pos = NULL; // mem location of the currently detected file in the VAT
		
   while ((namePtr = ti_Detect(&search_pos, HEADER_STR)) != NULL) {
		
		// get some info from the currently detected file
      fileSlot = ti_Open(namePtr, "r+");
		if(!fileSlot)
			return 0;
		
		ti_GetName(homescreen.fileNames[numFiles], fileSlot);
		
      homescreen.fileSizes[numFiles] = ti_GetSize(fileSlot);
		
		// files have to be at least 50 bytes large for (future) formatting purposes
		if(homescreen.fileSizes[numFiles] < MIN_FILE_SIZE) {
			ti_Seek(3, 0, fileSlot);
			
			ti_Write((const void *)0xE40000, MIN_FILE_SIZE-3, 1, fileSlot);
			homescreen.fileSizes[numFiles] = MIN_FILE_SIZE;
		}
		
		// "always close files after opening them" -Jacobly, ergo...
		ti_SetArchiveStatus(true, fileSlot);
		ti_Close(fileSlot);
      numFiles++;
   }
	
   homescreen.numFiles = numFiles;
   return numFiles;
}

static struct menu *loadHomeScreenOtherMenu(void) {
	static struct menu menu = { 
		.title = "Options",
		.x = 200, .y = 100,
		.numOptions = 5,
		.hasSprites = true,
		
		// array of menu entries
		.entry = {
			{"Back", left_arrow, left_arrow_height},
			{"Rename", rename, rename_height},
			{"(un)Hide", hide, hide_height},
			{"Settings", settings, settings_height},
			{"Help", help, help_height},
		},
		
	};
	
	return &menu;
}
