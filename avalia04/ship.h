#ifndef __SHIP__
#define __SHIP__

#include <allegro5/allegro.h>
#include <stdbool.h>

#include "display.h"
#include "sprites.h"
#include "fx.h"
#include "shots.h"
#include "keyboard.h"

#define SHIP_SPEED 3
#define SHIP_MAX_X (BUFFER_W - SHIP_W)
#define SHIP_MAX_Y (BUFFER_H - SHIP_H)

typedef struct SHIP {
    int x, y;
    int x0, y0;
    int shot_timer;
    int lives;
    int respawn_timer;
    int invincible_timer;
} SHIP;
SHIP ship;

void ship_init();

void ship_update();

void ship_draw();

#endif