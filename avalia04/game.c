#include <stdio.h>
#include <stdlib.h>

#include "aliens.h"
#include "audio.h"
#include "display.h"
#include "fx.h"
#include "general.h"
#include "hud.h"
#include "keyboard.h"
#include "obstacles.h"
#include "ship.h"
#include "shots.h"
#include "sprites.h"
#include "stars.h"

void init_all() {
    keyboard_init();
    fx_init();
    shots_init();
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

// --- main ---

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

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);

    while(1) {
        al_wait_for_event(queue, &event);

        switch(event.type) {
            case ALLEGRO_EVENT_TIMER:
                fx_update();
                shots_update();
                stars_update();
                ship_update();
                aliens_update();
                obstacles_update();

                if(frames == 120)
                    mothership_init();
                 
                if(hud_update() == 1) {
                    while (1) {
                        al_wait_for_event(queue, &event);

                        al_clear_to_color(al_map_rgb(0,0,0));

                        al_draw_text(
                            font,
                            al_map_rgb_f(1,1,1),
                            BUFFER_W / 2, BUFFER_H / 2,
                            ALLEGRO_ALIGN_CENTER,
                            "G A M E  O V E R"
                        );

                        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                                done = true;
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
            shots_draw();
            fx_draw();
            ship_draw();
            obstacles_draw();
            hud_draw();

            end_game_test();

            if(frames > 120) mothership_draw();

            disp_post_draw();
            redraw = false;
        }
    }

    deinit_all();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}