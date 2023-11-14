#include "aliens.h"

void aliens_init() {
    for(int i = 0; i < ALIENS_ROWS; i++) {
        for(int j = i*ALIENS_COLS; j < ALIENS_COLS*(i+1); j++) {
            aliens[j].x = 40 + ((j - (i*ALIENS_COLS)) * 20);
            aliens[j].y = 40 + (i * 20);
            
            if(i == 1 || i == 2) aliens[j].type = ALIEN_TYPE_BUG;
            else if (i == 3 || i == 4) aliens[j].type = ALIEN_TYPE_ARROW;
            else aliens[j].type = ALIEN_TYPE_BOW;
            
            aliens[j].shot_timer = 0;
            aliens[j].blink = 0;
            aliens[j].life = 1;
            aliens[j].used = true;
            aliens[j].movingRight = true;

            aliens[j].x0 = aliens[j].x;
            aliens[j].y0 = aliens[j].y;
        }
    }   
}

void mothership_init() {
    mothership.x = BUFFER_W + 30;
    mothership.y = 20;
    mothership.type = ALIEN_TYPE_MOTHERSHIP;
    mothership.shot_timer = 0;
    mothership.blink = 0;
    mothership.life = 1;
    mothership.used = false;
    mothership.movingRight = true;
}

void aliens_update() {
    int new_quota =
        (frames % 500)
        ? 0
        : 1
    ;
    int new_x = between(10, BUFFER_W-50);

    for(int i = 0; i < ALIENS_N; i++) {
        if(!aliens[i].used) {
            if(new_quota > 0) {
                for (int i = 0; i < 2; i++) {
                    int x = between(0, ALIENS_N);

                    while((!aliens[x].used) && (aliens[x].shot_timer > 0))
                        x = between(0, ALIENS_N);

                    int cx = aliens[x].x + (ALIEN_W[aliens[x].type] / 2);
                    int cy = aliens[x].y + (ALIEN_H[aliens[x].type] / 2);

                    if ((aliens[x].used) && (aliens[x].shot_timer <= 0)) {
                        switch (aliens[x].type) {
                            case ALIEN_TYPE_BUG:
                                shots_add(false, false, cx, cy);
                                aliens[x].shot_timer = 200;
                                break;
                            case ALIEN_TYPE_ARROW:
                                shots_add(false, true, cx, aliens[x].y);
                                aliens[x].shot_timer = 200;
                                break;
                            case ALIEN_TYPE_BOW:
                                shots_add(false, true, cx-5, cy);
                                shots_add(false, true, cx+5, cy);
                                shots_add(false, true, cx-5, cy + 8);
                                shots_add(false, true, cx+5, cy + 8);
                                aliens[x].shot_timer = 200;
                                break; 
                        }
                    }
                }

                new_quota--;
            }
            continue;
        }

        // movement
        if(!(frames % 5)) {            
            if (aliens[i].movingRight) {
                aliens[i].x++;

                if (aliens[i].x >= aliens[i].x0 + 20) {
                    aliens[i].movingRight = false;
                    aliens[i].y = aliens[i].y + 1.5;
                }
            } else {
                aliens[i].x--;
                
                if (aliens[i].x <= aliens[i].x0 - 10) {
                    aliens[i].movingRight = true;
                    aliens[i].y = aliens[i].y + 1.5;
                }
            }
        }

        if(aliens[i].blink)
            aliens[i].blink--;

        // essa parte é quando acerta um alien
        if(shots_collide(false, aliens[i].x, aliens[i].y, ALIEN_W[aliens[i].type], ALIEN_H[aliens[i].type])) {
            aliens[i].life--;
            aliens[i].blink = 4;
        }

        int cx = aliens[i].x + (ALIEN_W[aliens[i].type] / 2);
        int cy = aliens[i].y + (ALIEN_H[aliens[i].type] / 2);

        if(aliens[i].life <= 0) {
            fx_add(false, cx, cy);

            switch(aliens[i].type) {
                case ALIEN_TYPE_BUG:
                    score += 20;
                    break;

                case ALIEN_TYPE_ARROW:
                    score += 10;
                    break;

                case ALIEN_TYPE_BOW:
                    score += 30;
                    fx_add(false, cx-10, cy-4);
                    break;
            }

            aliens[i].used = false;
            continue;
        } 

        aliens[i].shot_timer--;
        
        if(aliens[i].shot_timer == 0) {
            switch(aliens[i].type) {
                case ALIEN_TYPE_BUG:
                    shots_add(false, false, cx, cy);
                    aliens[i].shot_timer = 150;
                    break;
                case ALIEN_TYPE_ARROW:
                    shots_add(false, true, cx, aliens[i].y);
                    aliens[i].shot_timer = 80;
                    break;
                case ALIEN_TYPE_BOW:
                    shots_add(false, true, cx-5, cy);
                    shots_add(false, true, cx+5, cy);
                    shots_add(false, true, cx-5, cy + 8);
                    shots_add(false, true, cx+5, cy + 8);
                    aliens[i].shot_timer = 200;
                    break;
            }
        }

        // mothership
        if (mothership.life > 0) {
            if(!(frames % 5)) mothership.x = mothership.x - 0.04;

            if(shots_collide(false, mothership.x, mothership.y, ALIEN_MOTHERSHIP_W, ALIEN_MOTHERSHIP_H)) {
                mothership.life--;
                mothership.blink = 4;
            }

            if (mothership.life <= 0) {
                int cx = mothership.x + (ALIEN_MOTHERSHIP_W);
                int cy = mothership.y + (ALIEN_MOTHERSHIP_H);

                score += 100;

                fx_add(false, cx-10, cy-4);
                fx_add(false, cx+4, cy+10);
                fx_add(false, cx+8, cy+8);

                mothership.used = true;
            }
        }
    }
}

void aliens_draw() {
    for(int i = 0; i < ALIENS_N; i++) {
        if(!aliens[i].used)
            continue;
        if(aliens[i].blink > 2)
            continue;

        // aqui pra escolher os sprites que vão ser usados pra cada tipo de alien
        al_draw_bitmap(sprites2.alien[aliens[i].type], aliens[i].x, aliens[i].y, 0);
    }
}

void mothership_draw() {
    if(!mothership.used) al_draw_bitmap(sprites.alien[2], mothership.x, mothership.y, 0);
}

int aliens_life_tester() {
    int testaAliens = 0;

    for(int i = 0; i < ALIENS_N; i++) {
        if(!aliens[i].used)
            testaAliens++;
    }

    if (testaAliens == ALIENS_N)
        return 1;

    return 0;
}