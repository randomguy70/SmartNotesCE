#include <stdint.h>
#include <fileioc.h>
#include <keypadc.h>
#include "graphx.h"
#include "includes/text.h"

uint8_t inputString(char* buffer, uint8_t maxLength)
{
   uint8_t result = 0; // return of inputString()
   uint8_t keyPressed; // value of key currently pressed
   uint8_t txtMode = 1; // caps, math, or lowercase. default is 1(caps)
   uint8_t strLen = 0; //  current length & offset of inputted string
   char character = '\0'; //  current inputted character
   uint8_t cursorX;

   while (!result) {
      kb_Scan();

      // clear quits
      if (kb_IsDown(kb_KeyClear)) {
         result = 2;
         break;
      } else if ((kb_IsDown(kb_KeyEnter)) && 0<strLen && strLen<=maxLength) {
         result = 1;
         break;
      }

      if ((keyPressed = get_single_key_pressed()) > 0) {
         if ((character = inputChar(txtMode, keyPressed)) != NULL && strLen<=maxLength) {
               buffer[strLen] = character;
               strLen++;
         }
      }

      if ((kb_IsDown(kb_KeyDel)) && strLen>0) {
         buffer[strLen] = 0;
         strLen--;
         delay(100);
      }
      
      // display current string/new filename with outline box

      gfx_SetDraw(1);
      // outer text box fill
      gfx_SetColor(3); // fill rectangle light grey
      gfx_FillRectangle_NoClip(110,90,80,40);

      // outer text box outline
      gfx_SetColor(6); //  blue outline for text input outer box
      gfx_Rectangle_NoClip(110,90,80,40);

      // inner text box fill
      gfx_SetColor(1); // fill inner text box white
      gfx_FillRectangle_NoClip(115,110,70,13);

      // inner text box outline
      gfx_SetColor(6); //  blue outline for text input inner box
      gfx_Rectangle_NoClip(115,110,70,13);

      // display inputted text
      gfx_SetTextFGColor(0);
      gfx_PrintStringXY("New file",120,95);
      gfx_PrintStringXY(buffer,117,113);

      // display cursor
      cursorX = gfx_GetTextX()+1;
      gfx_SetColor(6);
      gfx_VertLine_NoClip(cursorX, 111, 12);
      gfx_VertLine_NoClip(cursorX+1, 111, 12);
      gfx_Blit(1);
   }
   return result;
}

uint8_t inputChar(uint8_t txtMode, uint8_t keyPressed) {
   char result = NULL;
   uint8_t mathSlot = ti_Open("MATHASCI", "r"); //  slot of math ascii data
   uint8_t capsSlot = ti_Open("LETTERAS", "r"); //  slot of caps ascii data
   uint8_t lowerSlot = 0; //  slot of lowercase ascii data

	if (txtMode == 1) {
		if ((keyPressed = get_single_key_pressed()) > 0) { // math txtMode
			ti_Seek(keyPressed, 0, mathSlot);
			result = ti_GetC(mathSlot);
      } else if (txtMode == 2) { //  caps txtMode
		   if ((keyPressed = get_single_key_pressed()) > 0) {
            ti_Seek(keyPressed, 0, capsSlot);
            result = ti_GetC(capsSlot);
         }
      } else {
         result = NULL;
      }
   }

   return result;
}
/*
uint8_t deleteChar(uint8_t slot, short offset) {

}
*/
