#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>
#include <string.h>
#include <tice.h>
#include <fontlibc.h>

#include <includes/homescreen.h>
#include <includes/text.h>
#include <includes/file.h>
#include <includes/ui.h>
#include <gfx/gfx.h>

static void dispFiles(struct file files[], uint8_t numFiles, uint8_t offset, uint8_t selectedFile);
static void dispHomeScreenBG(struct homescreen* homescreen);
static void dispHomeScreenButtons(void);
static enum state handleHomeScreenKeyPresses(struct homescreen* homescreen);
static uint8_t loadFiles(struct file files[]);
static struct menu *loadHomeScreenOtherMenu(void);

enum state dispHomeScreen(struct homescreen* homescreen)
{
	enum state ret;
	
	homescreen->selectedFile = 0;
	homescreen->offset = 0;
	homescreen->numFiles = loadFiles(homescreen->files);
	
	while(true)
	{
		gfx_SetDraw(gfx_buffer);
		dispHomeScreenBG(homescreen); // not cause of crash
		dispHomeScreenButtons();      // not cause of crash
		dispFiles(homescreen->files, homescreen->numFiles, homescreen->offset, homescreen->selectedFile); // not cause of crash
		gfx_Wait();
		gfx_SwapDraw();
		
		kb_Scan();
		ret = handleHomeScreenKeyPresses(homescreen);
		
		if(ret == should_exit || ret == show_editor) 
		{
			return ret;
		}
	}
	
	return should_exit;
}

static void dispFiles(struct file files[], uint8_t numFiles, uint8_t offset, uint8_t selectedFile)
{
	uint8_t i;
	unsigned int fileY = 55;
	
	for(i=offset; i < MAX_FILES_VIEWABLE + offset && i<MAX_FILES_LOADABLE && i<numFiles; i++)
	{
		
		if (selectedFile == i)
		{
			// leave some pixels at the edge of the window for the scrollbar
			gfx_SetColor(LIGHT_GREY);
			gfx_FillRectangle_NoClip(36, fileY + 1, 242, 15);
			gfx_SetColor(BLACK);
			gfx_Rectangle_NoClip(36, fileY + 1, 242, 15);
		}
		
		fontlib_SetForegroundColor(BLACK);
		fontlib_DrawStringXY(files[i].os_name, 40, fileY);
		
		fileY += FILE_SPACING;
	}
	
	// display when no files were detected
	if (numFiles == 0)
	{
		char str1[] = "--NO FILES FOUND--)";
		char str2[] = "That's too bad for you :(";
		
		int x1 = (LCD_WIDTH / 2) - (fontlib_GetStringWidth(str1) / 2);
		int x2 = (LCD_WIDTH / 2) - (fontlib_GetStringWidth(str2) / 2);
		
		fontlib_SetForegroundColor(244);
		fontlib_DrawStringXY(str1, x1, 80);
		fontlib_DrawStringXY(str2, x2, 100);
	}
	
	return;
}

static void dispHomeScreenBG(struct homescreen *homescreen)
{
	int width;
	
	// scrollbar math (needs fixing)
	uint8_t scrollbarHeight = 148 * 10 / homescreen->numFiles;
	
	// make sure that the scrollbar is a reasonable size
	if(scrollbarHeight>148)
	{
		scrollbarHeight = 148;
	}
	if(scrollbarHeight<10)
	{
		scrollbarHeight = 10;
	}
	
	int scrollbarX = 280;
	int scrollbarY = (150 - (scrollbarHeight) + 1) * homescreen->selectedFile / (homescreen->numFiles-1) + 56;
	
	// lined-paper background
	gfx_FillScreen(PAPER_YELLOW);
	gfx_SetColor(LIGHT_BLUE);
	
	for(uint8_t i = 0; i<11; i++)
	{
		gfx_HorizLine_NoClip(0, i*20, SCRN_WIDTH);
		gfx_HorizLine_NoClip(0, i*20+1, SCRN_WIDTH);
	}

	// name and credits
	fontlib_SetForegroundColor(DARK_BLUE);
	width = fontlib_GetStringWidth("SMARTNOTES CE");
	fontlib_DrawStringXY("SMARTNOTES CE", (SCRN_WIDTH/2)-(width/2), 8);
	
	fontlib_SetForegroundColor(BLACK);
	width = fontlib_GetStringWidth("VERSION 1.0 BY Randomguy");
	fontlib_DrawStringXY("Version 1.0 BY Randomguy", (SCRN_WIDTH/2)-(width/2), 25);
	
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
	fontlib_SetForegroundColor(BLACK);
	fontlib_DrawStringXY("NAME",40,42);
	fontlib_DrawStringXY("STATUS",210,42);
	
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
	fontlib_SetForegroundColor(0);
	
	fontlib_DrawStringXY("Open" ,  27, 224);
	fontlib_DrawStringXY("New"  ,  90, 224);
	fontlib_DrawStringXY("Quit" , 157, 224);
	fontlib_DrawStringXY("Del"  , 220, 224);
	fontlib_DrawStringXY("Other", 271, 224);
	
}

static enum state handleHomeScreenKeyPresses(struct homescreen* homescreen)
{
	// quit
	if(kb_IsDown(kb_KeyClear) || kb_IsDown(kb_KeyZoom)) 
	{
		return should_exit;
	}
		
	// move cursor down
	if(kb_IsDown(kb_KeyDown) && homescreen->selectedFile < homescreen->numFiles-1)
	{
		homescreen->selectedFile++;
		if(homescreen->selectedFile >= homescreen->offset+10){
			homescreen->offset++;
		}
		
		return show_homescreen;
	}

	// move cursor up
	if (kb_IsDown(kb_KeyUp) && homescreen->selectedFile>0)
	{
		homescreen->selectedFile--;
		if(homescreen->selectedFile < homescreen->offset){
			homescreen->offset--;
		}
		
		return show_homescreen;
	}
	
	// open file
	if(kb_IsDown(kb_KeyYequ)|| kb_IsDown(kb_Key2nd) || kb_IsDown(kb_KeyEnter))
	{
		if(homescreen->numFiles <= 0)
		{
			return show_homescreen;
		}
		
		return show_editor;
	}
	
	// new file
	if (kb_IsDown(kb_KeyWindow))
	{
		if(homescreen->numFiles < 30)
		{
			if(newFile())
			{
				homescreen->numFiles = loadFiles(homescreen->files);
			}
		}
		
		alert("You can't have more than 30 files.");
		
		return show_homescreen;
	}
	
	// delete file
	if ((kb_IsDown(kb_KeyTrace) || kb_IsDown(kb_KeyDel)))
	{
		if(homescreen->numFiles <= 0)
		{
			alert("There aren't any files to delete!");
			return show_homescreen;
		}
		
		if(checkIfDeleteFile(homescreen->files[homescreen->selectedFile].os_name))
		{
			// shift the cursor...
			if(homescreen->numFiles > 0)
			{
				homescreen->selectedFile--;
				if(homescreen->offset > homescreen->selectedFile)
				{
					homescreen->offset--;
				}
			}
			homescreen->numFiles = loadFiles(homescreen->files);
		}
		
		return show_homescreen;
	}
	
	if(kb_IsDown(kb_KeyGraph)) {
		
		struct menu* menu = loadHomeScreenOtherMenu();
		uint8_t result = displayMenu(menu);
		
		switch(result)
		{
			// back
			case 1:
				break;
			
			// rename
			case 2:
				if(homescreen->numFiles>0)
				{
					bool result = renameFile(homescreen->files[homescreen->selectedFile].os_name);
					if(result)
					{
						loadFiles(homescreen->files);
						break;
					}
					alert("Something went wrong. Tough luck, buddy.");
					break;
				}
				
				alert("There aren't any files to rename (obviously)!");
				break;
				
			// hide
			case 3: 
				toggleHiddenStatus(homescreen->files[homescreen->selectedFile].os_name);
				loadFiles(homescreen->files);
				break;
				
			// settings
			case 4:
				// displaySettings();
				break;
				
			default:
				break;
		}
	}
	
	return show_homescreen;
}

static uint8_t loadFiles(struct file files[]) {
	uint8_t numFiles = 0;
	char *namePtr = NULL;
	void *search_pos = NULL;
	
	while ((namePtr = ti_Detect(&search_pos, HEADER_STR)) != NULL)
	{
		if (numFiles > 30)
		{
			return numFiles;
		}
		
		strcpy(files[numFiles].os_name, namePtr);
		numFiles++;
		
	}
	
	return numFiles;
}

static struct menu *loadHomeScreenOtherMenu(void)
{
	static struct menu menu =
	{
		.title = "Options",
		.x = 200, .y = 100,
		.numOptions = 5,
		.hasSprites = true,
		
		.entry =
		{
			{"Back", left_arrow, left_arrow_height},
			{"Rename", rename, rename_height},
			{"(un)Hide", hide, hide_height},
			{"Settings", settings_gear, settings_gear_height},
			{"Help", help, help_height},
		},
	};
	
	return &menu;
}
