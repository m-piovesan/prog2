/* 
 * gcc game.c -o game $(pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags)
 * ./game
 */

#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

// GENERAL --------------------------------------------

long frames;
long score;

void must_init(bool test, const char *description) {
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int between(int lo, int hi) {
    return lo + (rand() % (hi - lo));
}

float between_f(float lo, float hi) {
    return lo + ((float)rand() / (float)RAND_MAX) * (hi - lo);
}

bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
    if(ax1 > bx2) return false;
    if(ax2 < bx1) return false;
    if(ay1 > by2) return false;
    if(ay2 < by1) return false;

    return true;
}


// DISPLAY ------------------------------------------------------

#define BUFFER_W 320
#define BUFFER_H 240

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;

void disp_init() {
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST); // anti-aliasing
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST); 

    disp = al_create_display(DISP_W, DISP_H);
    must_init(disp, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(buffer, "bitmap buffer");
}

void disp_deinit() {
    al_destroy_bitmap(buffer);
    al_destroy_display(disp);
}

// draw to our buffer rather than to the screen
void disp_pre_draw() {
    al_set_target_bitmap(buffer);
}

// draw the buffer to the screen
void disp_post_draw() {
    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

    al_flip_display();
}


// KEYBOARD ------------------------------------------------------

#define KEY_SEEN 1
#define KEY_RELEASED 2
unsigned char key[ALLEGRO_KEY_MAX];

void keyboard_init() {
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT* event) {
    switch(event->type) {
        case ALLEGRO_EVENT_TIMER:
            for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event->keyboard.keycode] &= KEY_RELEASED;
            break;
    }
}


// SPRITES ------------------------------------------------------

#define SHIP_W 12
#define SHIP_H 13

#define SHIP_SHOT_W 2
#define SHIP_SHOT_H 9

#define LIFE_W 6
#define LIFE_H 6

const int ALIEN_W[] = {14, 13, 45};
const int ALIEN_H[] = {9, 10, 27};

#define ALIEN_BUG_W ALIEN_W[0]
#define ALIEN_BUG_H ALIEN_H[0]
#define ALIEN_ARROW_W ALIEN_W[1]
#define ALIEN_ARROW_H ALIEN_H[1]
#define ALIEN_MOTHERSHIP_W ALIEN_W[2]
#define ALIEN_MOTHERSHIP_H ALIEN_H[2]

#define ALIEN_SHOT_W 4
#define ALIEN_SHOT_H 4

#define EXPLOSION_FRAMES 4
#define SPARKS_FRAMES 3

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
SPRITES sprites21;
SPRITES sprites3;

// ship and effects
ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h) {
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    must_init(sprite, "sprite grab");
    return sprite;
}

// aliens
ALLEGRO_BITMAP* sprite_grab2(int x, int y, int w, int h) {
    ALLEGRO_BITMAP* sprite2 = al_create_sub_bitmap(sprites2._sheet, x, y, w, h);
    must_init(sprite2, "sprite grab");
    return sprite2;
}

//aliens moving
ALLEGRO_BITMAP* sprite_grab21(int x, int y, int w, int h) {
    ALLEGRO_BITMAP* sprite21 = al_create_sub_bitmap(sprites21._sheet, x, y, w, h);
    must_init(sprite21, "sprite grab");
    return sprite21;
}

// obstacles
ALLEGRO_BITMAP *sprite_grab3(int x, int y, int w, int h) {
    ALLEGRO_BITMAP* sprite3 = al_create_sub_bitmap(sprites3._sheet, x, y, w, h);
    must_init(sprite3, "sprite grab");
    return sprite3;
}

void sprites_init() {
    sprites._sheet = al_load_bitmap("./media/sprites/spritesheet.png");
    sprites2._sheet = al_load_bitmap("./media/sprites/aliens.png");
    sprites21._sheet = al_load_bitmap("./media/sprites/aliensmove.png");
    sprites3._sheet = al_load_bitmap("./media/sprites/obstacle.png");

    must_init(sprites._sheet, "spritesheet");
    must_init(sprites2._sheet, "spritesheet");
    must_init(sprites21._sheet, "spritesheet");
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

    sprites21.alien[0] = sprite_grab21(0, 20, 15, 11);
    sprites21.alien[1] = sprite_grab21(0, 0, 14, 10);
    sprites21.alien[2] = sprite_grab21(18, 21, 14, 10);

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
        al_destroy_bitmap(sprites21.alien[i]);
        al_destroy_bitmap(sprites.sparks[i]);
        al_destroy_bitmap(sprites3.obstacle[i]);
    }

    al_destroy_bitmap(sprites.ship_shot[0]);
    al_destroy_bitmap(sprites.ship_shot[1]);

    for (int i = 0; i < 4; i++) {
        al_destroy_bitmap(sprites.powerup[i]);
        al_destroy_bitmap(sprites.explosion[i]);
    }

    al_destroy_bitmap(sprites._sheet);
}


// AUDIO ------------------------------------------------------

ALLEGRO_SAMPLE* sample_shot;
ALLEGRO_SAMPLE* sample_explode[2];

void audio_init() {
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(128);

    sample_shot = al_load_sample("./media/sound/shot.flac");
    must_init(sample_shot, "shot sample");

    sample_explode[0] = al_load_sample("./media/sound/explode1.flac");
    must_init(sample_explode[0], "explode[0] sample");
    
    sample_explode[1] = al_load_sample("./media/sound/explode2.flac");
    must_init(sample_explode[1], "explode[1] sample");
}

void audio_deinit() {
    al_destroy_sample(sample_shot);
    al_destroy_sample(sample_explode[0]);
    al_destroy_sample(sample_explode[1]);
}

// FX ------------------------------------------------------

typedef struct FX {
    int x, y;
    int frame;
    bool spark;
    bool used;
} FX;

#define FX_N 128
FX fx[FX_N];

void fx_init() {
    for(int i = 0; i < FX_N; i++)
        fx[i].used = false;
}

void fx_add(bool spark, int x, int y) {
    if(!spark)
        al_play_sample(sample_explode[between(0, 2)], 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

    for(int i = 0; i < FX_N; i++) {
        if(fx[i].used)
            continue;

        fx[i].x = x;
        fx[i].y = y;
        fx[i].frame = 0;
        fx[i].spark = spark;
        fx[i].used = true;
        return;
    }
}

void fx_update() {
    for(int i = 0; i < FX_N; i++) {
        if(!fx[i].used)
            continue;

        fx[i].frame++;

        if((!fx[i].spark && (fx[i].frame == (EXPLOSION_FRAMES * 2)))
        || ( fx[i].spark && (fx[i].frame == (SPARKS_FRAMES * 2)))
        )
            fx[i].used = false;
    }
}

void fx_draw() {
    for(int i = 0; i < FX_N; i++) {
        if(!fx[i].used)
            continue;

        int frame_display = fx[i].frame / 2;

        // If spark, it uses a sprite from sprites.sparks
        // otherwise, it uses a sprite from sprites.explosion
        ALLEGRO_BITMAP* bmp =
            fx[i].spark
            ? sprites.sparks[frame_display]
            : sprites.explosion[frame_display]
        ;

        int x = fx[i].x - (al_get_bitmap_width(bmp) / 2);
        int y = fx[i].y - (al_get_bitmap_height(bmp) / 2);

        al_draw_bitmap(bmp, x, y, 0);
    }
}


// SHOTS ------------------------------------------------------

typedef struct SHOT {
    int x, y, dx, dy;
    int frame;
    bool ship;
    bool used;
} SHOT;

#define SHOTS_N 128
SHOT shots[SHOTS_N];

void shots_init(SHOT shot_list[SHOTS_N]) {
    for(int i = 0; i < SHOTS_N; i++)
        shots[i].used = false;
}

bool shots_add(SHOT shot_list[SHOTS_N], bool ship, bool straight, int x, int y) {
    // sound changes depending on who shot
    al_play_sample(
        sample_shot,
        0.2,
        0,
        ship ? 1.0 : between_f(1.5, 1.6),
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );

    for(int i = 0; i < SHOTS_N; i++) {
        if(shot_list[i].used)
            continue;

        shot_list[i].ship = ship;

        if(ship) {
            shot_list[i].x = x - (SHIP_SHOT_W / 2);
            shot_list[i].y = y;
        } else { // alien
            shot_list[i].x = x - (ALIEN_SHOT_W / 2);
            shot_list[i].y = y - (ALIEN_SHOT_H / 2);

            if(straight) {
                shot_list[i].dx = 0;
                shot_list[i].dy = 2;
            } else {
                shot_list[i].dx = between(-2, 2);
                shot_list[i].dy = between(1, 3);
            }

            // if the shot has no speed, it's not used
            if(!shot_list[i].dx && !shot_list[i].dy)
                return true;

            shot_list[i].frame = 0;
        }

        shot_list[i].frame = 0;
        shot_list[i].used = true;
        
        return true;
    }

    return false;
}

void shots_update(SHOT shot_list[SHOTS_N]) {
    for(int i = 0; i < SHOTS_N; i++) {
        if(!shot_list[i].used)
            continue;

        if(shot_list[i].ship) {
            shot_list[i].y -= 5; // ship shot speed

            if(shot_list[i].y < -SHIP_SHOT_H) { // out of screen
                shot_list[i].used = false;
                continue;
            }
        } else { // alien
            shot_list[i].x += shot_list[i].dx;
            shot_list[i].y += shot_list[i].dy;

            if((shot_list[i].y < -ALIEN_SHOT_H) || (shot_list[i].y > BUFFER_H)) {
                shot_list[i].used = false;
                continue;
            } else if (shot_list[i].x <= -4) { // bounce on the edges
                shot_list[i].dx = -shot_list[i].dx;
                shot_list[i].x += shot_list[i].dx;
                continue;
            } else if (shot_list[i].x >= BUFFER_W) {
                shot_list[i].dx = -shot_list[i].dx;
                shot_list[i].x += shot_list[i].dx;
                continue;
            }
        }

        shot_list[i].frame++;
    }
}

bool shots_collide(SHOT shot_list[SHOTS_N], bool ship, int x, int y, int w, int h) {
    for(int i = 0; i < SHOTS_N; i++) {
        if(!shot_list[i].used)
            continue;

        int sw, sh;

        if(ship) {
            sw = ALIEN_SHOT_W;
            sh = ALIEN_SHOT_H;
        } else {
            sw = SHIP_SHOT_W;
            sh = SHIP_SHOT_H;
        }

        if(collide(x, y, x+w, y+h, shot_list[i].x, shot_list[i].y, shot_list[i].x+sw, shot_list[i].y+sh)) {
            fx_add(true, shot_list[i].x + (sw / 2), shot_list[i].y + (sh / 2));
            shot_list[i].used = false;
            
            return true;
        }
    }

    return false;
}

// for player's shots, two versions of shot
// for aliens, there's only one
void shots_draw(SHOT shot_list[SHOTS_N]) {
    for(int i = 0; i < SHOTS_N; i++) {
        if(!shot_list[i].used)
            continue;

        int frame_display = (shot_list[i].frame / 2) % 2;

        if(shot_list[i].ship)
            al_draw_bitmap(sprites.ship_shot[frame_display], shot_list[i].x, shot_list[i].y, 0);
        else { // alien
            ALLEGRO_COLOR tint =
                frame_display
                ? al_map_rgb_f(1, 1, 1)
                : al_map_rgb_f(0.5, 0.5, 0.5)
            ;

            al_draw_tinted_bitmap(sprites.alien_shot, tint, shot_list[i].x, shot_list[i].y, 0);
        }
    }
}


// SHIP ------------------------------------------------------

#define SHIP_SPEED 3
#define SHIP_MAX_X (BUFFER_W - SHIP_W)
#define SHIP_MAX_Y (BUFFER_H - SHIP_H)

typedef struct SHIP {
    int x, y;
    int x0, y0;
    int shot_timer;
    int lives;
    int respawn_timer;
    int invincible_timer;
    SHOT ship_shots[SHOTS_N];
} SHIP;

SHIP ship;

void ship_init() {
    ship.x = (BUFFER_W / 2) - (SHIP_W / 2);
    ship.y = (BUFFER_H - 20) - (SHIP_H / 2);
    ship.shot_timer = 0;
    ship.lives = 3;
    ship.respawn_timer = 0;
    ship.invincible_timer = 120;

    shots_init(ship.ship_shots);

    ship.x0 = ship.x;
    ship.y0 = ship.y;
}

void ship_update() {
    if(ship.lives < 0)
        return;

    if(ship.respawn_timer) {
        ship.respawn_timer--;
        return;
    }

    if(key[ALLEGRO_KEY_LEFT]) {
        ship.x0 = ship.x;
        ship.x -= SHIP_SPEED;
    }

    if(key[ALLEGRO_KEY_RIGHT]) {
        ship.x0 = ship.x;
        ship.x += SHIP_SPEED;
    }

    if(ship.x < 0)
        ship.x = 0;
    if(ship.y < 0)
        ship.y = 0;

    if(ship.x > SHIP_MAX_X)
        ship.x = SHIP_MAX_X;
    if(ship.y > SHIP_MAX_Y)
        ship.y = SHIP_MAX_Y;

    if(ship.invincible_timer)
        ship.invincible_timer--;
    else {
        if(shots_collide(shots, true, ship.x, ship.y, SHIP_W, SHIP_H)) {
            int x = ship.x + (SHIP_W / 2);
            int y = ship.y + (SHIP_H / 2);
            fx_add(false, x, y);
            fx_add(false, x+4, y+2);
            fx_add(false, x-2, y-4);
            fx_add(false, x+1, y-5);

            ship.lives--;
            ship.respawn_timer = 90;
            ship.invincible_timer = 180;
        }
    }

    if(ship.shot_timer)
        ship.shot_timer--;
    else if(key[ALLEGRO_KEY_X]) {
        int x = ship.x + (SHIP_W / 2);

        if(shots_add(ship.ship_shots, true, false, x, ship.y)) {
            ship.shot_timer = 50;
        } 
    }
}

void ship_draw() {
    if(ship.lives <= 0)
        return;
    if(ship.respawn_timer)
        return;
    if(((ship.invincible_timer / 2) % 3) == 1)
        return;

    al_draw_bitmap(sprites.ship, ship.x, ship.y, 0);
}

// OBSTACLES ------------------------------------------------------
typedef struct OBSTACLE {
    int x, y;
    int life;
    bool used;
} OBSTACLE;

#define OBSTACLES_N 3
OBSTACLE obstacles[OBSTACLES_N];

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

        if (shots_collide(shots, true, obstacles[i].x, obstacles[i].y, OBSTACLE_W, OBSTACLE_H) ||
            shots_collide(ship.ship_shots, false, obstacles[i].x, obstacles[i].y, OBSTACLE_W, OBSTACLE_H)) {
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

// ALIENS ------------------------------------------------------

typedef enum ALIEN_TYPE {
    ALIEN_TYPE_BUG,
    ALIEN_TYPE_ARROW,
    ALIEN_TYPE_BOW,
    ALIEN_TYPE_MOTHERSHIP,
} ALIEN_TYPE;

typedef struct ALIEN {
    float x, y;
    int x0, y0;
    ALIEN_TYPE type;
    int shot_timer;
    int life;
    int changeStyle;
    bool used;
    bool movingRight;
} ALIEN;

#define ALIENS_N 12*5
#define ALIENS_COLS 12
#define ALIENS_ROWS 5

ALIEN aliens[ALIENS_N];
ALIEN mothership;

void aliens_init() {
    for(int i = 0; i < ALIENS_ROWS; i++) {
        for(int j = i*ALIENS_COLS; j < ALIENS_COLS*(i+1); j++) {
            aliens[j].x = 40 + ((j - (i*ALIENS_COLS)) * 20);
            aliens[j].y = 40 + (i * 20);
            
            if(i == 1 || i == 2) aliens[j].type = ALIEN_TYPE_BUG;
            else if (i == 3 || i == 4) aliens[j].type = ALIEN_TYPE_ARROW;
            else aliens[j].type = ALIEN_TYPE_BOW;
            
            aliens[j].shot_timer = 0;
            aliens[j].life = 1;
            aliens[j].changeStyle = 100;
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
    mothership.life = 1;
    mothership.used = false;
    mothership.movingRight = true;
}

void aliens_update(int level) {
    int difficulty = 0;

    if (level == 1)
        difficulty = 100;
    else if (level == 2)
        difficulty = 95;
    else if (level == 3)
        difficulty = 90;
    else if (level == 4)
        difficulty = 85;
    else 
        difficulty = 80;

    int counter =
        (frames % difficulty)
        ? 0
        : 1
    ;

    for(int i = 0; i < ALIENS_N; i++) {
        // aliens' shot
        if(!aliens[i].used) {
            if(counter > 0) {
                for (int i = 0; i < 2; i++) {
                    int x = between(0, ALIENS_N);

                    while((aliens[x].used) && (aliens[x].shot_timer > 0))
                        x = between(0, ALIENS_N);

                    int cx = aliens[x].x + (ALIEN_W[aliens[x].type] / 2);
                    int cy = aliens[x].y + (ALIEN_H[aliens[x].type] / 2);

                    if ((aliens[x].used) && (aliens[x].shot_timer <= 0)) {
                        switch (aliens[x].type) {
                            case ALIEN_TYPE_BUG:
                                shots_add(shots, false, false, cx, cy);
                                aliens[x].shot_timer = 200;
                                break;
                            case ALIEN_TYPE_ARROW:
                                shots_add(shots, false, true, cx, aliens[x].y);
                                aliens[x].shot_timer = 200;
                                break;
                            case ALIEN_TYPE_BOW:
                                shots_add(shots, false, true, cx-5, cy);
                                shots_add(shots, false, true, cx+5, cy);
                                shots_add(shots, false, true, cx-5, cy + 8);
                                shots_add(shots, false, true, cx+5, cy + 8);
                                aliens[x].shot_timer = 200;
                                break; 
                        }
                    }
                }

                counter--;
            }
            continue;
        }

        aliens[i].shot_timer--;

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

        if(shots_collide(ship.ship_shots, true, aliens[i].x, aliens[i].y, ALIEN_W[aliens[i].type], ALIEN_H[aliens[i].type]))
            aliens[i].life--;
        
        // variables set to draw the explosions
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

        // change style
        if(aliens[i].changeStyle > 0)
            aliens[i].changeStyle--;
        else
            aliens[i].changeStyle = 100;

        // mothership
        if (mothership.life > 0) {
            if(!(frames % 5)) mothership.x = mothership.x - 0.04;

            if(shots_collide(ship.ship_shots, false, mothership.x, mothership.y, ALIEN_MOTHERSHIP_W, ALIEN_MOTHERSHIP_H))
                mothership.life--;

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

        if (aliens[i].changeStyle > 50)
            al_draw_bitmap(sprites2.alien[aliens[i].type], aliens[i].x, aliens[i].y, 0);
        else
            al_draw_bitmap(sprites21.alien[aliens[i].type], aliens[i].x, aliens[i].y, 0);
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

int aliens_position_tester() {
    for(int i = 0; i < ALIENS_N; i++) {
        if(aliens[i].used) {
            if (aliens[i].y >= BUFFER_H - 70)
                return 1;
        }
    }

    return 0;
}

// STARS ------------------------------------------------------

typedef struct STAR {
    float y;
    float speed;
} STAR;

#define STARS_N ((BUFFER_W / 2) - 1)
STAR stars[STARS_N];

void stars_init() {
    for(int i = 0; i < STARS_N; i++) {
        stars[i].y = between_f(0, BUFFER_H);
        stars[i].speed = between_f(0.1, 1);
    }
}

void stars_update() {
    for(int i = 0; i < STARS_N; i++) {
        stars[i].y += stars[i].speed;
        if(stars[i].y >= BUFFER_H) {
            stars[i].y = 0;
            stars[i].speed = between_f(0.1, 1);
        }
    }
}

void stars_draw() {
    float star_x = 1.5;
    for(int i = 0; i < STARS_N; i++) {
        float l = stars[i].speed * 0.8;
        al_draw_pixel(star_x, stars[i].y, al_map_rgb_f(l,l,l));
        star_x += 2;
    }
}

// HUD ------------------------------------------------------

ALLEGRO_FONT* font;
long score_display;

void hud_init() {
    font = al_create_builtin_font();
    must_init(font, "font");

    score_display = 0;
}

void hud_deinit() {
    al_destroy_font(font);
}

void hud_update() {
    if(frames % 2)
        return;

    for(long i = 5; i > 0; i--) {
        long diff = 1 << i;

        if(score_display <= (score - diff))
            score_display += diff;
    }
}

void hud_draw() {
    int spacing = LIFE_W + 1;

    al_draw_textf(
        font,
        al_map_rgb_f(1,1,1),
        1, 1,
        0,
        "%06ld",
        score_display
    );

    for(int i = 0; i < ship.lives; i++)
        al_draw_bitmap(sprites.life, 1 + (i * spacing), 10, 0);
}

// TESTERS ------------------------------------------------------

int new_round_test() {
    int testaAliens = 0;

    for(int i = 0; i < ALIENS_N; i++) {
        if(!aliens[i].used)
            testaAliens++;
    }

    if (testaAliens == ALIENS_N) {
        aliens_init();
        
        if (ship.lives < 5) ship.lives++;

        return 1;
    }

    return 0;
}

int game_over_test() {
    if (ship.lives <= 0)
        return 1;

    return 0;
}

// INIT & DEINIT ------------------------------------------------------

void init_all() {
    keyboard_init();
    fx_init();
    shots_init(shots);
    ship_init();
    aliens_init();
    stars_init();
    obstacles_init();
}

void deinit_all () {
    sprites_deinit();
    hud_deinit();
    audio_deinit();
    disp_deinit();
}

// MENUS ------------------------------------------------------

void start_menu() {
    disp_pre_draw();
    al_clear_to_color(al_map_rgb(0,0,0));
    
    al_draw_text(
        font,
        al_map_rgb_f(1,1,1),
        0, 10,
        ALLEGRO_ALIGN_LEFT,
        "############################################"
    );

    al_draw_text(
        font,
        al_map_rgb_f(1,1,1),
        0, BUFFER_H-25,
        ALLEGRO_ALIGN_LEFT,
        "############################################"
    );

    al_draw_text(
        font,
        al_map_rgb_f(0,1,0),
        BUFFER_W / 2, BUFFER_H / 3,
        ALLEGRO_ALIGN_CENTER,
        "WELCOME TO SPACE INVADERS!"
    );
    
    al_draw_text(
        font,
        al_map_rgb_f(1,1,1),
        BUFFER_W / 2, BUFFER_H / 2,
        ALLEGRO_ALIGN_CENTER,
        "Press 'P' to play or 'ESC' to quit"
    );

    al_draw_text(
        font,
        al_map_rgb_f(1,1,1),
        BUFFER_W / 2, (BUFFER_H / 2) + 20,
        ALLEGRO_ALIGN_CENTER,
        "Press 'T' to open tutorial"
    );

    disp_post_draw();
}

void tutorial() {
    disp_pre_draw();
    al_clear_to_color(al_map_rgb(0,0,0));
    
    al_draw_text(
        font,
        al_map_rgb_f(1,1,1),
        0, 10,
        ALLEGRO_ALIGN_LEFT,
        "############################################"
    );

    al_draw_text(
        font,
        al_map_rgb_f(1,1,1),
        0, BUFFER_H-25,
        ALLEGRO_ALIGN_LEFT,
        "############################################"
    );

    al_draw_text(
        font,
        al_map_rgb_f(0,1,0),
        BUFFER_W / 2, 50,
        ALLEGRO_ALIGN_CENTER,
        "CONTROLS:"
    );
    
    al_draw_text(
        font,
        al_map_rgb_f(0,1,0),
        BUFFER_W / 2, 100,
        ALLEGRO_ALIGN_CENTER,
        "MOVE: Arrow keys (left / right)"
    );

    al_draw_text(
        font,
        al_map_rgb_f(0,1,0),
        BUFFER_W / 2, 120,
        ALLEGRO_ALIGN_CENTER,
        "PEW-PEW: press 'X' to shoot"
    );

    al_draw_text(
        font,
        al_map_rgb_f(1,1,1),
        BUFFER_W / 2, 170,
        ALLEGRO_ALIGN_CENTER,
        "Press 'B' to return to menu"
    );

    al_draw_text(
        font,
        al_map_rgb_f(1,1,1),
        BUFFER_W / 2, 190,
        ALLEGRO_ALIGN_CENTER,
        "or 'ESC' to quit"
    );


    disp_post_draw();
}

// MAIN ------------------------------------------------------

int main() {
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    disp_init();

    audio_init();

    must_init(al_init_image_addon(), "image");
    sprites_init();

    hud_init();

    must_init(al_init_primitives_addon(), "primitives");

    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    init_all();

    frames = 0;
    score = 0;
    int menu = 0;
    int level = 1;

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);

    while(1) {
        al_wait_for_event(queue, &event);

        // main menu
        if(menu == 0) {
            start_menu();

            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_P) {
                    menu = 1;
                    continue;
                } else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) break;
                else if (event.keyboard.keycode == ALLEGRO_KEY_T) {
                    menu = 2;
                    continue;
                }
            } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
        }

        // tutorial
        if(menu == 2) {
            tutorial();

            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_B) {
                    menu = 0;
                    continue;
                } else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) break;
            } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
        }

        // gaming \0/
        if(menu == 1) {
            switch(event.type) {
                case ALLEGRO_EVENT_TIMER:
                    fx_update();
                    shots_update(shots);
                    shots_update(ship.ship_shots);
                    stars_update();
                    ship_update();
                    aliens_update(level);
                    obstacles_update();
                    hud_update();

                    // mothership spawns at frame 1200
                    if (frames == 1200) mothership_init();
                    
                    if((game_over_test()) || (aliens_position_tester())) {
                        while (1) {
                            al_wait_for_event(queue, &event);

                            disp_pre_draw();
                            al_clear_to_color(al_map_rgb(0,0,0));

                            al_draw_text(
                                font,
                                al_map_rgb_f(1,1,1),
                                BUFFER_W / 2, BUFFER_H / 3,
                                ALLEGRO_ALIGN_CENTER,
                                "G A M E  O V E R"
                            );

                            al_draw_text(
                                font,
                                al_map_rgb_f(1,1,1),
                                (BUFFER_W / 2), (BUFFER_H / 2),
                                ALLEGRO_ALIGN_CENTER,
                                "Press 'ESC' to quit or 'R' to restart"
                            );

                            disp_post_draw();

                            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                                    done = true;
                                    break;
                                } else if (event.keyboard.keycode == ALLEGRO_KEY_R) {
                                    init_all();
                                    frames = 0;
                                    score_display = 0;
                                    score = 0;
                                    menu = 0;

                                    break;
                                }
                            }

                            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                                done = true;
                                break;
                            }
                        }
                    }

                    if(key[ALLEGRO_KEY_ESCAPE])
                        done = true;

                    redraw = true;
                    frames++;
                    break;

                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    break;
                }

            if(done) break;

            keyboard_update(&event);

            if(redraw && al_is_event_queue_empty(queue)) {
                disp_pre_draw();
                al_clear_to_color(al_map_rgb(0,0,0));
               
                stars_draw();
                aliens_draw();
                shots_draw(ship.ship_shots);
                shots_draw(shots);
                fx_draw();
                ship_draw();
                obstacles_draw();
                hud_draw();

                if (new_round_test()) level++;

                if(frames > 1200) mothership_draw();

                disp_post_draw();
                redraw = false;
            }
        }      
    }

    deinit_all();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}