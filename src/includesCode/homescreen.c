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
static void dispHomeScreenBG(void);
static void dispHomeScreenButtons(void);
static void refreshHomeScreenGraphics(struct homescreen *homescreen);
static enum state handleHomeScreenKeyPresses(struct homescreen* homescreen);
static uint8_t loadFiles(struct file files[]);
static struct menu *loadHomeScreenAboutMenu(void);
static struct menu *loadHomeScreenFileMenu(void);

enum state dispHomeScreen(struct homescreen* homescreen)
{
	enum state ret;
	
	homescreen->selectedFile = 0;
	homescreen->offset = 0;
	homescreen->numFiles = loadFiles(homescreen->files);
	
	while(true)
	{
		kb_Scan();
		
		refreshHomeScreenGraphics(homescreen);
		
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
	const unsigned int fileX = FILE_VIEWER_X + 5;
	unsigned int fileY = FILE_VIEWER_Y + 3;
	
	for(i=offset; i < MAX_FILES_VIEWABLE + offset && i<MAX_FILES_LOADABLE && i<numFiles; i++)
	{
		if (selectedFile == i)
		{
			if(kb_IsDown(kb_Key2nd) || kb_IsDown(kb_KeyEnter))
			{
				gfx_SetColor(DARK_GREY);
			}
			else
			{
				gfx_SetColor(LIGHT_GREY);
			}
			
			gfx_FillRectangle_NoClip(FILE_VIEWER_X + WINDOW_BORDER_THICKNESS, fileY - 1, FILE_VIEWER_WIDTH - 4, 15);
			gfx_SetColor(BLACK);
			gfx_Rectangle_NoClip(FILE_VIEWER_X + WINDOW_BORDER_THICKNESS, fileY - 1, FILE_VIEWER_WIDTH - 4, 15);
		}
		
		fontlib_SetForegroundColor(BLACK);
		fontlib_DrawStringXY(files[i].os_name, fileX, fileY);
		
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

static void dispHomeScreenBG(void)
{
	int width;
	
	fontlib_SetAlternateStopCode(0);
	fontlib_SetWindowFullScreen();
	
	// lined-paper background
	gfx_FillScreen(PAPER_YELLOW);
	gfx_SetColor(LIGHT_BLUE);
	
	for(uint8_t i = 0; i<11; i++)
	{
		gfx_HorizLine_NoClip(0, i*20, LCD_WIDTH);
		gfx_HorizLine_NoClip(0, i*20+1, LCD_WIDTH);
	}

	// name and credits
	fontlib_SetForegroundColor(DARK_BLUE);
	width = fontlib_GetStringWidth("SMARTNOTES CE");
	fontlib_DrawStringXY("SMARTNOTES CE", (LCD_WIDTH/2)-(width/2), 5);
	
	fontlib_SetForegroundColor(BLACK);
	width = fontlib_GetStringWidth("V.1 by Randomguy");
	fontlib_DrawStringXY("V.1 by Randomguy", (LCD_WIDTH/2)-(width/2), 25);
	
	// box with file names
	gfx_SetColor(WHITE);
	gfx_FillRectangle_NoClip(FILE_VIEWER_X, FILE_VIEWER_Y, FILE_VIEWER_WIDTH, FILE_VIEWER_HEIGHT);
	gfx_SetColor(LIGHT_BLUE);
	thick_Rectangle(FILE_VIEWER_X, FILE_VIEWER_Y, FILE_VIEWER_WIDTH, FILE_VIEWER_HEIGHT, 2);
	
	// print labels for displayed file data columns
	fontlib_SetForegroundColor(BLACK);
	fontlib_DrawStringXY("NAME", FILE_VIEWER_X + 2, FILE_VIEWER_Y - 13);
	width = fontlib_GetStringWidth("STATUS");
	fontlib_DrawStringXY("STATUS", FILE_VIEWER_X + FILE_VIEWER_WIDTH - width - 2, FILE_VIEWER_Y - 13);
	
	return;
}

static void dispHomeScreenButtons(void)
{
	const char *text[NUM_HOMESCREEN_BUTTONS] = {"Exit", "Refresh", "About", "Settings", "File"};
	const uint8_t keys[5] = {kb_Yequ, kb_Window, kb_Zoom, kb_Trace, kb_Graph};
	const uint8_t spacing = LCD_WIDTH / NUM_HOMESCREEN_BUTTONS;
	int i, x;
	
	// bar at bottom
	gfx_SetColor(LIGHT_GREY);
	gfx_FillRectangle_NoClip(0, LCD_HEIGHT - 25, LCD_WIDTH, 25);
	
	// line on top of bar
	gfx_SetColor(BLACK);
	gfx_HorizLine_NoClip(0, LCD_HEIGHT - 25, LCD_WIDTH);
		
	fontlib_SetForegroundColor(0);
	
	for(i = 0; i < NUM_HOMESCREEN_BUTTONS; i++)
	{
		if(kb_Data[1])
		{
			if(kb_Data[1] & keys[i])
			{
				gfx_SetColor(LIGHT_BLUE);
				gfx_FillRectangle_NoClip(i*spacing, LCD_HEIGHT - 24, spacing, 24);
			}
		}
		
		x = (i * spacing) + (spacing / 2) - (fontlib_GetStringWidth(text[i]) / 2);
		fontlib_DrawStringXY(text[i], x, LCD_HEIGHT - 19);
	}
}

static void refreshHomeScreenGraphics(struct homescreen *homescreen)
{
	gfx_SetDraw(gfx_buffer);
	dispHomeScreenBG();
	dispHomeScreenButtons();
	dispFiles(homescreen->files, homescreen->numFiles, homescreen->offset, homescreen->selectedFile);
	gfx_Wait();
	gfx_SwapDraw();
}

static enum state handleHomeScreenKeyPresses(struct homescreen *homescreen)
{
	// quit
	if(kb_IsDown(kb_KeyClear) || kb_IsDown(kb_KeyYequ)) 
	{	
		while(kb_AnyKey()) kb_Scan();
		return should_exit;
	}
	
	// refresh
	if(kb_IsDown(kb_KeyWindow))
	{
		homescreen->selectedFile = 0;
		homescreen->offset = 0;
		homescreen->numFiles = loadFiles(homescreen->files);
				
		return show_homescreen;
	}
	
	// File Options Menu
	if(kb_IsDown(kb_KeyGraph))
	{
		
		struct menu* menu = loadHomeScreenFileMenu();
		uint8_t result = displayMenu(menu);
		
		switch(result)
		{
			// New
			case 1:
				newFile();
				homescreen->numFiles = loadFiles(homescreen->files);
				break;
			
			// Open
			case 2:
				return show_editor;
				break;
			
			// rename
			case 3:
				if(homescreen->numFiles>0)
				{
					bool result = renameFile(homescreen->files[homescreen->selectedFile].os_name);
					if(result)
					{
						homescreen->numFiles = loadFiles(homescreen->files);
						break;
					}
					
					alert("Something went wrong. Tough luck, buddy.");
					break;
				}
				
				alert("There aren't any files to rename (obviously)!");
				break;
				
			// Delete
			case 4:
				checkIfDeleteFile(homescreen->files[homescreen->selectedFile].os_name);
				homescreen->numFiles = loadFiles(homescreen->files);
				break;
			
			// (un) Hide
			case 5: 
				toggleHiddenStatus(homescreen->files[homescreen->selectedFile].os_name);
				homescreen->numFiles = loadFiles(homescreen->files);
				break;
			
			default:
				break;
		}
		
		return show_editor;
	}
	
	// XXX About Menu
	if(kb_IsDown(kb_KeyZoom))
	{
		struct menu* menu = loadHomeScreenAboutMenu();
		uint8_t result = displayMenu(menu);
		
		switch (result)
		{
			default: break;
		}
		return show_homescreen;
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
	if(kb_IsDown(kb_KeyEnter) || kb_IsDown(kb_Key2nd))
	{
		return show_editor;
	}
	
	// delete file
	if (kb_IsDown(kb_KeyDel))
	{
		if(homescreen->numFiles <= 0)
		{
			alert("There aren't any files to delete!");
			return show_homescreen;
		}
		
		if(checkIfDeleteFile(homescreen->files[homescreen->selectedFile].os_name))
		{
			// shift cursor
			if(homescreen->numFiles > 0)
			{
				if(homescreen->selectedFile > 0)
				{
					homescreen->selectedFile--;
				}
				if(homescreen->offset > homescreen->selectedFile)
				{
					homescreen->offset = homescreen->selectedFile;
				}
			}
		}
		
		homescreen->numFiles = loadFiles(homescreen->files);
		return show_homescreen;
	}
	
	return show_homescreen;
}

static uint8_t loadFiles(struct file files[])
{
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

static struct menu *loadHomeScreenAboutMenu(void)
{
	const int width = 90;
	const int height = 62;
	
	static struct menu menu = 
	{
		.title = "About",
		.x = (64*2) + ((LCD_WIDTH / NUM_HOMESCREEN_BUTTONS) / 2) - (width / 2),
		.y = LCD_HEIGHT - height - 25,
		.width = width,
		.height = height,
		.numOptions = 3,
		.hasSprites = false,
		
		.entry = 
		{
			{"Credits"},
			{"Help"},
			{"Build Info"},
		}
	};
	
	return &menu;
}

static struct menu *loadHomeScreenFileMenu(void)
{
	const int width = 100;
	const int numOptions = 7;
	
	static struct menu menu =
	{
		.title = "Options",
		.width = 100,
		.height = MENU_ENTRY_SPACING * numOptions + WINDOW_BORDER_THICKNESS,
		.x = LCD_WIDTH - width + 1,
		.y = 73,
		.numOptions = 7,
		.hasSprites = false,
		
		.entry =
		{
			{"New"},
			{"Open"},
			{"Rename"},
			{"Delete"},
			{"(un)Hide"},
			{"(un)Encrypt"},
			{"Back"},
		},
	};
	
	return &menu;
}