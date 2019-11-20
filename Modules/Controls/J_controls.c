#include <stdlib.h>

#include "../Objects/J_objects.h"

/*
 * Creates a new birdType instance using its object ID
 * Returns the length of the birds array
 */ 
int spawnBird (int oID, birdTypes bt, birds b, int x, int y, int dir, int player) {
    int i, n = bt.l, m = b.l;

    for (i = 0; i < n; i++) {
        if (bt.brdT[i].o.objectID == oID) {
            b.brd[m].b = bt.brdT[i];
            b.brd[m].dir = dir;
            b.brd[m].velY = 0;
            b.brd[m].instanceID = m;
            b.brd[m].p.x = x;
            b.brd[m].p.y = y;
            b.brd[m].player = player;
        }
    }

    return m + 1;
}

/*
 * Creates a new platform instance using the passed ID
 * Returns the length of the platforms array
 */ 
int createPlatform (int oID, objectTypes ot, platforms p, int x, int y) {
    int i, n = ot.l, m = p.l;

    for (i = 0; i < n; i++) {
        if (ot.objT[i].objectID == oID) {
            p.plt[m].instanceID = m;
            p.plt[m].o = ot.objT[i];
            p.plt[m].p.x = x;
            p.plt[m].p.y = y;
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
 * yOffset lets you offset the vertical position of the bird
 */
int platCollision (bird b, platforms p, int yOffset) {
    int i, n = p.l;

    b.p.y += yOffset;
    for (i = 0; i < n; i++) {
        if (areColliding(b.p, b.b.o.s, p.plt[i].p, p.plt[i].o.s)) return p.plt[i].instanceID;
    }
    b.p.y -= yOffset;

    return -1;
}

/*
 * Returns the instance ID of any bird colliding with the bird passed as param
 * Returns -1 otherwise
 */
int birdCollision (bird b, birds brds) {
    int i, n = brds.l;

    for (i = 0; i < n; i++) {
        if (areColliding(b.p, b.b.o.s, brds.brd[n].p, brds.brd[n].b.o.s)) return brds.brd[n].instanceID;
    }

    return -1;
}

/*
 * Returns the result of a joust:
 * 1 if brd1 is the winner
 * 0 for for a tie
 * -1 if brd2 is the winner
 */
int joust (bird brd1, bird brd2) {
    if (brd1.p.y > brd2.p.y) return 1;
    else if (brd2.p.y > brd1.p.y) return -1;
    else return 0;
}

/*
 * Calculates the next x and y coordinates for any bird only using bird properties
 * Detects collisions (initiates joust for chars colliding with mobs)
 * Detects screen edge and moves bird accordingly (TODO: add header ref for screen size)
 */
void moveBird (bird b, birds brds, platforms p) {
    int ox = b.p.x, oy = b.p.y;

    if (b.velY == 0) { /* On platform */
        b.p.x = b.p.x + b.b.runSpeed * b.dir;
        b.p.y = b.p.y;
    } else { /* In air */
        b.p.x = b.p.x + b.b.glideSpeed * b.dir;
        b.p.y = b.p.y + b.velY;
    }

    if (platCollision(b, p, -1)) { /* Test if is on platform */
        b.velY = 0;
        b.p.x = /* Snap to platform */
    } else {
        b.velY -= b.b.glideSpeed;
    }

    if(platCollision(b, p, 0)) {

    }


}

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