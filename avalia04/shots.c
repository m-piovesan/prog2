#include "shots.h"

void shots_init() {
    for(int i = 0; i < SHOTS_N; i++)
        shots[i].used = false;
}

bool shots_add(bool ship, bool straight, int x, int y) {
    al_play_sample(
        sample_shot,
        0.2,
        0,
        ship ? 1.0 : between_f(1.5, 1.6),
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );

    for(int i = 0; i < SHOTS_N; i++) {
        if(shots[i].used)
            continue;

        shots[i].ship = ship;

        if(ship) {
            shots[i].x = x - (SHIP_SHOT_W / 2);
            shots[i].y = y;
        }
        else // alien
        {
            shots[i].x = x - (ALIEN_SHOT_W / 2);
            shots[i].y = y - (ALIEN_SHOT_H / 2);

            if(straight) {
                shots[i].dx = 0;
                shots[i].dy = 2;
            }
            else {
                shots[i].dx = between(-2, 2);
                shots[i].dy = between(1, 3);
            }

            // if the shot has no speed, don't bother
            if(!shots[i].dx && !shots[i].dy)
                return true;

            shots[i].frame = 0;
        }

        shots[i].frame = 0;
        shots[i].used = true; // pegar isso aqui pra dizer se o tiro ainda tá em movimento ou não
        
        return true;
    }
    return false;
}

void shots_update() {
    for(int i = 0; i < SHOTS_N; i++) {
        if(!shots[i].used)
            continue;

        if(shots[i].ship) {
            shots[i].y -= 5;

            if(shots[i].y < -SHIP_SHOT_H)
            {
                shots[i].used = false;
                continue;
            }
        }
        else // alien
        {
            shots[i].x += shots[i].dx;
            shots[i].y += shots[i].dy;

            if((shots[i].y < -ALIEN_SHOT_H) || (shots[i].y > BUFFER_H)) {
                shots[i].used = false;
                continue;
            } else if (shots[i].x <= -4) {
                shots[i].dx = -shots[i].dx;
                shots[i].x += shots[i].dx;
                continue;
            } else if (shots[i].x >= BUFFER_W) {
                shots[i].dx = -shots[i].dx;
                shots[i].x += shots[i].dx;
                continue;
            }
        }

        shots[i].frame++;
    }
}

bool shots_collide(bool ship, int x, int y, int w, int h) {
    for(int i = 0; i < SHOTS_N; i++)
    {
        if(!shots[i].used)
            continue;

        // don't collide with one's own shots
        if(shots[i].ship == ship)
            continue;

        int sw, sh;
        if(ship)
        {
            sw = ALIEN_SHOT_W;
            sh = ALIEN_SHOT_H;
        }
        else
        {
            sw = SHIP_SHOT_W;
            sh = SHIP_SHOT_H;
        }

        if(collide(x, y, x+w, y+h, shots[i].x, shots[i].y, shots[i].x+sw, shots[i].y+sh))
        {
            fx_add(true, shots[i].x + (sw / 2), shots[i].y + (sh / 2));
            shots[i].used = false;
            return true;
        }
    }

    return false;
}

void shots_draw() {
    for(int i = 0; i < SHOTS_N; i++)
    {
        if(!shots[i].used)
            continue;

        int frame_display = (shots[i].frame / 2) % 2;

        if(shots[i].ship)
            al_draw_bitmap(sprites.ship_shot[frame_display], shots[i].x, shots[i].y, 0);
        else // alien
        {
            ALLEGRO_COLOR tint =
                frame_display
                ? al_map_rgb_f(1, 1, 1)
                : al_map_rgb_f(0.5, 0.5, 0.5)
            ;
            al_draw_tinted_bitmap(sprites.alien_shot, tint, shots[i].x, shots[i].y, 0);
        }
    }
}