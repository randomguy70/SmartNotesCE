#include "includes/menues.h"

#include <tice.h>
#include <keypadc.h>
#include <fontlibc.h>

#include "includes/ui.h"
#include "includes/homescreen.h"
#include "includes/text.h"

// XXX
int displayMenu(struct menu *menu)
{
	int selected = 0;
	bool wasPressed = false;
	int stringWidth;
	
	if(menu->numOptions > MAX_MENU_ENTRIES)
	{
		menu->numOptions = MAX_MENU_ENTRIES;
	}
	
	menu->width = 100; // eventually will be proportional to width of longest entry
	
	fontlib_SetWindow(menu->x, menu->y, menu->width, menu->height);
	
	while(true)
	{
		kb_Scan();
		
		gfx_Blit(gfx_screen);
		gfx_SetDraw(gfx_buffer);
		
		// box
		
		gfx_SetColor(WHITE);
		gfx_FillRectangle_NoClip(menu->x, menu->y, menu->width, menu->height);
		
		// outline
		
		gfx_SetColor(LIGHT_BLUE);
		gfx_HorizLine_NoClip(menu->x, menu->y, menu->width);
		gfx_HorizLine_NoClip(menu->x, menu->y + 1, menu->width);
		
		gfx_VertLine_NoClip(menu->x, menu->y, menu->height);
		gfx_VertLine_NoClip(menu->x + 1, menu->y, menu->height);
		
		gfx_VertLine_NoClip(menu->x + menu->width - 1 - WINDOW_BORDER_THICKNESS, menu->y, menu->height);
		gfx_VertLine_NoClip(menu->x + menu->width - WINDOW_BORDER_THICKNESS, menu->y, menu->height);
		
		fontlib_SetForegroundColor(BLACK);
		
		for(uint8_t i = 0; i < menu->numOptions; i++)
		{
			// selecting bar
			if(i == selected)
			{
				// fill selecting box
				if(kb_IsDown(kb_KeyEnter) || kb_IsDown(kb_Key2nd))
				{
					gfx_SetColor(DARK_GREY);
				}
				else
				{
					gfx_SetColor(LIGHT_GREY);
				}
				
				gfx_FillRectangle_NoClip(menu->x + WINDOW_BORDER_THICKNESS, menu->y + i * MENU_ENTRY_SPACING + WINDOW_BORDER_THICKNESS, menu->width - 2 * WINDOW_BORDER_THICKNESS, MENU_ENTRY_SPACING);
				
				// outline for selecting box
				gfx_SetColor(BLACK);
				gfx_Rectangle_NoClip(menu->x + WINDOW_BORDER_THICKNESS, menu->y + i * MENU_ENTRY_SPACING + WINDOW_BORDER_THICKNESS, menu->width - 2 * WINDOW_BORDER_THICKNESS, MENU_ENTRY_SPACING);
			}
			
			// text
			stringWidth = fontlib_GetStringWidth(menu->entry[i].string);
			fontlib_DrawStringXY(menu->entry[i].string, menu->x + (menu->width / 2 - stringWidth / 2), menu->y + i * MENU_ENTRY_SPACING + 5);
		}
		
		gfx_SwapDraw();
		
		// move selecting bar down
		if(kb_IsDown(kb_KeyDown) && selected < menu->numOptions - 1)
		{
			selected++;
			delay(100);
		}
		
		// move selecter bar up
		else if(kb_IsDown(kb_KeyUp) && selected>0)
		{
			selected--;
			delay(100);
		}
		
		// select an option
		if(wasPressed == true && !(kb_IsDown(kb_KeyEnter) || kb_IsDown(kb_Key2nd)))
		{
			return selected + 1;
		}
		
		// quit the menu (once every key has been released)
		if(kb_IsDown(kb_KeyClear))
		{
			while(kb_AnyKey()) kb_Scan();
			return 0;
		}
		
		wasPressed = (kb_IsDown(kb_Key2nd) || kb_IsDown(kb_KeyEnter));
	}
	
	return 0;
};

struct menu *loadHomeScreenAboutMenu(void)
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

struct menu *loadHomeScreenFileMenu(void)
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