#ifndef __HUD_H__
#define __HUD_H__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "general.h"
#include "ship.h"
#include "aliens.h"

ALLEGRO_FONT* font;
long score_display;

void hud_init();
void hud_deinit();
int hud_update();
void hud_draw();
void end_game_test();

#endif