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
	
	// header
	gfx_SetColor(MEDIUM_GREY);
	gfx_FillRectangle_NoClip(0, 0, SCRN_WIDTH, 15);
	
	gfx_SetTextFGColor(LIGHT_BLUE);
	
	// footer
	gfx_SetColor(MEDIUM_GREY);
	gfx_FillRectangle_NoClip(0, 220, SCRN_WIDTH, 20);
	
	gfx_Blit(1);
	
	return;
	
}

static uint8_t handleEditorKeyPresses(struct editor *editor) {
	kb_Scan();
	
	if(kb_IsDown(kb_KeyClear)) {
		while(kb_AnyKey) kb_Scan();
		return QUIT;
	}
	
	if(kb_IsDown(kb_Key2nd)) {
		alert("Well well, it worked!!!");
		return CANCEL;
	}
	
}
