#include "sprites.h"

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h) {
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    must_init(sprite, "sprite grab");
    return sprite;
}

ALLEGRO_BITMAP* sprite_grab2(int x, int y, int w, int h) {
    ALLEGRO_BITMAP* sprite2 = al_create_sub_bitmap(sprites2._sheet, x, y, w, h);
    must_init(sprite2, "sprite grab");
    return sprite2;
}

ALLEGRO_BITMAP *sprite_grab3(int x, int y, int w, int h) {
    ALLEGRO_BITMAP* sprite3 = al_create_sub_bitmap(sprites3._sheet, x, y, w, h);
    must_init(sprite3, "sprite grab");
    return sprite3;
}

void sprites_init() {
    sprites._sheet = al_load_bitmap("./media/sprites/spritesheet.png");
    sprites2._sheet = al_load_bitmap("./media/sprites/aliens.png");
    sprites3._sheet = al_load_bitmap("./media/sprites/obstacle.png");

    must_init(sprites._sheet, "spritesheet");
    must_init(sprites2._sheet, "spritesheet");
    must_init(sprites3._sheet, "spritesheet");

    sprites.ship = sprite_grab(0, 0, SHIP_W, SHIP_H);

    sprites.life = sprite_grab(0, 14, LIFE_W, LIFE_H);

    sprites.ship_shot[0] = sprite_grab(13, 0, SHIP_SHOT_W, SHIP_SHOT_H);
    sprites.ship_shot[1] = sprite_grab(16, 0, SHIP_SHOT_W, SHIP_SHOT_H);

    sprites.alien[0] = sprite_grab(19, 0, ALIEN_BUG_W, ALIEN_BUG_H);
    sprites.alien[1] = sprite_grab(19, 10, ALIEN_ARROW_W, ALIEN_ARROW_H);
    sprites.alien[2] = sprite_grab(0, 21, ALIEN_MOTHERSHIP_W, ALIEN_MOTHERSHIP_H);

    sprites2.alien[0] = sprite_grab2(0, 21, 14, 10);
    sprites2.alien[1] = sprite_grab2(0, 0, 14, 10);
    sprites2.alien[2] = sprite_grab2(18, 21, 14, 10);

    sprites.alien_shot = sprite_grab(13, 10, ALIEN_SHOT_W, ALIEN_SHOT_H);

    sprites.explosion[0] = sprite_grab(33, 10, 9, 9);
    sprites.explosion[1] = sprite_grab(43, 9, 11, 11);
    sprites.explosion[2] = sprite_grab(46, 21, 17, 18);
    sprites.explosion[3] = sprite_grab(46, 40, 17, 17);

    sprites.sparks[0] = sprite_grab(34, 0, 10, 8);
    sprites.sparks[1] = sprite_grab(45, 0, 7, 8);
    sprites.sparks[2] = sprite_grab(54, 0, 9, 8);

    sprites.powerup[0] = sprite_grab(0, 49, 9, 12);
    sprites.powerup[1] = sprite_grab(10, 49, 9, 12);
    sprites.powerup[2] = sprite_grab(20, 49, 9, 12);
    sprites.powerup[3] = sprite_grab(30, 49, 9, 12);

    sprites3.obstacle[0] = sprite_grab3(0, 22, OBSTACLE_W, OBSTACLE_H);
    sprites3.obstacle[1] = sprite_grab3(0, 13, OBSTACLE_W, OBSTACLE_H);
    sprites3.obstacle[2] = sprite_grab3(0, 2, OBSTACLE_W, OBSTACLE_H);
}

void sprites_deinit() {
    al_destroy_bitmap(sprites.ship);

    for (int i = 0; i < 3; i++) {
        al_destroy_bitmap(sprites.alien[i]);
        al_destroy_bitmap(sprites2.alien[i]);
        al_destroy_bitmap(sprites.sparks[i]);
        al_destroy_bitmap(sprites3.obstacle[i]);
    }

    al_destroy_bitmap(sprites.ship_shot[0]);
    al_destroy_bitmap(sprites.ship_shot[1]);

    al_destroy_bitmap(sprites.explosion[0]);
    al_destroy_bitmap(sprites.explosion[1]);
    al_destroy_bitmap(sprites.explosion[2]);
    al_destroy_bitmap(sprites.explosion[3]);

    al_destroy_bitmap(sprites.powerup[0]);
    al_destroy_bitmap(sprites.powerup[1]);
    al_destroy_bitmap(sprites.powerup[2]);
    al_destroy_bitmap(sprites.powerup[3]);

    al_destroy_bitmap(sprites._sheet);
}