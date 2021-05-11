#include <stdint.h>
#include <fileioc.h>
#include <keypadc.h>
#include "graphx.h"
#include "includes/text.h"
#include "includes/key.h"

uint8_t inputString(char* buffer, uint8_t maxLength)
{
   uint8_t result = 0; // return of inputString(), 1=success, 0=break
   uint8_t keyPressed = 0; // value of key currently pressed
   uint8_t txtMode = 1; // caps, math, or lowercase. default is 1 (caps)
   uint8_t strLen = 0; //  currently length & offset of inputted string
   char character = '\0'; //  current inputted character
   uint8_t cursorX;

   while(1) {
      kb_Scan();

      // clear quits and returns failure (0)
      if (kb_IsDown(kb_KeyClear)) {
         result = 0;
         return result;
      }
      // enter finishes string input and returns 1
      if ((kb_IsDown(kb_KeyEnter)) && strLen>0 && strLen<=maxLength) {
         result = 1;
         return result;
      }

      // input character and add the character to the current offset in the string buffer
      keyPressed = get_single_key_pressed();
      
      if (keyPressed) {
         character = inputChar(txtMode, keyPressed);
         if (character && strLen<=maxLength) {
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
      gfx_SetColor(6); // blue outline for text input outer box
      gfx_Rectangle_NoClip(109,89,82,42);
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
   uint8_t mathSlot = ti_Open("MATHASCI", "r"); // slot of math ascii data
   uint8_t capsSlot = ti_Open("LETTERAS", "r"); // slot of caps ascii data
   //uint8_t lowerSlot = 0; // slot of lowercase ascii data

	if (txtMode == 1 && keyPressed) { // math txtMode
		ti_Seek(keyPressed, 0, mathSlot);
		return(ti_GetC(mathSlot));
   }
   else if (txtMode == 2 && keyPressed) { // caps txtMode
      ti_Seek(keyPressed, 0, capsSlot);
      return(ti_GetC(capsSlot));
   }
   else {
      return 0;
   }
}

/*
uint8_t deleteChar(uint8_t slot, short offset) {

}
*/
