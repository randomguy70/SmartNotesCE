#ifndef MENUES_H
#define MENUES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <graphx.h>

#define MENU_ENTRY_SPACING       20
#define MAX_MENU_ENTRIES         10
#define MAX_MENU_ENTRIES_VISIBLE  7

struct menu_entry {
	const char *string;
	gfx_sprite_t *sprite;
};

// thank you mateo(again)
struct menu {
	const char *title;
	int x, y, width, height;
	uint8_t numOptions;
	uint8_t selected;
	bool hasSprites;
	struct menu_entry entry[];
};

/** displays a menu with sprites (optional)
 * @param menu pointer to a menu struct ALREADY initialised with config data and text
 * */
int displayMenu(struct menu *menu);

struct menu *loadHomeScreenAboutMenu(void);
struct menu *loadHomeScreenFileMenu(void);

#ifdef __cplusplus
}
#endif

#endif