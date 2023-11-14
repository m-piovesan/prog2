#ifndef __KEYBOARD__
#define __KEYBOARD__

#include <allegro5/allegro.h>

#define KEY_SEEN     1
#define KEY_RELEASED 2

unsigned char key[ALLEGRO_KEY_MAX];

void keyboard_init();
void keyboard_update(ALLEGRO_EVENT* event);

#endif