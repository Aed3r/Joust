#include "J_ia.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int isBelow(birds b, int n){
	if(b.brd[0].p.y < b.brd[n].p.y) return 1;
	return 0;
}

void moveAggIa(bird *b, birds brd){
    int a;
    srand(time(0));
    a = rand() %5;
	if (b->deathTime == -1) {
        /* Test for flap */
        if(isBelow(brd, b->instanceID)){ /*Flap*/
            if (b->flapped < 1) {
                b->vVel += b->b.flapStrength;
                b->flapped ++;
            }else if(b->flapped < 7){
                b->flapped ++;
            }else b->flapped = 0;
        }else if(a == 1){ /*A out of 10 chance to go there*/
            if (b->flapped < 1) {
                b->vVel += b->b.flapStrength;
                b->flapped ++;
            }else if(b->flapped < 10){
                b->flapped ++;
            }else b->flapped = 0;

        }
    }
}
void movePassiveIa(bird *b){
    int a;
    srand(time(0));
    a = rand() %2;
    if (b->deathTime == -1) {
        if(a == 1){ /*A out of 10 chance to go there*/
            if (b->flapped < 1) {
                b->vVel += b->b.flapStrength;
                b->flapped ++;
            }else if(b->flapped < 4){
                b->flapped ++;
            }else b->flapped = 0;
        }
    }
}