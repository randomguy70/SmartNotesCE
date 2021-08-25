#ifndef KEY_H
#define KEY_H

#ifdef __cplusplus
extern "C" {
#endif

uint8_t get_single_key_pressed(void);

bool pressed_Up();
bool pressed_Down();
bool pressed_Left();
bool pressed_Right();
bool pressed_Enter();
bool pressed_2nd();

#ifdef __cplusplus
}
#endif

#endif