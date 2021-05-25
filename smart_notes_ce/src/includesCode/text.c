#include <stdint.h>
#include <fileioc.h>
#include <keypadc.h>
#include "graphx.h"
#include "includes/text.h"
#include "includes/key.h"

uint8_t inputString(char* buffer, uint8_t maxLength)
{
   uint8_t math = 1;
   uint8_t caps = 2;
   uint8_t lowerCase = 3;
   uint8_t keyPressed = 0; // value of key currently pressed
   uint8_t txtMode = 1; // caps, math, or lowercase. default is 2 (caps)
   uint8_t strLen = 0; //  current character length & offset of inputted string
   char character; //  current inputted character buffer
   uint8_t cursorX;
   uint8_t cursorBlink = 0;

   while(1) {
      kb_Scan();

      // clear quits and returns failure (0)
      if (kb_IsDown(kb_KeyClear)) {
         delay(200);
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
      if (keyPressed>0) {
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
      } else if(txtMode == caps) {
         gfx_PrintStringXY("A", 180, 95);
      } else if(txtMode == lowerCase) {
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
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 
      0x0, 0x0 , 0x22, 0x57, 0x52, 0x4D, 0x48, 0x0 , 
      0x0, 0x3F, 0x5B, 0x56, 0x51, 0x4C, 0x47, 0x0 , 
      0x0, 0x3A, 0X5A, 0x55, 0x50, 0x4B, 0x46, 0x43, 
      0x0, 0x20, 0x59, 0x54, 0x4F, 0x4A, 0x45, 0x42, 
      0x0, 0x0 , 0x58, 0x53, 0x4E, 0x49, 0x44, 0x41, 
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 
   };
   unsigned char capsDat[] = {
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 
      0x0, 0x0 , 0x2B, 0x2D, 0x2A, 0x2F, 0x5E, 0x0, 
      0x0, 0x2D, 0x33, 0x36, 0x39, 0x29, 0x0 , 0x0, 
      0x0, 0x2E, 0X32, 0x35, 0x38, 0x28, 0x0 , 0x0, 
      0x0, 0x30, 0x31, 0x34, 0x37, 0x2C, 0x0 , 0x0, 
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 
   };
   unsigned char lowerCaseDat[] = {
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 
      0x0, 0x3f, 0x3f, 0x41, 0x3e, 0x43, 0x72, 0x0, 
      0x0, 0x3a, 0x47, 0x4a, 0x4d, 0x3d, 0x0 , 0x0, 
      0x0, 0x42, 0X46, 0x49, 0x4c, 0x3c, 0x0 , 0x0, 
      0x0, 0x44, 0x45, 0x48, 0x4b, 0x40, 0x0 , 0x0, 
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 
      0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 
   };

   uint8_t math = 1;
   uint8_t caps = 2;
   uint8_t lowerCase = 3;
   char character = '\0';

	if (txtMode == math) {
		character = mathDat[keyPressed];
		return character;
   }
   else if (txtMode == caps) {
      character = capsDat[keyPressed];
      return character;
   }
   else if (txtMode == lowerCase) {
      character = capsDat[keyPressed] + 0x20;
      return character;
   }
   else {
      return 0;
   }
}

uint8_t copyString(char* inputStr, char* outputBuffer) {
   uint8_t strPos = 0;
   while(inputStr[strPos]!= '\0') {
      outputBuffer[strPos] = inputStr[strPos];
      strPos++;
   }
   if(!strPos) {
      return 0;
   } else {
      return strPos;
   }
}

/*
uint8_t deleteChar(uint8_t slot, short offset) {

}
*/
