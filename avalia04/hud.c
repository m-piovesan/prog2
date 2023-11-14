

#include "hud.h"

extern long frames;
extern long score;

void hud_init() {
    font = al_create_builtin_font();
    must_init(font, "font");

    score_display = 0;
}

void hud_deinit() {
    al_destroy_font(font);
}

int hud_update() {
    if(frames % 2)
        return 0;

    for(long i = 5; i > 0; i--) {
        long diff = 1 << i;
        if(score_display <= (score - diff))
            score_display += diff;
    }

    if(ship.lives <= 0) {
        // al_clear_to_color(al_map_rgb(0,0,0));

        // al_draw_text(
        //     font,
        //     al_map_rgb_f(1,1,1),
        //     BUFFER_W / 2, BUFFER_H / 2,
        //     ALLEGRO_ALIGN_CENTER,
        //     "G A M E  O V E R"
        // );

        return 1;
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

// colocar teste pra ver se a nave morreu também
void end_game_test() {
    int testaAliens = 0;

    for(int i = 0; i < ALIENS_N; i++) {
        if(!aliens[i].used)
            testaAliens++;
    }

    if (testaAliens == ALIENS_N) {
        aliens_init();
        
        if (ship.lives < 5) ship.lives++;

        // return 2;
    }
    // return 0;
}