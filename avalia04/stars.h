#ifndef __STARS__
#define __STARS__

#include <allegro5/allegro_primitives.h>

#include "general.h"
#include "display.h"

typedef struct STAR {
    float y;
    float speed;
} STAR;

#define STARS_N ((BUFFER_W / 2) - 1)
STAR stars[STARS_N];

void stars_init();
void stars_update();
void stars_draw();

#endif