#include "main.h"

#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>
#include <string.h>

#include <includes/homescreen.h>
#include <gfx/gfx.h>
#include <includes/file.h>
#include <includes/ui.h>

//// declarations
static void dispHomeScreenBG(struct fileViewerStruct * HS);
static uint8_t dispFiles(struct fileViewerStruct *HS);
static void dispHSButtons(void);
static uint8_t handleHomeScrnKeyPresses(struct fileViewerStruct *HS);
static uint8_t loadFiles(struct fileViewerStruct *HS);
static struct menu *loadHomeScreenOtherMenu(void);


// btw, HS stands for homescreen
uint8_t dispHomeScreen(struct fileViewerStruct *HS) {
	// initialise / reset variables
   HS->selectedFile = 0;
   HS->offset = 0;
   HS->numFiles = loadFiles(HS);
	HS->shouldQuit = false;
	archiveAll();
	
	uint8_t result = 1;

   while(true) {
      dispHomeScreenBG(HS);
      dispHSButtons();
		
      dispFiles(HS);
		
		gfx_Wait();
		gfx_SwapDraw();
		
      result = handleHomeScrnKeyPresses(HS);
		
		if(result == QUIT)
			return QUIT;
		if(result == OPEN)
			return OPEN;
			
   }
}

// display the file names & info stored in the fileViewerStruct *HS
static uint8_t dispFiles(struct fileViewerStruct *HS) {
   uint8_t i;          // starting increment of file display, ends up as the number of files displayed onscreen (should be <=10)
   uint8_t ii = 0;     // how many files have been displayed so far
   uint8_t fileSlot;   // slot number of currently detected file
   uint8_t fileSize;   // size of currently detected and drawn file
   int fileY = 61;

   for(i=HS->offset, HS->numFilesDisplayed = 0; i < 10+HS->offset && i  <HS->numFiles; i++, HS->numFilesDisplayed++) {
      fileSlot = ti_Open(HS->fileNames[i],"r+");
      fileSize = ti_GetSize(fileSlot);

      // display currently selected file with a scrollbar on top of it
      if (HS->selectedFile == i) {
			// draw scrollbar & leave some pixels at the edge of the window for the scrollbar
         gfx_SetColor(LIGHT_GREY);
         gfx_FillRectangle_NoClip(36,fileY-5,242,15);
			gfx_SetColor(BLACK);
			gfx_Rectangle_NoClip(36,fileY-5,242,15);

      }
		
      gfx_SetTextFGColor(BLACK);
		
      gfx_PrintStringXY(HS->fileNames[i],40,fileY);
      gfx_SetTextXY(135,fileY);
      gfx_PrintInt(fileSize,4);
      gfx_SetTextFGColor(0);
      fileY+=15;
      ii++;
   }
   // display when no files were detected because you forgot to take notes :P
   if (HS->numFiles == 0) {
      gfx_SetTextFGColor(244);
      gfx_PrintStringXY("--NO FILES FOUND--)",93,80);
      gfx_PrintStringXY("That's too bad for you :(",93,100);
   }

   // Return the number of files displayed, I guess. This really isn't necessary at all btw. If I run low on bytes in the stack, which is very unlikely (since I am writing in C && am not defining a ton of variables && am such an efficient programmer lol && never use globals && always use tons of small functions), I will delete this. :P
   return i;
}

// homescreen for the fileViewer, rectangles, title, etc...
static void dispHomeScreenBG(struct fileViewerStruct * HS) {
	int width;
	
	// scrollbar math
	uint8_t scrollbarHeight = 148 * HS->numFilesDisplayed / HS->numFiles;
	
	// just making sure that the scrollbar is a reasonable size...
	if(scrollbarHeight>148)
		scrollbarHeight = 148;
	if(scrollbarHeight<10)
		scrollbarHeight = 10;
	
	int scrollbarX = 280;
	int scrollbarY = (150 - (scrollbarHeight) + 1) * HS->selectedFile / (HS->numFiles-1) + 56;
	
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

static uint8_t handleHomeScrnKeyPresses(struct fileViewerStruct *HS) {
   kb_Scan();

   // move cursor down
   if(kb_IsDown(kb_KeyDown) && HS->selectedFile < HS->numFiles-1) {
      HS->selectedFile++;
      if(HS->selectedFile >= HS->offset+10){
         HS->offset++;
      }
   }

	// move cursor up
   if (kb_IsDown(kb_KeyUp) && HS->selectedFile>0) {
      HS->selectedFile--;
      if(HS->selectedFile < HS->offset){
         HS->offset--;
      }
   }

	// open file
	if(kb_IsDown(kb_KeyYequ)) {
		// if there aren't any files to open...
		if(HS->numFiles <= 0) {
			alert("There aren't any files to open (obviously).");
			return CANCEL;
		}
		// otherwise...open the selected file.
		return OPEN;
	}
	
   // new file
   if (kb_IsDown(kb_KeyWindow)) {
		if(HS->numFiles >= 30 ) {
			alert("You can't have more than 30 files, my note-crazy friend!");
			return CANCEL;
		}
		newFile();
		loadFiles(HS);
		return true;
   }
	
	// quit program
	if (kb_IsDown(kb_KeyClear) || kb_IsDown(kb_KeyZoom)) {
		return QUIT;
	}
	
	// delete file
   if ((kb_IsDown(kb_KeyTrace) || kb_IsDown(kb_KeyDel))) {
		// make sure there is a file to delete
		if(HS->numFiles == 0) {
			alert("There aren't any files to delete!");
			return CANCEL;
		}
		// if there is at least 1 file...
		checkIfDeleteFile(HS->fileNames[HS->selectedFile]);
		loadFiles(HS);
		// check if you need to shift the cursor
		if(HS->numFiles > 0 && HS->selectedFile >= HS->numFiles)
			HS->selectedFile--;
			
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
				if(HS->numFiles>0) {
					if(renameFile(HS->fileNames[HS->selectedFile])) {
						loadFiles(HS);
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

//loads the data into the struct for the homescreen menu that is triggered by the "other" menu option (literally)

static uint8_t loadFiles(struct fileViewerStruct *HS) {
   uint8_t numFiles = 0;
   ti_var_t fileSlot = 0; // slot of currently detected file
	int fileSize;
   char * namePtr = NULL;
   void * search_pos = NULL; // mem location of the currently detected file in the VAT
		
   while ((namePtr = ti_Detect(&search_pos, HEADER_STR)) != NULL) {
		
		// copy the currently detected file's name into the fileviewer struct's names array
      strcpy(HS->fileNames[numFiles], namePtr);
		
		// get some info from the currently detected file
      fileSlot = ti_Open(namePtr, "r+");
		
      fileSize = ti_GetSize(fileSlot);
		
		// files have to be at least 10 bytes large for future formatting data purposes
		if(fileSize < MIN_FILE_SIZE) {
			ti_Seek(3, 0, fileSlot);
			ti_Write((const void *)0xE40000, MIN_FILE_SIZE-3, 1, fileSlot);
		}
		
		HS->fileSizes[numFiles] = fileSize;
		
		// "always close files after opening them" -Jacobly, ergo...
		ti_Close(fileSlot);
      numFiles++;
   }
		
   HS->numFiles = numFiles;
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
