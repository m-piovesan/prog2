#include "obstacles.h"

void obstacles_init() {
    for (int i = 0; i < OBSTACLES_N; i++) {
        obstacles[i].x = 75 + (i * 75);
        obstacles[i].y = 170;
        obstacles[i].life = 10;
        obstacles[i].used = true;
    }
}

void obstacles_update() {
    for (int i = 0; i < OBSTACLES_N; i++) {
        if (!obstacles[i].used)
            continue;

        if (obstacles[i].life <= 0) {
            obstacles[i].used = false;
            fx_add(false, obstacles[i].x+5, obstacles[i].y);
            fx_add(false, obstacles[i].x+30, obstacles[i].y+10);
            continue;
        }

        if (shots_collide(false, obstacles[i].x, obstacles[i].y, OBSTACLE_W, OBSTACLE_H) ||
            shots_collide(true, obstacles[i].x, obstacles[i].y, OBSTACLE_W, OBSTACLE_H)) {
            obstacles[i].life--;
        }
    }
}

void obstacles_draw() {
    for (int i = 0; i < OBSTACLES_N; i++) {
        if (!obstacles[i].used) continue;

        if(obstacles[i].life > 6)
            al_draw_bitmap(sprites3.obstacle[0], obstacles[i].x, obstacles[i].y, 0);
        else if ((obstacles[i].life > 3) && (obstacles[i].life <= 6))
            al_draw_bitmap(sprites3.obstacle[1], obstacles[i].x, obstacles[i].y, 0);
        else
            al_draw_bitmap(sprites3.obstacle[2], obstacles[i].x, obstacles[i].y, 0);
    }
}