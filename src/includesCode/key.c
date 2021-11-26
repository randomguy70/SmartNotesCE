#include <stdint.h>
#include <keypadc.h>
#include <stdbool.h>

#include "includes/key.h"

/**
 * returns the os_GetCSC() keypress value using kb_Data[] & kb_Scan()
 * * created by Jacobly
**/
uint8_t get_single_key_pressed(void) {
	static uint8_t last_key;
	uint8_t only_key = 0;
	kb_Scan();
	for (uint8_t key = 1, group = 7; group; --group) {
		for (uint8_t mask = 1; mask; mask <<= 1, ++key) {
			if (kb_Data[group] & mask) {
				if (only_key) {
					last_key = 0;
					return 0;
				}
				else {
					only_key = key;
				}
			}
		}
	}
	if (only_key == last_key)
	{
		return 0;
	}
	last_key = only_key;
	return only_key;
}

bool pressed_Up() {
	kb_Scan();
	return (kb_IsDown(kb_KeyUp) == true);
}
bool pressed_Down() {
	kb_Scan();
	return (kb_IsDown(kb_KeyDown) == true);
}
bool pressed_Left() {
	kb_Scan();
	return (kb_IsDown(kb_KeyLeft) == true);
}
bool pressed_Right() {
	kb_Scan();
	return (kb_IsDown(kb_KeyRight) == true);
}
bool pressed_Enter() {
	kb_Scan();
	return (kb_IsDown(kb_KeyEnter) == true);
}
bool pressed_2nd() {
	kb_Scan();
	return (kb_IsDown(kb_Key2nd) == true);
}

void loadKeys(struct keys* keys) {
	if(keys->firstLoad) {
		keys->firstLoad = false;
		keys->alpha_prev = false;
		keys->second_prev = false;
		keys->enter_prev = false;
		keys->del_prev = false;
		
		return;
	}
	
	keys->alpha_prev = keys->alpha;
	keys->second_prev = keys->second;
	keys->enter_prev = keys->enter;
	keys->del_prev = keys->del;
	
	kb_Scan();
	keys->alpha = kb_IsDown(kb_KeyAlpha);
	keys->second = kb_IsDown(kb_Key2nd);
	keys->enter = kb_IsDown(kb_KeyEnter);
	keys->del = kb_IsDown(kb_KeyDel);
	
	return;
}