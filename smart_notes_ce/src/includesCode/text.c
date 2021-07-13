#include "main.h"

uint8_t inputString(char* buffer, uint8_t maxLength, const char * title)
{
   uint8_t keyPressed = 0; // value of key currently pressed
   uint8_t txtMode = CAPS; // caps, math, or lowercase
   uint8_t strLen = 0; //  current character length & offset of inputted string
   char character; //  current inputted character buffer
   uint16_t cursorX;
	uint16_t cursorY;
   uint8_t cursorBlink = 0;
	uint16_t windowWidth = 100;
	uint16_t windowHeight = 50;

   while(1) {
      kb_Scan();

      // clear quits and returns failure (0)
      if (kb_IsDown(kb_Clear)) {
			return 0;
		}
			
		// enter creates a new file with the inputted string for a name
      if ((kb_IsDown(kb_KeyEnter)) && strLen > 0 && strLen <= maxLength) { // enter finishes string input and returns 1
         return 1;
      }
		
		// switching text modes
      if (os_GetCSC() == sk_Alpha) {
			if(txtMode == MATH) {
				txtMode = CAPS;
			}
			else if(txtMode == CAPS){
				txtMode = LOWER_CASE;
			}
			else if(txtMode == LOWER_CASE) {
				txtMode = MATH;
			}
		}
		
      // input character and add the character to the current offset in the string buffer
      keyPressed = get_single_key_pressed();
      if (keyPressed>0 && strLen<8) {
         character = inputChar(txtMode, keyPressed);
         if (character != '\0' && strLen<=maxLength) {
               buffer[strLen] = character;
               strLen++;
         }
      }

      if ((kb_IsDown(kb_KeyDel)) && strLen>0) {
         buffer[strLen-1] = 0;
         strLen--;;
         delay(80);
      }

		gfx_SetDraw(1);
		
      // display current string/new filename with outline box
      // outer text box fill
      gfx_SetColor(3); // fill rectangle light grey
      gfx_FillRectangle_NoClip((SCRN_WIDTH/2)-(windowWidth/2),(SCRN_HEIGHT/2)-(windowHeight/2),windowWidth,windowHeight);

      // outer text box outline
      gfx_SetColor(DARK_BLUE); // blue outline for text input outer box
		thick_Rectangle((SCRN_WIDTH/2)-(windowWidth/2),(SCRN_HEIGHT/2)-(windowHeight/2),windowWidth,windowHeight, 2);

      // inner text box fill
      gfx_SetColor(1); // fill inner text box white
		gfx_FillRectangle((SCRN_WIDTH/2)-(72/2),(SCRN_HEIGHT/2)-(windowHeight/2)+20, 72, 15);

      // inner text box outline
      gfx_SetColor(DARK_BLUE);
		gfx_Rectangle((SCRN_WIDTH/2)-(72/2),(SCRN_HEIGHT/2)-(windowHeight/2)+20, 72, 15);

      // display inputted text and alpha mode (either A, a, or 1)
      gfx_SetTextFGColor(BLACK);
		gfx_SetTextXY((SCRN_WIDTH/2)+(windowWidth/2)-10,(SCRN_HEIGHT/2)-(windowHeight/2)+5);
		
      if(txtMode == MATH) {
			gfx_PrintChar('1');
      }
      if(txtMode == CAPS) {
			gfx_PrintChar('A');
      }
      if(txtMode == LOWER_CASE) {
			gfx_PrintChar('a');
      }
		
      gfx_SetTextFGColor(BLACK);
		gfx_PrintStringXY(title, ((SCRN_WIDTH/2)-(windowWidth/2)+((SCRN_WIDTH/2)+(windowWidth/2)-10))/2-(gfx_GetStringWidth(title)/2), (SCRN_HEIGHT/2)-(windowHeight/2)+5);
		gfx_PrintStringXY(buffer, (SCRN_WIDTH/2)-(72/2)+2, (SCRN_HEIGHT/2)-(windowHeight/2)+24);

      // display cursor
      cursorX = gfx_GetTextX() + 2;
		cursorY = gfx_GetTextY()-2;
      gfx_SetColor(DARK_BLUE);
		
      if(cursorBlink > 15) {
			gfx_VertLine(cursorX, cursorY, 11);
			gfx_VertLine(cursorX+1, cursorY, 11);
         if(cursorBlink == 40) {
            cursorBlink = 0;
         }
      }
      cursorBlink++;
      gfx_Blit(1);
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

int varToArray(uint8_t slot, int varSize, char array[])
{
   int i;

   ti_Seek(10, 0, slot);

   for(i=0; i<varSize-10; i++) {
      array[i] = ti_GetC(slot);
   }
   return varSize;
}

int arrayToVar(char array[], int arraySize, uint8_t slot)
{
   ti_Resize(arraySize+10, slot);
   ti_Seek(10, 0, slot);
   ti_Write(array, 1, arraySize, slot);
   return 0;
}

int getWordLen(char* loc, struct wordStruct *word)
{
   int chars = 0;
	int pixelLen;
	char wordBuffer[200]; // make it large if somebody is messing around with typing random letters and no spaces :Ps
   while(loc[chars] != '\0' && loc[chars] != ' ') {
		wordBuffer[chars] = loc[chars];
      chars++;
   }
	wordBuffer[chars]='\0';
	pixelLen = gfx_GetStringWidth(wordBuffer);
	word->pixelLen=pixelLen;
	word->numChars = chars;
   return pixelLen;
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


/** copies a string into the end of another string (pointer)
 * @param dest pointer to first string
 * @param dest pointer to string that you are tacking on to the first string
**/
/*
int strcat(char *dest, char *src) {
	int charsRead = 0;
	int writePos = dest[strlen(dest)];
	
	while(src[charsRead]!='/0') {
		dest[writePos] = src[charsRead];
	}
	
	return charsRead;
}
*/