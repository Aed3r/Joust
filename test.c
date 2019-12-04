#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>

#include "Modules/Objects/J_objects.h"
#include "Modules/Screen/J_screen.h"
#include "Modules/Controls/J_controls.h"
#include "Modules/IA/J_ia.h"

/* Show all object types attributes */
void printOBJTs(objectTypes *o) {
    int i;

    printf("------- Loaded Objects (%d): -------\n", o->l);
    for (i = 0; i < o->l; i++) {
        printf(" - ID: %d ; Sprite: %s; Name: %s; Width: %d; Height: %d\n",
            o->objT[i].objectID, o->objT[i].spriteName, o->objT[i].name,
            o->objT[i].s.width, o->objT[i].s.height);
    }
}

/* Show all bird types attributes */
void printBRDTs(birdTypes *b) {
    int i;

    printf("------- Loaded Birds (%d): -------\n", b->l);
    for (i = 0; i < b->l; i++) {
        printf(" - objectID: %d ; Sprite: %s; Name: %s; Width: %d; Height: %d; runSpeed: %f; glideSpeed: %f; flapStrength: %f; respawnTime: %d; isMob: %d; aggressiveness: %d\n",
            b->brdT[i].o.objectID, b->brdT[i].o.spriteName, b->brdT[i].o.name, 
            b->brdT[i].o.s.width, b->brdT[i].o.s.height,
            b->brdT[i].hSpeed, b->brdT[i].vSpeed, b->brdT[i].flapStrength, 
            b->brdT[i].respawnTime, b->brdT[i].isMob, b->brdT[i].aggressiveness);
    }
}

/* Show all platform instances */
void printP(platforms *p) {
    int i;

    printf("------- Platform instances (%d) : -------\n", p->l);
    for (i = 0; i < p->l; i++) {
        printf("  - instanceID: %d; objectID: %d; Name : %s; X: %f; Y: %f\n", 
        p->plt[i].instanceID, p->plt[i].o.objectID, p->plt[i].o.name, p->plt[i].p.x, p->plt[i].p.y);
    }
}

/* Show all bird instances */
void printB(birds *b) {
    int i;

    printf("------- Bird instances (%d) : -------\n", b->l);
    for (i = 0; i < b->l; i++) {
        printf("  - instanceID: %d; objectID: %d; X: %f; Y: %f; dir: %d; vVel: %d; hVel: %d; player: %d\n", 
        b->brd[i].instanceID, b->brd[i].b.o.objectID, b->brd[i].p.x, 
        b->brd[i].p.y, b->brd[i].dir, b->brd[i].vVel, b->brd[i].hVel, b->brd[i].player);
    }
}

int main() {
    objectTypes oT;
    birdTypes bT;
    birds b;
    platforms p;
    int done = 0, i;
    int listArg[5];
    listArg[0] = 2;
    listArg[1] = 100;
    listArg[2] = 100;
    listArg[3] = 3;
    listArg[4] = 3;


    b.l = 0;
    p.l = 0;
    MLV_create_window("Joust", NULL, SCREENWIDTH, SCREENHEIGHT);
    MLV_change_frame_rate(30);
    /* OBJECTS TEST */
    importOBJs(&oT, "Data/Files/objects");
    importBirdTypes(&bT, &oT, "Data/Files/birds");

    printOBJTs(&oT);
    printBRDTs(&bT);

    /* CONTROLS CREATION TEST */
    createPlatform(1, oT, &p, 500, 500);
    spawnBird(2, bT, &b, 500, 350, 1, 1);
    spawnBird(3, bT, &b, 700, 360, 1, 2);
    spawnBird(3, bT, &b, 490, 340, 1, -1);

    printP(&p);
    printB(&b);

    /* SCREEN TEST */
    dispMenu("Data/Sprites/menu.png");
    MLV_actualise_window();
    dispClear();
    dispPlats(p);
    dispBirds(b);
    dispStatus(2, 100, 150, 3, 2);
    MLV_actualise_window(); 
    MLV_wait_keyboard_or_mouse(NULL, NULL, NULL, NULL, NULL);

    /* CONTROLS COLLISION TEST */
    printf("Platform collision Blue: %d\n", platCollision(b.brd[0], p, 0));
    printf("Platform collision Red: %d\n", platCollision(b.brd[1], p, 0));
    printf("Bird collision: %d\n", birdCollision(b.brd[0], b));

    /* CONTROLS PHYSICS TEST */

    b.l -= 1; /* "kill" the last bird */

    while(done != 1) {
        dispClear();
        for (i=0; i < b.l; i++) if (b.brd[i].b.isMob != 1) updateCharPos(&b.brd[i]);
        for (i = 0; i < b.l; i++) moveBird(&b.brd[i], &b, p);
        dispFrame(p, b, listArg);
        printf("hVel : %d ; vVel : %d\n", b.brd[0].hVel, b.brd[0].vVel);

        MLV_actualise_window();
        MLV_delay_according_to_frame_rate();

        if(MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED) done = 1;
    }

    MLV_free_window();
    exit(0);
}