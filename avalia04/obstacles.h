#ifndef __OBSTACLES__
#define __OBSTACLES__

#include <allegro5/allegro.h>
#include <stdbool.h>

#include "fx.h"
#include "shots.h"

typedef struct OBSTACLE {
    int x, y;
    int life;
    bool used;
} OBSTACLE;

#define OBSTACLES_N 3
OBSTACLE obstacles[OBSTACLES_N];

void obstacles_init();

void obstacles_update();

void obstacles_draw();

#endif