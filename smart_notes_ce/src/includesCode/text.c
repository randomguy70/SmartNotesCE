#include "main.h"

#define TEXT_ORIGIN 10
#define SCRNXMIN 15
#define SCRNYMIN 0
#define SCRNXMAX 310
#define SCRNYMAX 210

uint8_t inputString(char* buffer, uint8_t maxLength)
{
   uint8_t math = 1; // constant for value of math/numbers txt mode
   uint8_t caps = 2; // constant for value of capital letters txt mode
   uint8_t lowerCase = 3; // constant for value of lowerCase txt mode
   uint8_t keyPressed = 0; // value of key currently pressed
   uint8_t txtMode = caps; // caps, math, or lowercase
   uint8_t strLen = 0; //  current character length & offset of inputted string
   char character; //  current inputted character buffer
   uint8_t cursorX;
   uint8_t cursorBlink = 0;

   while(1) {
      kb_Scan();

      // clear quits and returns failure (0)
      if (chooseToQuit()) {
         return 0;
      }
      if ((kb_IsDown(kb_KeyEnter)) && strLen>0 && strLen<=maxLength) { // enter finishes string input and returns 1
         delay(150);
         return 1;
      }
      if (kb_IsDown(kb_KeyAlpha) && txtMode!=caps) {
         txtMode = caps;
         delay(100);
      } else if ((kb_IsDown(kb_KeyAlpha)) && txtMode!=lowerCase) {
         txtMode = lowerCase;
         delay(100);
      } else if ((kb_IsDown(kb_Key2nd))) {
         txtMode = math;
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

      // display current string/new filename with outline box
      gfx_SetDraw(1);
      // outer text box fill
      gfx_SetColor(3); // fill rectangle light grey
      gfx_FillRectangle_NoClip(110,90,80,40);

      // outer text box outline
      gfx_SetColor(6); // blue outline for text input outer box
      gfx_Rectangle_NoClip(109,89,82,42);
      gfx_Rectangle_NoClip(110,90,80,40);

      // inner text box fill
      gfx_SetColor(1); // fill inner text box white
      gfx_FillRectangle_NoClip(114,109,72,15);

      // inner text box outline
      gfx_SetColor(6); //  blue outline for text input inner box
      gfx_Rectangle_NoClip(114,109,72,15);

      // display inputted text and txtMode
      gfx_SetTextFGColor(0);
      if(txtMode == math) {
         gfx_PrintStringXY("1", 180, 95);
      }
      if(txtMode == caps) {
         gfx_PrintStringXY("A", 180, 95);
      }
      if(txtMode == lowerCase) {
         gfx_PrintStringXY("a", 180, 95);
      }
      gfx_SetTextFGColor(0);
      gfx_PrintStringXY("New file",120,95);
      gfx_PrintStringXY(buffer,117,113);
      gfx_SetTextFGColor(1);

      // display cursor
      cursorX = gfx_GetTextX()+1;
      gfx_SetColor(6);
      if(cursorBlink > 15) {
         gfx_VertLine_NoClip(cursorX, 111, 11);
         gfx_VertLine_NoClip(cursorX+1, 111, 11);
         if(cursorBlink == 40) {
            cursorBlink = 0;
         }
      }
      cursorBlink++;
      gfx_Blit(1);
   }
}

uint8_t inputChar(uint8_t txtMode, uint8_t keyPressed) {
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

int varToArray(uint8_t slot, int varSize, char array[]) {
   int i;

   ti_Seek(10, 0, slot);

   for(i=0; i<varSize-10; i++) {
      array[i] = ti_GetC(slot);
   }
   return varSize;
}

int arrayToVar(char array[], int arraySize, uint8_t slot) {
   ti_Resize(arraySize+10, slot);
   ti_Seek(10, 0, slot);
   ti_Write(array, 1, arraySize, slot);
   return 0;
}

int getLineLen(char* loc, struct lineStruct *lineBuffer) {
	int pos = 0; // byte offset we are reading from after the given *loc ptr
	int linePixelLen = 0;
	int chars = 0; // number of characters in the line
	char line[50] = {0};
	int curWordPixelLen = 0;
	
	while(1) {
		struct wordStruct word;
		curWordPixelLen = getWordLen(loc + pos, &word);

		// if the line contains at least 1 word, and adding the next word will make it too long, then return the current length of the line and ignore the next word
		if(linePixelLen + curWordPixelLen > SCRNXMAX && linePixelLen > 0) {
			lineBuffer->pixelLen=linePixelLen;
			lineBuffer->numChars=chars;
			return lineBuffer->numChars;
		}
		// if a single word is longer than a line because some nut was bored & messing around :P
		else if(linePixelLen == 0 && curWordPixelLen > SCRNXMAX) {
			while(linePixelLen < SCRNXMAX-5 && loc[pos] != '\0' && loc[pos] != ' ') { // 312 is slightly smaller than 315 so that a character won't go out of the text box
				line[pos] = loc[pos];
				linePixelLen += gfx_GetCharWidth(loc[pos]);
				pos++;
				chars++;
			}
			line[pos] = '\0';
			lineBuffer->pixelLen=gfx_GetStringWidth(line);
			lineBuffer->numChars=chars;
			return lineBuffer->numChars; // don't return the pixel length because I will be skipping through the text file based on the line offsets, not the pixel lengths. the pixel lengths are only for joining individual words into lines.
		}
		// add a word to the end of the line if it doesn't go beyond the screen
		else if(linePixelLen + curWordPixelLen < SCRNXMAX) {
			copyWord(&line[pos], &loc[pos]);
			linePixelLen+=getWordLen(&loc[pos], &word);
			pos+=word.numChars;
		}
	}
}

int getWordLen(char* loc, struct wordStruct *word) {
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

int copyWord(char* dest, char* src) {
   int pos = 0;
   while(src[pos]!='\0' && src[pos]!=' ') {
      dest[pos] = src[pos];
		pos++;
   }
	dest[pos] = '\0';
   return pos;
}


