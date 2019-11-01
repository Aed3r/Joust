#include <stdlib.h>

#include "../Objects/J_objects.h"

/*
 * Creates a new birdType instance using its object ID
 * Returns the length of the birds array
 */ 
int spawnBird (int oID, birdType brdT[BIRDTYPES], int n, bird birds[MAXINSTANCES], int m, int x, int y, int dir, int player) {
    int i;

    for (i = 0; i < n; i++) {
        if (brdT[i].o.objectID == oID) {
            birds[m].b = brdT[i];
            birds[m].dir = dir;
            birds[m].flaps = 0;
            birds[m].instanceID = m;
            birds[m].p.x = x;
            birds[m].p.y = y;
            birds[m].player = player;
        }
    }

    return m + 1;
}

/*
 * Creates a new platform instance using the passed ID
 * Returns the length of the platforms array
 */ 
int createPlatform (int oID, objectType objT[OBJS], int n, platform plt[PLATFORMS], int m, int x, int y) {
    int i;

    for (i = 0; i < n; i++) {
        if (objT[i].objectID == oID) {
            plt[m].instanceID = m;
            plt[m].o = objT[i];
            plt[m].p.x = x;
            plt[m].p.y = y;
        }
    }

    return m + 1;
}

/*
 * Returns 1 if two rectangles defined by (p1, s1) and (p2, s2) collide
 */
int areColliding (point p1, size s1, point p2, size s2) {
    int xCollides = 0, yCollides = 0;
    if ((p1.x > p2.x && p1.x < p2.x + s2.width) ||
        (p1.x + s1.width > p2.x && p1.x + s1.width < p2.x + s2.width)) xCollides = 1;

    if ((p1.y > p2.y && p1.y < p2.y + s2.height) ||
        (p1.y + s1.height > p2.y && p1.y + s1.height < p2.y + s2.height)) yCollides = 1;   

    return (xCollides && yCollides); 
}

/*
 * Returns the instance ID of any platform colliding with the bird passed as param
 * Returns -1 otherwise
 */
int platCollision (bird *b, platform plt[PLATFORMS], int n) {
    int i;

    for (i = 0; i < n; i++) {
        if (areColliding(b->p, b->b.o.s, plt[i].p, plt[i].o.s)) return plt[i].instanceID;
    }

    return -1;
}

/*
 * Returns the instance ID of any bird colliding with the bird passed as param
 * Returns -1 otherwise
 */
int birdCollision (bird *b, bird brd[MAXINSTANCES], int n) {
    int i;

    for (i = 0; i < n; i++) {
        if (areColliding(b->p, b->b.o.s, brd[n].p, brd[n].b.o.s)) return brd[n].instanceID;
    }

    return -1;
}

/*
 * Returns the result of a joust:
 * 1 if brd1 is the winner
 * 0 for for a tie
 * -1 if brd2 is the winner
 */
int joust (bird *brd1, bird *brd2) {
    if (brd1->p.y > brd2->p.y) return 1;
    else if (brd2->p.y > brd1->p.y) return -1;
    else return 0;
}

/*
 * Calculates the next x and y coordinates for any bird only using bird properties
 * Detects collisions (initiates joust for chars colliding with mobs)
 * Detects screen edge and moves bird accordingly (TODO: add header ref for screen size)
 */
void moveBird (bird *b, bird brd[MAXINSTANCES], platform plt[PLATFORMS]);

/*
 * Updates the position of a player controlled bird
 */
void updateCharPos (bird *b, bird brd[MAXINSTANCES], platform plt[PLATFORMS]);

/*
 * Updates a mobs position using "AI"
 * Detects potential obstacles and players
 * Either flies straight with small variations or towards the player, according to the aggressivity percentage
 */
void updateMobPos (bird *b, bird brd[MAXINSTANCES], platform plt[PLATFORMS]);

/*
 * Updates the positions of all bird instances 
 */ 
void updatePos (bird brd[MAXINSTANCES]);