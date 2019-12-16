#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include <time.h>

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

    int done, close = 0, waveCounter, cdTime, tmpMobCount, i, tmp, nbjr;
    char tmpText[200];

    b.l = 0;
    p.l = 0;
    srand(time(0));

    MLV_create_window("Joust", NULL, SCREENWIDTH, SCREENHEIGHT);
    MLV_change_frame_rate(30);

    /* Import Objects */
    importOBJs(&oT, "Data/Files/objects");
    importBirdTypes(&bT, &oT, "Data/Files/birds");

    /* Make sure everything's loaded nicely */
    if (oT.l < 6 || bT.l < 4 || MOBSPERWAVE == 0) {
        printf("ERROR: Something didn't load correctly!\n");
        exit(EXIT_FAILURE);
    }

    /* Platform creation */
    /* Bottom */
    createPlatform(1, oT, &p, 550, 970, 0);
    createPlatform(1, oT, &p, 750, 970, 0);
    createPlatform(1, oT, &p, 350, 970, 0);
    createPlatform(1, oT, &p, 950, 970, 0);
    /* Left bottom */
    createPlatform(1, oT, &p, 0, 700, 0);
    /* Right bottom */
    createPlatform(1, oT, &p, 1300, 700, 0);
    /* Center top */
    createPlatform(1, oT, &p, 550, 300, 0);
    createPlatform(1, oT, &p, 750, 300, 0);
    /* Center bottom */
    createPlatform(1, oT, &p, 550, 685, 0);
    createPlatform(1, oT, &p, 750, 685, 0);
    /* Left top */
    createPlatform(1, oT, &p, 0, 200, 0);
    /* Right Top */
    createPlatform(1, oT, &p, 1300, 200, 0);
    createPlatform(1, oT, &p, 1500, 200, 1);
    /* Middle Left */
    createPlatform(1, oT, &p, 200, 450, 0);
    /* Middle Right */
    createPlatform(1, oT, &p, 1100, 450, 0);
    /* Bottom OOB */
    createPlatform(1, oT, &p, 0, 1000, 1);
    createPlatform(1, oT, &p, 200, 1000, 1);
    createPlatform(1, oT, &p, 400, 1000, 1);
    createPlatform(1, oT, &p, 600, 1000, 1);
    createPlatform(1, oT, &p, 800, 1000, 1);
    createPlatform(1, oT, &p, 1000, 1000, 1);
    createPlatform(1, oT, &p, 1200, 1000, 1);
    createPlatform(1, oT, &p, 1400, 1000, 1);

    /*TEST DES SCORES*/

    /* Main loop */
    while(close != 1){
        nbjr = 0;
        b.l = 0;
        done = 0;
        waveCounter = MOBSPERWAVE / 2;
        cdTime = -1;
        switch (dispMenu("menu.png")) {
            case 2:
                /* Two players */
                spawnBird(4, bT, &b, 562, 872, 1, 2);
                nbjr ++;
                __attribute__((fallthrough));
            case 1:
                /* One player */
                spawnBird(2, bT, &b, 343, 872, -1, 1);
                nbjr ++;
                vspfunc(tmpText,"Début du jeu à %d joueur(s)", nbjr);
                dispClear();
                dispText(tmpText);
                MLV_actualise_window();
                MLV_wait_seconds(1);
                break;
            case 3:
                /* Show scores */
                dispScore();
                done = 1;
                break;
            case 4:
                /*Save game*/
                break;
            case 5:
                /*Load game*/
                break;
            default:
                exit(EXIT_FAILURE);
                break;
        }

        printf("%d\n", loadGameState(&b, &waveCounter, &nbjr));

        while(done != 1) {
            /* Count living mobs */
            tmpMobCount = 0;
            for(i = 0; i < b.l; i++) if (b.brd[i].b.isMob && b.brd[i].lives > 0) tmpMobCount++;

            /* All enemies are dead */
            if (tmpMobCount == 0 && waveCounter != 0) {
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
                        default:
                            /* >= 1: Hard difficulty */
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
                            /* Easy bird */
                            spawnBird(6, bT, &b, -1, -1, 1, -1);
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
                    /* Reset cooldown counter and wave counter message */
                    cdTime = -1;
                    strcpy(tmpText, "");
                } else {
                    /* Countdown not over. Diplay wave number */
                    vspfunc(tmpText, "VAGUE %d", waveCounter / 10 * MOBSPERWAVE + waveCounter % 10 - (MOBSPERWAVE / 2) + 1);
                }
            }

            updatePos(&b, p, oT);
            dispFrame(p, b, oT, tmpText);
            if(nbjr == 1 && b.brd[0].lives == 0){
                done = 1;
                dispAskScore(b);
            }else if(b.brd[0].lives == 0 && b.brd[1].lives == 0){
                done = 1;
                dispAskScore(b);
            }
            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
            
            if(MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED) {
                saveGameState(b, waveCounter, nbjr);
                done = 1;
            }
        }
    }
    MLV_free_window();
    exit(EXIT_SUCCESS);
}