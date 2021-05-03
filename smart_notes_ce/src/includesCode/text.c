#include <stdint.h>
#include <fileioc.h>
#include <keypadc.h>
#include "graphx.h"

uint8_t inputString(char* buffer, uint8_t maxLength)
{
   uint8_t txtMode = 1;
   uint8_t result = 0;
   uint8_t fileSlot = 0;
   uint8_t strLen = 0;
   uint8_t cursorX;
   uint8_t cursorY;
   char *character = NULL;

   while (!result) {
      kb_Scan();

      if (kb_IsDown(kb_KeyClear)) {
         result = 2;
      } else if (kb_IsDown(kb_KeyEnter) && 0<strLen && strLen<=maxLength) {
         result = 1;
      } else if (kb_AnyKey() && strLen<9) {
         if(inputChar(character, txtMode)){
            buffer[strLen] = character;
            strLen++;
         }
      }
      
      if ((kb_IsDown(kb_KeyDel)) && strLen>0) {
         buffer[strLen] = 0;
         delay(100);
      }
      
      //display current string/new filename with outline box
      gfx_SetDraw(1);
      //big box fill
      gfx_SetColor(149);
      gfx_FillRectangle_NoClip(110,90,80,40);
      //big box outline
      gfx_SetColor(0);
      gfx_Rectangle_NoClip(110,90,80,40);
      //text box fill
      gfx_SetColor(255);
      gfx_FillRectangle_NoClip(115,110,70,13);
      //text box outline
      gfx_SetColor(0);
      gfx_Rectangle_NoClip(115,110,70,13);
      //display text
      gfx_SetTextFGColor(0);
      gfx_PrintStringXY("New file",120,95);
      gfx_PrintStringXY(buffer,117,113);
      //display cursor
      cursorX = gfx_GetTextX()+1;
      cursorY = gfx_GetTextY()-4;
      gfx_VertLine_NoClip(cursorX,cursorY,8);
      gfx_Blit(1);
   }
   return result;
}

uint8_t inputChar(char* buffer, uint8_t txtMode) {
   char result;
   uint8_t keyPressed;
   uint8_t mathSlot = ti_Open("MATH", "r"); // slot of math ascii data
   uint8_t capsSlot = ti_Open("CAPS", "r"); // slot of caps ascii data
   uint8_t lowerSlot; // slot of lowercase ascii data

	if (txtMode == 1) {
		if ((keyPressed = get_single_key_pressed()) > 0) { //math txtMode
			ti_Seek(keyPressed, 0, mathSlot);
			buffer = ti_GetC(mathSlot);
			result = 1;
      } else if (txtMode == 2) { // caps txtMode
		   if ((keyPressed = get_single_key_pressed()) > 0) {
            ti_Seek(keyPressed,0, capsSlot);
            buffer = ti_GetC(capsSlot);
            result = 1;
         }
      } else {
         result = 0;
      }
   }

   return result;
}

uint8_t deleteChar(uint8_t slot, short offset) {

}

