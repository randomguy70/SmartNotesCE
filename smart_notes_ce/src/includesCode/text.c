#include "main.h"
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>
#include <fontlibc.h>
#include <string.h>

#include <includes/text.h>
#include <includes/ui.h>

enum txt_mode {
	MATH = 1,
	CAPS,
	LOWER_CASE,
};

uint8_t inputString(char* buffer, uint8_t maxLength, const char * title)
{
   uint8_t keyPressed; // value of key currently pressed
   uint8_t txtMode = CAPS; // caps, math, or lowercase
   uint8_t strLen = 0; // current character length & offset of inputted string
   char character; // current inputted character buffer
	
   uint16_t cursorX;
	uint16_t cursorY;
   uint8_t cursorBlink = 0;
	
	struct window window;
	window.width = 150;
	window.height = 50;
	window.x = (SCRN_WIDTH/2)-(window.width/2);
	window.y = (SCRN_HEIGHT/2)-(window.height/2);
	
	window.window_outline_color = LIGHT_BLUE;
	window.window_background_color = LIGHT_GREY;
	
	int titleX = (SCRN_WIDTH/2) - (gfx_GetStringWidth(title) / 2);
	
   while(1) {

		gfx_SetDraw(1);
		
		drawWindow(&window);
		
      // fill inner text box white
		int textBoxWidth = 72;
		int textBoxHeight = 15;
		int textBoxX = (SCRN_WIDTH/2)-(textBoxWidth/2);
		int textBoxY = window.y + 20;
      gfx_SetColor(1);
		gfx_FillRectangle(textBoxX, textBoxY, textBoxWidth, textBoxHeight);

      // inner text box outline
      gfx_SetColor(DARK_BLUE);
		gfx_Rectangle(textBoxX, window.y+20, textBoxWidth, textBoxHeight);

      // display alpha mode (either A, a, or 1)
		int alphaXPos = window.x + window.width - 10;
		int alphaYPos = window.y + 5;
      gfx_SetTextFGColor(BLACK);
		gfx_SetTextXY(alphaXPos, alphaYPos);
		
      if(txtMode == MATH) {
			gfx_PrintChar('1');
      }
      if(txtMode == CAPS) {
			gfx_PrintChar('A');
      }
      if(txtMode == LOWER_CASE) {
			gfx_PrintChar('a');
      }
		
		// display the title and inputted string
      gfx_SetTextFGColor(BLACK);
		gfx_PrintStringXY(title, titleX, (SCRN_HEIGHT/2)-(window.height/2)+5);
		gfx_PrintStringXY(buffer, (SCRN_WIDTH/2)-(72/2)+2, (SCRN_HEIGHT/2)-(window.height/2)+24);

      // display cursor
      cursorX = gfx_GetTextX() + 2;
		cursorY = gfx_GetTextY()-2;
      gfx_SetColor(DARK_BLUE);
		
		// deal with cursor cycles
      if(cursorBlink > 20) {
			gfx_VertLine(cursorX, cursorY, 11);
			gfx_VertLine(cursorX+1, cursorY, 11);
         if(cursorBlink == 40) {
            cursorBlink = 0;
         }
      }
      cursorBlink++;
      gfx_Blit(1);
		
		// keypresses
		{
		kb_Scan();
		keyPressed = os_GetCSC();
		
		// enter creates a new file with the inputted string for a name
      if (kb_IsDown(kb_KeyEnter) && strLen > 0 && strLen <= maxLength) { // enter finishes string input and returns 1
         return 1;
      }
		
		// clear quits and returns failure (0)
      if (kb_IsDown(kb_KeyClear)) {
			// wait for the delete key to be released before moving on
			while(kb_AnyKey()) kb_Scan();
				
			return 0;
		}
		
		// delete deletes one character (obviously)
      if (kb_IsDown(kb_KeyDel) && strLen>0) {
         buffer[strLen-1] = 0;
         strLen--;
			
			while(kb_AnyKey()) kb_Scan();
      }
		
		// switching text modes
      if (kb_IsDown(kb_KeyAlpha)) {
			if(txtMode == MATH) {
				txtMode = CAPS;
			}
			else if(txtMode == CAPS){
				txtMode = LOWER_CASE;
			}
			else if(txtMode == LOWER_CASE) {
				txtMode = MATH;
			}
			// wait for the delete key to be released before moving on
			while(kb_AnyKey()) kb_Scan();
		}
		
		// input character and add the character to the current offset in the string buffer
      // keyPressed = get_single_key_pressed();
      if (strLen < 8) {
         character = inputChar(txtMode, keyPressed);
         if (character != '\0' && strLen<=maxLength) {
               buffer[strLen] = character;
               strLen++;
         }
      }
		}
		
   }
}

uint8_t inputChar(uint8_t txtMode, uint8_t keyPressed)
{
   unsigned char mathDat[] = {
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 
      0x0, 0x0 , 0x2B, 0x2D, 0x2A, 0x2F, 0x5E, 0x0, 
      0x0, 0x2D, 0x33, 0x36, 0x39, 0x29, 0x0 , 0x0, 
      0x0, 0x2E, 0X32, 0x35, 0x38, 0x28, 0x0 , 0x0, 
      0x0, 0x30, 0x31, 0x34, 0x37, 0x2C, 0x0 , 0x0, 
      0x0, 0x0 , 0x1a, 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 
   };
   unsigned char capsDat[] = {
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 
      0x0, 0x0 , 0x22, 0x57, 0x52, 0x4D, 0x48, 0x0 , 
      0x0, 0x3F, 0x5B, 0x56, 0x51, 0x4C, 0x47, 0x0 , 
      0x0, 0x3A, 0X5A, 0x55, 0x50, 0x4B, 0x46, 0x43, 
      0x0, 0x20, 0x59, 0x54, 0x4F, 0x4A, 0x45, 0x42, 
      0x0, 0x0 , 0x58, 0x53, 0x4E, 0x49, 0x44, 0x41, 
   };
   unsigned char lowerCaseDat[] = {
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 
      0x0, 0x22, 0x22, 0x77, 0x72, 0x6d, 0x68, 0x0 , 
      0x0, 0x3f, 0x0 , 0x76, 0x71, 0x6c, 0x67, 0x0 , 
      0x0, 0x3a, 0X7a, 0x75, 0x70, 0x6b, 0x66, 0x63, 
      0x0, 0x20, 0x79, 0x74, 0x6f, 0x6a, 0x65, 0x62, 
      0x0, 0x0 , 0x78, 0x73, 0x6e, 0x69, 0x64, 0x61, 
   };

   char character = '\0';

	if (txtMode == MATH) {
		character = mathDat[keyPressed];
		return character;
   }
   if (txtMode == CAPS) {
      character = capsDat[keyPressed];
      return character;
   }
   if (txtMode == LOWER_CASE) {
      character = lowerCaseDat[keyPressed];
      return character;
   }
   return 0;
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

void fontlib_DrawStringXY(char *str, int x, int y) {
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