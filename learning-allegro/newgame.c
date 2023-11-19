// gcc newgame.c -o spcinv $(pkg-config allegro-5 allegro_font-5 allegro_primitives-5 --libs --cflags)

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define GRID_WIDTH 10
#define GRID_HEIGHT 10

int grid[GRID_WIDTH][GRID_HEIGHT];
int playerX, playerY;

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *event_queue;

void must_init(bool test, const char *description) {
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

void init() {
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");

    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    must_init(display, "display");

    event_queue = al_create_event_queue();
    must_init(event_queue, "event queue");

    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
}

void draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            int element = grid[x][y];
            if (element == 1) {
                al_draw_filled_rectangle(x * (SCREEN_WIDTH / GRID_WIDTH), y * (SCREEN_HEIGHT / GRID_HEIGHT),
                    (x + 1) * (SCREEN_WIDTH / GRID_WIDTH), (y + 1) * (SCREEN_HEIGHT / GRID_HEIGHT), al_map_rgb(255, 0, 0));
            } else if (element == 2) {
                al_draw_filled_rectangle(x * (SCREEN_WIDTH / GRID_WIDTH), y * (SCREEN_HEIGHT / GRID_HEIGHT),
                    (x + 1) * (SCREEN_WIDTH / GRID_WIDTH), (y + 1) * (SCREEN_HEIGHT / GRID_HEIGHT), al_map_rgb(0, 0, 255));
            }
        }
    }

    al_flip_display();
}

void update() {
    // Atualize a lógica do jogo aqui

    // Por exemplo, você pode adicionar movimento de inimigos
    // Aqui, estou apenas fazendo com que um inimigo desça uma linha a cada quadro
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = GRID_HEIGHT - 1; y > 0; y--) {
            grid[x][y] = grid[x][y - 1];
        }
    }

    // Gere novos inimigos na parte superior da grade
    for (int x = 0; x < GRID_WIDTH; x++) {
        grid[x][0] = (rand() % 4) == 0 ? 2 : 0; // Gera inimigos aleatoriamente
    }

    // Verifique a detecção de colisões, pontuação e outras lógicas do jogo
}


int main() {
    init();

    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            grid[x][y] = 0;
        }
    }

    playerX = GRID_WIDTH / 2;
    playerY = GRID_HEIGHT - 1;
    grid[playerX][playerY] = 1;

    bool done = false;
    while (!done) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                    if (playerX > 0) {
                        grid[playerX][playerY] = 0;
                        playerX--;
                        grid[playerX][playerY] = 1;
                    }
                    break;
                case ALLEGRO_KEY_RIGHT:
                    if (playerX < GRID_WIDTH - 1) {
                        grid[playerX][playerY] = 0;
                        playerX++;
                        grid[playerX][playerY] = 1;
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    done = true;
                    break;
            }
        }

        update();
        draw();
    }

    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}
