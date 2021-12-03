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
#include "includes/buffer.h"
#include "includes/editor.h"

uint8_t inputString(char* buffer, uint8_t maxLength, const char * title)
{
	uint8_t strLen = 0;
	char character;
	
	struct inputState inputState = {
		.textMode = CAPS,
		.alphaPrev = false,
	};
	
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
		.windowOutlineColor = LIGHT_BLUE,
		.titleBarColor = DARK_GREY,
		.titleTextColor = BLACK,
		.title = title,
		.bodyColor = LIGHT_GREY,
		.bodyTextColor = BLACK,
	};
	
	unsigned int textBoxWidth = window.width - 10;
	unsigned int textBoxHeight = 17;
	unsigned int textBoxX = window.x + ((window.width/2) - (textBoxWidth/2));
	unsigned int textBoxY = window.y + WINDOW_TITLE_BAR_HEIGHT + 12;
	
	unsigned int alphaXPos = window.x + window.width - 10;
	unsigned int alphaYPos = window.y + 5;
	
	sk_key_t keyPressed;
	
	bool delete = false;
	bool deletePrev = false;
	
	while(true)
	{
		inputState.alphaPrev = kb_IsDown(kb_KeyAlpha);
		deletePrev = delete;
		
		kb_Scan();
		delete = kb_IsDown(kb_KeyDel);
		
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
		
		displayTextMode(alphaXPos, alphaYPos, inputState.textMode);
		updateInputMode(&inputState);
		
		// display input
		gfx_SetTextFGColor(BLACK);
		fontlib_DrawStringXY(buffer, textBoxX + 2, textBoxY + 2);
		
		// cursor
		cursor.x = textBoxX + fontlib_GetStringWidth(buffer) + 2;
		cursor.y = textBoxY + 2;
		
		drawCursor(&cursor);
		updateCursor(&cursor);
		
		gfx_Blit(1);
		
		kb_Scan();
		
		// success
		if (kb_IsDown(kb_KeyEnter) && strLen > 0 && strLen <= maxLength && strlen > 0)
		{
			while(kb_IsDown(kb_KeyEnter)) kb_Scan();
			return 0;
		}
		
		// quit
		if (kb_IsDown(kb_KeyClear))
		{
			while(kb_IsDown(kb_KeyClear)) kb_Scan();
			return 0;
		}
		
		// input character
		keyPressed = get_single_key_pressed();
		if (strLen < 8 && (keyPressed != sk_Alpha && keyPressed != sk_2nd && keyPressed != sk_Mode && keyPressed != sk_Del && keyPressed != sk_GraphVar && keyPressed != sk_Stat && keyPressed != sk_Enter))
		{
			character = inputChar(inputState.textMode, keyPressed);
			if (character != '\0' && strLen<=maxLength)
			{
				buffer[strLen] = character;
				strLen++;
			}
		}
		
		// delete character
		if (delete && !deletePrev && strLen > 0)
		{
			buffer[strLen-1] = '\0';
			strLen--;
		}
	}
	
	return 0;
}

char inputChar(enum textMode mode, uint8_t keyPressed)
{
	static const unsigned char mathDat[] = {
		0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 
		0x0, 0x0 , 0x2B, 0x2D, 0x2A, 0x2F, 0x5E, 0x0, 
		0x0, 0x2D, 0x33, 0x36, 0x39, 0x29, 0x0 , 0x0, 
		0x0, 0x2E, 0X32, 0x35, 0x38, 0x28, 0x0 , 0x0, 
		0x0, 0x30, 0x31, 0x34, 0x37, 0x2C, 0x0 , 0x0, 
		0x0, 0x0 , 0x1a, 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 
	};
	static const unsigned char capsDat[] = {
		0x0, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 
		0x0, 0x0 , 0x22, 0x57, 0x52, 0x4D, 0x48, 0x0 , 
		0x0, 0x3F, 0x5B, 0x56, 0x51, 0x4C, 0x47, 0x0 , 
		0x0, 0x3A, 0X5A, 0x55, 0x50, 0x4B, 0x46, 0x43, 
		0x0, 0x20, 0x59, 0x54, 0x4F, 0x4A, 0x45, 0x42, 
		0x0, 0x0 , 0x58, 0x53, 0x4E, 0x49, 0x44, 0x41, 
	};
	static const unsigned char lowerCaseDat[] = {
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
	
	return character;
}

int fontlib_strlen(char *string)
{
	int i = 0;
	
	while(string[i]!='\0' && string[i] != fontlib_GetAlternateStopCode())
	{
		i++;
	}
	
	return i;
}

// finds the character length of the next word in a file buffer
int getWordLen(char *start)
{
	int len = 0;
	char *pos = start;
	
	while(true)
	{
		if(isValidWordChar(*pos))
		{
			len++;
			pos++;
		}
		else
		{
			return len;
		}
	}
	return 0;
}

int getLineLen(char *start)
{
	char *pos = start;
	int wordLen;
	int wordWidth;
	int lineLen;
	int lineWidth;
	
	while(true)
	{
		wordLen = getWordLen(pos);
		wordWidth = fontlib_GetStringWidth(pos);
		
		// if word doesn't fit on to line and line already has at least 1 word
		if(lineWidth + wordWidth > EDITOR_TEXT_BOX_WIDTH && lineLen > 0)
		{
			return lineLen;
		}
		// if word does fit on line
		else if(lineWidth + wordWidth <= EDITOR_TEXT_BOX_WIDTH)
		{
			
		}
	}
}

void fontlib_DrawStringXY(const char *str, int x, int y)
{
	fontlib_SetCursorPosition(x, y);
	fontlib_DrawString(str);
	return;
}

// XXX NEEDS TO BE CONVERTED TO THE NEW BUFFER FORMAT
int copyWordL(char *dest, char *src, int chars)
{
	int pos = 0;
	
	while(pos < chars && src[pos] != '\0' && src[pos] != ' ')
	{
		dest[pos] = src[pos];
		pos++;
	}
	
	dest[pos] = '\0';
	
	return pos;
}

bool isValidWordChar(char character)
{
	if(character != ' ' && character != '\0' && character != '\n')
	{
		return true;
	}
	
	return false;
}

void displayTextMode(int x, int y, enum textMode textMode)
{
	gfx_SetColor(BLACK);
	gfx_SetTextXY(x, y);
	
	if(textMode == MATH)
	{
		gfx_PrintChar('1');
	}
	if(textMode == CAPS)
	{
		gfx_PrintChar('A');
	}
	if(textMode == LOWER_CASE)
	{
		gfx_PrintChar('a');
	}
}

void updateInputMode(struct inputState *inputState)
{
	kb_Scan();
	bool alpha = kb_IsDown(kb_KeyAlpha);
	bool second = kb_IsDown(kb_Key2nd);
	
	if(inputState->textMode == MATH && alpha && !inputState->alphaPrev)
	{
		inputState->textMode = CAPS;
	}
	else if(inputState->textMode == CAPS && alpha)
	{
		inputState->textMode = LOWER_CASE;
	}
	else if(inputState->textMode == LOWER_CASE && alpha)
	{
		inputState->textMode = CAPS;
	}
	else if(second)
	{
		inputState->textMode = MATH;
	}
	
	inputState->alphaPrev = alpha;
}