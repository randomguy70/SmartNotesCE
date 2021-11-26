#ifndef KEY_H
#define KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

uint8_t get_single_key_pressed(void);

bool pressed_Up();
bool pressed_Down();
bool pressed_Left();
bool pressed_Right();
bool pressed_Enter();
bool pressed_2nd();

// contains press info about important keys
struct keys {
	bool firstLoad;
	bool alpha, alpha_prev;
	bool second, second_prev;
	bool enter, enter_prev;
	bool del, del_prev;
};

void loadKeys(struct keys* keys);

#ifdef __cplusplus
}
#endif

#endif