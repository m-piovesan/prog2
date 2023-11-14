#ifndef __ALIENS__
#define __ALIENS__

#include <allegro5/allegro.h>
#include <stdbool.h>

#include "fx.h"
#include "shots.h"
#include "general.h"

typedef enum ALIEN_TYPE {
    ALIEN_TYPE_BUG = 0,
    ALIEN_TYPE_ARROW,
    ALIEN_TYPE_BOW,
    ALIEN_TYPE_MOTHERSHIP,
} ALIEN_TYPE;

typedef struct ALIEN {
    float x, y;
    int x0, y0;
    ALIEN_TYPE type;
    int shot_timer;
    int blink;
    int life;
    bool used;
    bool movingRight;
} ALIEN;

#define ALIENS_N 12*5
#define ALIENS_COLS 12
#define ALIENS_ROWS 5

ALIEN aliens[ALIENS_N];
ALIEN mothership;

void aliens_init();
void mothership_init();
void aliens_update();
void aliens_draw();
void mothership_draw();
int aliens_life_tester();

#endif