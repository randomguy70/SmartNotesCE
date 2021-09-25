#include "main.h"
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>
#include <fontlibc.h>
#include <string.h>

#include "includes/text.h"
#include "includes/ui.h"
#include "includes/colors.h"
#include "includes/key.h"

uint8_t inputString(char* buffer, uint8_t maxLength, const char * title)
{
   enum txt_mode txtMode = CAPS;
   uint8_t strLen = 0;
   char character;
	
   struct cursor cursor = {
		.animation_cycles_completed = 0,
		.cycles_per_animation = 40,
		.invisibleTime = 15,
	};
	
	struct window window = {
		.width = 150,
		.height = 50,
		.x = (LCD_WIDTH/2)-(150/2),
		.y = (LCD_HEIGHT/2)-(50/2),
		.window_outline_color = LIGHT_BLUE,
		.title_bar_color = DARK_GREY,
		.title_text_color = BLACK,
		.title = title,
		.body_color = LIGHT_GREY,
		.body_text_color = BLACK,
	};
	
	// text box
	unsigned int textBoxWidth = window.width - 10;
	unsigned int textBoxHeight = 17;
	unsigned int textBoxX = window.x + ((window.width/2) - (textBoxWidth/2));
	unsigned int textBoxY = window.y + WINDOW_TITLE_BAR_HEIGHT + 12;
	
	unsigned int alphaXPos = window.x + window.width - 10;
	unsigned int alphaYPos = window.y + 5;
	
	sk_key_t keyPressed;
	
	bool second = false;
	bool alpha  = false;
	bool delete    = false;
	
	bool alpha_prev;
	bool delete_prev;
	
   while(true)
	{
		kb_Scan();
		
		alpha_prev = alpha;
		delete_prev = delete;
		
		second = kb_IsDown(kb_Key2nd);
		alpha  = kb_IsDown(kb_KeyAlpha);
		delete    = kb_IsDown(kb_KeyDel);
		
		gfx_SetDraw(gfx_buffer);
		
		drawWindow(&window);
		
		// text box
      gfx_SetColor(WHITE);
		gfx_FillRectangle_NoClip(textBoxX, textBoxY, textBoxWidth, textBoxHeight);
		
		gfx_SetColor(LIGHT_BLUE);
		gfx_Rectangle_NoClip(textBoxX, textBoxY, textBoxWidth, textBoxHeight);
		
      // text mode (caps, lowercase, numbers)
      gfx_SetTextFGColor(BLACK);
		gfx_SetTextXY(alphaXPos, alphaYPos);
		
      if(txtMode == MATH)
			gfx_PrintChar('1');
      if(txtMode == CAPS)
			gfx_PrintChar('A');
      if(txtMode == LOWER_CASE)
			gfx_PrintChar('a');
		
		if(txtMode == MATH && alpha) {
			txtMode = CAPS;
		}
		else if(txtMode == CAPS && alpha && !alpha_prev) {
			txtMode = LOWER_CASE;
		}
		else if(txtMode == LOWER_CASE && alpha && !alpha_prev) {
			txtMode = CAPS;
		}
		else if((txtMode == LOWER_CASE || txtMode == CAPS) && second) {
			txtMode = MATH;
		}
		
		// display the title and inputted string
		gfx_SetTextFGColor(BLACK);
		fontlib_DrawStringXY(buffer, textBoxX + 2, textBoxY + 2);
		
		// display cursor
		cursor.x = textBoxX + fontlib_GetStringWidth(buffer) + 2;
		cursor.y = textBoxY + 2;
		
		drawCursor(&cursor);
		updateCursor(&cursor);
		
		gfx_Blit(1);
		
		kb_Scan();
		
		// exit successfully
		if (kb_IsDown(kb_KeyEnter) && strLen > 0 && strLen <= maxLength && strlen > 0)
		{
			return 1;
		}
		
		// quit
		if (kb_IsDown(kb_KeyClear))
		{
			while(kb_AnyKey()) kb_Scan();
			
			return 0;
		}
		
		// delete character
      if (delete && !delete_prev && strLen > 0)
		{
         buffer[strLen-1] = '\0';
         strLen--;
      }
		
		// input character
		keyPressed = os_GetCSC();
      if (strLen < 8 && (keyPressed != sk_Alpha && keyPressed != sk_2nd))
		{
			character = inputChar(txtMode, keyPressed);
			if (character != '\0' && strLen<=maxLength)
			{
				buffer[strLen] = character;
				strLen++;
			}
		}
	}
}

char inputChar(enum txt_mode mode, uint8_t keyPressed)
{
   const unsigned char mathDat[] = {
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 
      0x0, 0x0 , 0x2B, 0x2D, 0x2A, 0x2F, 0x5E, 0x0, 
      0x0, 0x2D, 0x33, 0x36, 0x39, 0x29, 0x0 , 0x0, 
      0x0, 0x2E, 0X32, 0x35, 0x38, 0x28, 0x0 , 0x0, 
      0x0, 0x30, 0x31, 0x34, 0x37, 0x2C, 0x0 , 0x0, 
      0x0, 0x0 , 0x1a, 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 
   };
   const unsigned char capsDat[] = {
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 
      0x0, 0x0 , 0x22, 0x57, 0x52, 0x4D, 0x48, 0x0 , 
      0x0, 0x3F, 0x5B, 0x56, 0x51, 0x4C, 0x47, 0x0 , 
      0x0, 0x3A, 0X5A, 0x55, 0x50, 0x4B, 0x46, 0x43, 
      0x0, 0x20, 0x59, 0x54, 0x4F, 0x4A, 0x45, 0x42, 
      0x0, 0x0 , 0x58, 0x53, 0x4E, 0x49, 0x44, 0x41, 
   };
   const unsigned char lowerCaseDat[] = {
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 
      0x0, 0x22, 0x22, 0x77, 0x72, 0x6d, 0x68, 0x0 , 
      0x0, 0x3f, 0x0 , 0x76, 0x71, 0x6c, 0x67, 0x0 , 
      0x0, 0x3a, 0X7a, 0x75, 0x70, 0x6b, 0x66, 0x63, 
      0x0, 0x20, 0x79, 0x74, 0x6f, 0x6a, 0x65, 0x62, 
      0x0, 0x0 , 0x78, 0x73, 0x6e, 0x69, 0x64, 0x61, 
   };

   char character = '\0';
	
	if (mode == MATH) {
		character = mathDat[keyPressed];
		return character;
   }
   if (mode == CAPS) {
      character = capsDat[keyPressed];
      return character;
   }
	
   if (mode == LOWER_CASE) {
      character = lowerCaseDat[keyPressed];
      return character;
   }
	
   return '\0';
}

int fontlib_GetStrLen(const char *string) {
	int i = 0;
	
	while(string[i]!='\0' && string[i]!=' ')
		i++;
		
	return i;
}

int copyWord(char* dest, char* src)
{
   int pos = 0;
   while(src[pos]!='\0' && src[pos]!=' ') {
      dest[pos] = src[pos];
		pos++;
   }
	dest[pos] = '\0';
   return pos;
}

int getWordLen(char *src) {
	int chars = 0;
	
	while(src[chars] != '\0' && src[chars] != ' ')
		chars++;
	
	return chars;
}

void fontlib_DrawStringXY(const char *str, int x, int y) {
	fontlib_SetCursorPosition(x, y);
	fontlib_DrawString(str);
	return;
}

int copyWordL(char *dest, char *src, int chars) {
	int pos = 0;
	
	while(pos < chars && src[pos] != '\0' && src[pos] != ' ')
	{
		dest[pos] = src[pos];
		pos++;
	}
	
	dest[pos] = '\0';
	
	return pos;
}

int getMaxCharsPerLine(char *src) {
	unsigned int maxLineWidth = fontlib_GetWindowWidth();
	unsigned int lineLen = 0; // the pixe length of the current line
	uint8_t chars = 0; // how many characters are fitting on the line currently
	
	while(lineLen < maxLineWidth)
		lineLen = fontlib_GetStringWidthL(src, chars++);
	
	return chars;
}

int getByteDifference(void *ptrOne, void *ptrTwo) {
	int byteDifference = 0;
	
	if(ptrOne <= ptrTwo) {
		while(ptrOne + byteDifference++ < ptrTwo){}
		
		return byteDifference;
	}
	
	while(ptrTwo + byteDifference++ < ptrOne){}
	
	return byteDifference;
}

int drawSpace() {
	int x, y;
	
	x = fontlib_GetCursorX() + 4;
	y = fontlib_GetCursorY();
	
	fontlib_SetCursorPosition(x, y);
	
	return x;
}