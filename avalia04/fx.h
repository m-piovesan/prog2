#ifndef __FX__
#define __FX__

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

#include "audio.h"
#include "sprites.h"

typedef struct FX {
    int x, y;
    int frame;
    bool spark;
    bool used;
} FX;

#define FX_N 128
FX fx[FX_N];

void fx_init();
void fx_add(bool spark, int x, int y);
void fx_update();

#endif