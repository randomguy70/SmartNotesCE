#include "main.h"

//// declarations
static void dispEditorBK(struct editor *editor);
static uint8_t handleEditorKeyPresses(struct editor *editor);


uint8_t dispEditor(struct editor *editor) {
	uint8_t action; // action triggered by keypresses
	
	while(true) {
		dispEditorBK(editor);
		
		action = handleEditorKeyPresses(editor);
		
		if(action == QUIT)
			return QUIT;
	}
}

// displays the editor background
static void dispEditorBK(struct editor *editor) {
	
	gfx_SetDraw(1);
	
	gfx_FillScreen(WHITE);
	
	// header rect
	gfx_SetColor(MEDIUM_GREY);
	gfx_FillRectangle_NoClip(0, 0, SCRN_WIDTH, 20);
	gfx_SetColor(BLACK);
	gfx_HorizLine_NoClip(0, 20, SCRN_WIDTH);
	
	// header txt
	fontlib_SetTransparency(true);
	fontlib_SetForegroundColor(BLACK);
	fontlib_SetCursorPosition(1, 2);
	fontlib_DrawString(editor->file.full_name);
	
	// footer
	gfx_SetColor(MEDIUM_GREY);
	gfx_FillRectangle_NoClip(0, 220, SCRN_WIDTH, 20);
	gfx_SetColor(BLACK);
	gfx_HorizLine_NoClip(0, 220, SCRN_WIDTH);
	
	gfx_Blit(1);
	
	waitForInput();
	return;
	
}

static uint8_t handleEditorKeyPresses(struct editor *editor) {
	kb_Scan();
	
	if(kb_IsDown(kb_KeyClear)) {
		return QUIT;
	}
	
	if(kb_IsDown(kb_Key2nd)) {
		alert("Well well, it worked!!!");
		return CANCEL;
	}
	
}
