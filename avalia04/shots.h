#ifndef __SHOTS__
#define __SHOTS__

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <stdbool.h>

#include "general.h"
#include "sprites.h"
#include "audio.h"
#include "display.h"
#include "fx.h"

typedef struct SHOT {
    int x, y, dx, dy;
    int frame;
    bool ship;
    bool used;
} SHOT;

#define SHOTS_N 128
SHOT shots[SHOTS_N];

void shots_init();
bool shots_add(bool ship, bool straight, int x, int y);
void shots_update();
bool shots_collide(bool ship, int x, int y, int w, int h);
void shots_draw();

#endif