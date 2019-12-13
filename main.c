#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>

#include "Modules/Objects/J_objects.h"
#include "Modules/Screen/J_screen.h"
#include "Modules/Controls/J_controls.h"
#include "Modules/IA/J_ia.h"

int main() {
    objectTypes oT;
    birdTypes bT;
    birds b;
    platforms p;
    int done = 0;
    b.l = 0;
    p.l = 0;

    MLV_create_window("Joust", NULL, SCREENWIDTH, SCREENHEIGHT);
    MLV_change_frame_rate(30);

    /* Import Objects */
    importOBJs(&oT, "Data/Files/objects");
    importBirdTypes(&bT, &oT, "Data/Files/birds");

    /* Platform creation */
    createPlatform(1, oT, &p, 300, 957);
    createPlatform(1, oT, &p, 500, 957);
    createPlatform(1, oT, &p, 0, 700);
    createPlatform(1, oT, &p, 800, 700);
    createPlatform(1, oT, &p, 990, 700);
    createPlatform(1, oT, &p, 393, 500);
    createPlatform(1, oT, &p, 0, 150);
    createPlatform(1, oT, &p, 800, 150);
    createPlatform(1, oT, &p, 990, 150);

    /* SCREEN TEST */
    switch (dispMenu("menu.png")) {
    case 2:
        spawnBird(4, bT, &b, 562, 872, 1, 2);
        __attribute__((fallthrough));
    case 1:
        spawnBird(2, bT, &b, 343, 872, -1, 1);
        break;
    case 3:
        /* TODO: show scores */
        break;
    case 0:
    default:
        exit(EXIT_FAILURE);
        break;
    }

    spawnBird(3, bT, &b, 350, 200, 1, -1);

    while(done != 1) {
        dispClear();
        updatePos(&b, p);
        dispFrame(p, b, oT);

        MLV_actualise_window();
        MLV_delay_according_to_frame_rate();

        if(MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED) done = 1;
    }

    MLV_free_window();
    exit(0);
}