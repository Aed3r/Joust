#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>

#include "Modules/Objects/J_objects.h"
#include "Modules/Screen/J_screen.h"
#include "Modules/Controls/J_controls.h"
#include "Modules/values.h"
#include "Modules/IA/J_ia.h"

/*
 * Create a formated string and put it in a char * varable printf style
 */
static int vspfunc(char *str, char *format, ...){
	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(str, format, aptr); 
	va_end(aptr);

	return ret;
}

int main() {
    objectTypes oT;
    birdTypes bT;
    birds b;
    platforms p;

    int done = 0, close = 0, waveCounter = 3, cdTime = -1, tmpMobCount, i, tmp, nbjr = 0;
    char tmpText[200];

    b.l = 0;
    p.l = 0;

    MLV_create_window("Joust", NULL, SCREENWIDTH, SCREENHEIGHT);
    MLV_change_frame_rate(30);

    /* Import Objects */
    importOBJs(&oT, "Data/Files/objects");
    importBirdTypes(&bT, &oT, "Data/Files/birds");

    /* Make sure everything's loaded nicely */
    if (oT.l < 7 || bT.l < 5) {
        printf("ERROR: Something didn't load correctly!\n");
        exit(EXIT_FAILURE);
    }

    /* Platform creation */
    createPlatform(1, oT, &p, 300, 957, 0);
    createPlatform(1, oT, &p, 500, 957, 0);
    createPlatform(1, oT, &p, 0, 700, 0);
    createPlatform(1, oT, &p, 800, 700, 0);
    createPlatform(1, oT, &p, 990, 700, 1);
    createPlatform(1, oT, &p, 393, 500, 0);
    createPlatform(1, oT, &p, 0, 150, 0);
    createPlatform(1, oT, &p, 800, 150, 0);
    createPlatform(1, oT, &p, 990, 150, 1);
    createPlatform(1, oT, &p, 0, 1000, 1);
    createPlatform(1, oT, &p, 200, 1000, 1);
    createPlatform(1, oT, &p, 700, 1000, 1);
    createPlatform(1, oT, &p, 900, 1000, 1);

    /* SCREEN TEST */
    while(close != 1){
        nbjr = 0;
        b.l = 0;
        done = 0;
        waveCounter = 3;
        cdTime = -1;
        switch (dispMenu("menu.png")) {
            case 2:
                spawnBird(4, bT, &b, 562, 872, 1, 2);
                nbjr ++;
                __attribute__((fallthrough));
            case 1:
                spawnBird(2, bT, &b, 343, 872, -1, 1);
                nbjr ++;
                vspfunc(tmpText,"Début du jeu à %d joueur(s)", nbjr);
                dispText(tmpText);
                MLV_actualise_window();
                MLV_wait_seconds(1);
                break;
            case 3:
                /* TODO: show scores */
                break;
            case 0:
                done = 1;
                close = 1;
                break;
            default:
                exit(EXIT_FAILURE);
                break;
        }


        while(done != 1) {
            /* Count living mobs */
            tmpMobCount = 0;
            for(i = 0; i < b.l; i++) if (b.brd[i].b.isMob && b.brd[i].lives > 0) tmpMobCount++;

            /* All enemies are dead */
            if (tmpMobCount == 0) {
                /* Start coutdown and display wave number */
                if (cdTime == -1) cdTime = MLV_get_time();
                else if (MLV_get_time() >= cdTime + WAVECOOLDOWN * 1000) {
                    /* Countdown over. Spawn new mobs over old ones */
                    if (nbjr == 2) b.l = 2;
                    else b.l = 1;
                    for (i = 0; i < waveCounter % 10; i++) {
                        /* Spawn correct type of mob */
                        switch (waveCounter / 10) {
                        case 0:
                            /* Easy difficulty */
                            spawnBird(6, bT, &b, -1, -1, 1, -1);
                            break;
                        case 1:
                            /* Medium difficulty */
                            spawnBird(7, bT, &b, -1, -1, 1, -1);
                            break;
                        default:
                            /* >= 2: Hard difficulty */
                            spawnBird(3, bT, &b, -1, -1, 1, -1);
                            break;
                        }
                    /* Center bird on empty platform */
                        tmp = findFreePlat(b, p);
                        b.brd[b.l - 1].p.x = p.plt[tmp].p.x + p.plt[tmp].o.s.width / 2 - b.brd[b.l-1].b.o.s.width / 2;
                        b.brd[b.l - 1].p.y = p.plt[tmp].p.y - b.brd[b.l-1].b.o.s.height;
                    }

                    /* If not on first wave, pad with additional mobs */
                    if (waveCounter / 10 != 0) {
                        for (i = waveCounter % 10; i < MOBSPERWAVE; i++) {
                            switch (waveCounter / 10) {
                            case 1:
                                /* Easy difficulty */
                                spawnBird(6, bT, &b, -1, -1, 1, -1);
                                break;
                            default:
                                /* Medium difficulty */
                                spawnBird(7, bT, &b, -1, -1, 1, -1);
                                break;
                            }
                            /* Center bird on empty platform */
                            tmp = findFreePlat(b, p);
                            b.brd[b.l - 1].p.x = p.plt[tmp].p.x + p.plt[tmp].o.s.width / 2 - b.brd[b.l-1].b.o.s.width / 2;
                            b.brd[b.l - 1].p.y = p.plt[tmp].p.y - b.brd[b.l-1].b.o.s.height;
                        }
                    }

                    /* Increment wave counter */
                    waveCounter++;
                    /* Increase difficulty level */
                    if (waveCounter % 10 == MOBSPERWAVE) waveCounter += 10 - MOBSPERWAVE;
                    /* Reset cooldown counter */
                    cdTime = -1;
                } else {
                    /* Countdown not over. Diplay wave number */
                    vspfunc(tmpText, "WAVE %d", waveCounter / 10 * MOBSPERWAVE + waveCounter % 10 - 2);
                    /*dispText(tmpText);*/
                    MLV_actualise_window(); 
                }
            }

            dispClear();
            updatePos(&b, p, oT);
            dispFrame(p, b, oT);
        
            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
            if(nbjr == 1 && b.brd[0].lives == 0){
                done = 1;
                dispText("Everyone died\n GAME OVER");
                MLV_actualise_window();
                MLV_wait_seconds(5);
            }else if(b.brd[0].lives == 0 && b.brd[1].lives == 0){
                done = 1;
                dispText("Everyone died\n GAME OVER");
                MLV_actualise_window();
                MLV_wait_seconds(5);
            }
                if(MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED) done = 1;
        }
    }
    MLV_free_window();
    exit(0);
}