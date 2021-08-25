#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>
#include <string.h>
#include <tice.h>

#include <includes/homescreen.h>
#include <includes/text.h>
#include <includes/file.h>
#include <includes/ui.h>
#include <gfx/gfx.h>

static void dispFiles(struct file files[30], uint8_t offset, uint8_t selectedFile);
static void dispHomeScreenBG(struct homescreen* homescreen);
static void dispHomeScreenButtons(void);
static enum state handleHomeScreenKeyPresses(struct homescreen* homescreen);
static uint8_t loadFiles(struct file files[]);
static struct menu *loadHomeScreenOtherMenu(void);

enum state dispHomeScreen(struct homescreen* homescreen) {
	enum state ret = show_homescreen;
	
   homescreen->selectedFile = 0;
   homescreen->offset = 0;
	
	homescreen->numFiles = loadFiles(homescreen->files);
	
   while(true) {
		dispHomeScreenBG(homescreen);
		dispHomeScreenButtons();
		dispFiles(homescreen->files, homescreen->offset, homescreen->selectedFile);
		
		gfx_Wait();
		gfx_SwapDraw();
		
      ret = handleHomeScreenKeyPresses(homescreen);
		
		if(ret == should_exit || ret == show_editor || kb_IsDown(kb_KeyClear))
		{
			return ret;
		}
		
		if(os_GetCSC() == sk_Right) {
			toggleHiddenStatus(homescreen->files[homescreen->selectedFile].os_name);
			// homescreen->numFiles = loadFiles(homescreen->files);
		}
   }
	
	return should_exit;
}

static void dispFiles(struct file files[30], uint8_t offset, uint8_t selectedFile) {
   uint8_t i;
   unsigned int fileY = 61;
	unsigned int numFiles = 0;
	
	gfx_SetTextFGColor(BLACK);
	gfx_SetDraw(gfx_buffer);
	
   for(i=offset; i < 10+offset && files[i].os_name[0] != '\0' && i<30; i++) {

      // display currently selected file with a scrollbar on top of it
      if (selectedFile == i)
		{
			// draw scrollbar & leave some pixels at the edge of the window for the scrollbar
         gfx_SetColor(LIGHT_GREY);
         gfx_FillRectangle_NoClip(36,fileY-5,242,15);
			gfx_SetColor(BLACK);
			gfx_Rectangle_NoClip(36,fileY-5,242,15);
      }
		
      gfx_PrintStringXY(files[i].os_name,40,fileY);
      fileY+=FILE_SPACING;
   }
   // display when no files were detected because you forgot to take notes :P
   if (numFiles == 0)
	{
      gfx_SetTextFGColor(244);
      gfx_PrintStringXY("--NO FILES FOUND--)",93,80);
      gfx_PrintStringXY("That's too bad for you :(",93,100);
   }

   return;
}

static void dispHomeScreenBG(struct homescreen* homescreen) {
	int width;
	
	// scrollbar math
	uint8_t scrollbarHeight = 148 * 10 / homescreen->numFiles;
	
	// just making sure that the scrollbar is a reasonable size...
	if(scrollbarHeight>148)
		scrollbarHeight = 148;
	if(scrollbarHeight<10)
		scrollbarHeight = 10;
	
	int scrollbarX = 280;
	int scrollbarY = (150 - (scrollbarHeight) + 1) * homescreen->selectedFile / (homescreen->numFiles-1) + 56;
	
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
   gfx_PrintStringXY("STATUS",210,45);
	
	return;
}

static void dispHomeScreenButtons(void) {
	
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

static enum state handleHomeScreenKeyPresses(struct homescreen* homescreen) {
   kb_Scan();

   // move cursor down
   if(kb_IsDown(kb_KeyDown) && homescreen->selectedFile < homescreen->numFiles-1) {
      homescreen->selectedFile++;
      if(homescreen->selectedFile >= homescreen->offset+10){
         homescreen->offset++;
      }
		
		return show_homescreen;
   }

	// move cursor up
   if (kb_IsDown(kb_KeyUp) && homescreen->selectedFile>0) {
      homescreen->selectedFile--;
      if(homescreen->selectedFile < homescreen->offset){
         homescreen->offset--;
      }
		
		return show_homescreen;
   }
	
	// open file
	if(kb_IsDown(kb_KeyYequ))
	{
		if(homescreen->numFiles <= 0) {
			alert("There aren't any files to open (obviously).");
			return show_homescreen;
		}
		
		return show_editor;
	}
	
   // new file
   if (kb_IsDown(kb_KeyWindow))
	{
		if(homescreen->numFiles >= 30 )
		{
			alert("You can't have more than 30 files, my note-crazy friend!");
			return show_homescreen;
		}
		
		if(newFile())
		{
			loadFiles(homescreen->files);
			return show_homescreen;
		}
   }
	
	// quit program
	if (kb_IsDown(kb_KeyClear) || kb_IsDown(kb_KeyZoom))
	{
		return should_exit;
	}
	
	// delete file
   if ((kb_IsDown(kb_KeyTrace) || kb_IsDown(kb_KeyDel))) {
		
		if(homescreen->numFiles == 0)
		{
			alert("There aren't any files to delete!");
			return show_homescreen;
		}
		
		
		checkIfDeleteFile(homescreen->files[homescreen->selectedFile].os_name);
		// if I need to shift the cursor...
		if(homescreen->numFiles > 0 && homescreen->selectedFile >= homescreen->numFiles)
			homescreen->selectedFile--;
		
		return show_homescreen;
   }
	
	// other (opens fun menu with sprites)
	if(kb_IsDown(kb_KeyGraph)) {
		
		struct menu* menu = loadHomeScreenOtherMenu();
		uint8_t result = displayMenu(menu);
		
		switch(result)
		{
			
			// quit
			case QUIT:
				return should_exit;
				
			// back
			case 1:
				return show_homescreen;
			
			// rename
			case 2:
				if(homescreen->numFiles>0)
				{
					if(renameFile(homescreen->files[homescreen->selectedFile].os_name))
					{
						loadFiles(homescreen->files);
						return show_homescreen;
					}
				}
				
				// if it didn't return already then there aren't any files to rename...
				alert("There aren't any files to rename (obviously)!");
				return show_homescreen;
				
			// hide
			case 3: 
				toggleHiddenStatus(homescreen->files[homescreen->selectedFile].os_name);
				return show_homescreen;
				
			// settings
			case 4:
				// displaySettings(); // i haven't defined this...
				return show_homescreen;
				
			// if the user simply wants to close the menu
			default:
				return show_homescreen;
			
		}
	}

	return show_homescreen;
}

static uint8_t loadFiles(struct file files[]) {
	uint8_t numFiles  = 0;
	ti_var_t fileSlot = 0; // slot of currently detected file
	char *namePtr     = NULL;
	void *search_pos  = NULL; // mem location of the currently detected file in the VAT
	
	while ((namePtr = ti_Detect(&search_pos, HEADER_STR)) != NULL && numFiles < 30) {
		
		fileSlot = ti_Open(namePtr, "r");
		
		if(!fileSlot) {
			return false;
		}
		
		strcpy(files[numFiles].os_name, namePtr);
		files[numFiles].size = ti_GetSize(fileSlot);
		
		ti_Close(fileSlot);
		numFiles++;
	}
	
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
			{"Settings", settings_gear, settings_gear_height},
			{"Help", help, help_height},
		},
		
	};
	
	return &menu;
}
