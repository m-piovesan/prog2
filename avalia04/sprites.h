#ifndef __SPRITES__
#define __SPRITES__

#include <allegro5/allegro.h>

#define SHIP_W 12
#define SHIP_H 13

#define SHIP_SHOT_W 2
#define SHIP_SHOT_H 9

#define LIFE_W 6
#define LIFE_H 6

int ALIEN_W[] = {14, 13, 45};
int ALIEN_H[] = { 9, 10, 27};

#define ALIEN_BUG_W      ALIEN_W[0]
#define ALIEN_BUG_H      ALIEN_H[0]
#define ALIEN_ARROW_W    ALIEN_W[1]
#define ALIEN_ARROW_H    ALIEN_H[1]
#define ALIEN_MOTHERSHIP_W ALIEN_W[2]
#define ALIEN_MOTHERSHIP_H ALIEN_H[2]

#define ALIEN_SHOT_W 4
#define ALIEN_SHOT_H 4

#define EXPLOSION_FRAMES 4
#define SPARKS_FRAMES    3

#define OBSTACLE_W 32
#define OBSTACLE_H 10

typedef struct SPRITES {
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* ship;
    ALLEGRO_BITMAP* ship_shot[2];
    ALLEGRO_BITMAP* life;

    ALLEGRO_BITMAP* alien[3];
    ALLEGRO_BITMAP* alien_shot;

    ALLEGRO_BITMAP* explosion[EXPLOSION_FRAMES];
    ALLEGRO_BITMAP* sparks[SPARKS_FRAMES];

    ALLEGRO_BITMAP* powerup[4];
    
    ALLEGRO_BITMAP* obstacle[3];
} SPRITES;

SPRITES sprites;
SPRITES sprites2;
SPRITES sprites3;

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h);
ALLEGRO_BITMAP* sprite_grab2(int x, int y, int w, int h);
ALLEGRO_BITMAP *sprite_grab3(int x, int y, int w, int h);
void sprites_init();
void sprites_deinit();

#endif