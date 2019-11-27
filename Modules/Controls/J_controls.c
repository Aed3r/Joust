#include <stdlib.h>
#include <math.h>

#include "../Objects/J_objects.h"
#include "../values.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

/*
 * Creates a new birdType instance using its object ID
 * Returns the length of the birds array
 */ 
int spawnBird (int oID, birdTypes bt, birds *b, int x, int y, int dir, int player) {
    int i = 0, n = bt.l, m = b->l;

    /* Search the object corresponding to the passed id */
    while (i < n && bt.brdT[i].o.objectID != oID) i++;

    /* Test if the passed id couldn't be found */
    if (bt.brdT[i].o.objectID != oID) return -1;

    /* Set bird instance parameters */
    b->brd[m].b = bt.brdT[i];
    b->brd[m].dir = dir;
    b->brd[m].hVel = 0;
    b->brd[m].vVel = 0;
    b->brd[m].instanceID = m;
    b->brd[m].p.x = x;
    b->brd[m].p.y = y;
    b->brd[m].player = player;

    b->l++;

    return m + 1;
}

/*
 * Creates a new platform instance using the passed ID
 * Returns the length of the platforms array
 */ 
int createPlatform (int oID, objectTypes ot, platforms *p, int x, int y) {
    int i = 0, n = ot.l, m = p->l;

    /* Search the object corresponding to the passed id */
    while (i < n && ot.objT[i].objectID != oID) i++;

    /* Test if the passed id couldn't be found */
    if (ot.objT[i].objectID != oID) return -1;

    /* Set platform instance parameters */
    p->plt[m].instanceID = m;
    p->plt[m].o = ot.objT[i];
    p->plt[m].p.x = x;
    p->plt[m].p.y = y;

    p->l++;

    return m + 1;
}

/*
 * Returns 1 if two rectangles defined by (p1, s1) and (p2, s2) collide
 */
int areColliding (point p1, size s1, point p2, size s2) {
    int xCollides = 0, yCollides = 0;
    /* Tests for a potential collision on the x axis */
    if ((p1.x > p2.x && p1.x < p2.x + s2.width) ||
        (p1.x + s1.width > p2.x && p1.x + s1.width < p2.x + s2.width)) xCollides = 1;

    /* Tests for a potential collision on the y axis */
    if ((p1.y > p2.y && p1.y < p2.y + s2.height) ||
        (p1.y + s1.height > p2.y && p1.y + s1.height < p2.y + s2.height)) yCollides = 1;   

    return (xCollides && yCollides); 
}
/*
 * Returns which side the rectangle defined by (c2, s2) collides with the rectangle defined by (c1, s1)
 * 1: Right, 2: Top, 3: Left, 4: Bottom
 * Note on atan2: values  in ranges [0;PI] and [-PI;0]
 */
int collisionSide (point p1, size s1, point p2, size s2) {
    point c1;
    point c2;
    double angle;
    
    /* Get first rectangle center coords */
    c1.x = p1.x + s1.width / 2;
    c1.y = p1.y + s1.height / 2;
    /* Get second rectangle center coords */
    c2.x = p2.x + s2.width / 2;
    c2.y = p2.y + s2.height / 2;

    /* Get the angle (rad) of the second center relative to the first */
    angle = atan2(-(c2.y - c1.y), (c2.x - c1.x));
    
    if (angle > - M_PI / 4 && angle <= M_PI / 4) return 1; /* Right side */
    else if (angle > M_PI / 4 && angle <= 3 * M_PI / 4) return 2; /* Top side */
    else if (angle > 3 * M_PI / 4 && angle <= - 3 * M_PI / 4) return 3; /* Left side */
    else if (angle > - 3 * M_PI / 4 && angle <= - M_PI / 4) return 4; /* Bottom side */
    else return -1; /* should be impossible (i hope) */
}

/*
 * Returns the instance ID of any platform colliding with the bird passed as param
 * Returns -1 otherwise
 * yOffset lets you test for relative positions
 */
int platCollision (bird b, platforms p, int yOffset) {
    int i, n = p.l;

    b.p.y += yOffset;
    for (i = 0; i < n; i++) {
        if (areColliding(b.p, b.b.o.s, p.plt[i].p, p.plt[i].o.s)) return p.plt[i].instanceID;
    }
    b.p.y -= yOffset; /* TODO: test if important here */

    return -1;
}

/*
 * Returns the instance ID of any bird colliding with the bird passed as param
 * Returns -1 otherwise
 */
int birdCollision (bird b, birds brds) {
    int i, n = brds.l;

    for (i = 0; i < n; i++) {
        if(brds.brd[i].instanceID != b.instanceID) {
            if (areColliding(b.p, b.b.o.s, brds.brd[i].p, brds.brd[i].b.o.s)) return brds.brd[i].instanceID;
        }
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
 * Calculates the next x and y coordinates for any bird using its properties only
 * Handles collisions (initiates joust for chars colliding with mobs)
 * Detects screen edge and moves bird accordingly (TODO: add header ref for screen size)
 */
void moveBird (bird *b, birds *brds, platforms p) {
    /* We save the birds original position so as to roll it back when needed */
    int oX = b->p.x, oY = b->p.y;
    int tmpID, i, tmpVal;

    /* Set new positions now. Check for problems later */
    b->p.x += b->hVel;
    b->p.y += b->vVel;

    if ((tmpID = birdCollision(*b, *brds)) != -1) { /* Colliding with other bird */
        /* Find the bird the collision happened with */
        i = 0;
        while (brds->brd[i].instanceID != tmpID) i++;

        if (b->b.isMob != brds->brd[i].b.isMob) { /* Both birds are of different kind */
            tmpVal = joust(*b, brds->brd[i]); /* Get Joust result */
            if (tmpVal == 0) {
                b->hVel *= -1;
                brds->brd[i].hVel *= -1;
            }
        }
    }
    if ((tmpID = platCollision(*b, p, 0)) != -1) { /* Colliding with platform */
        /* Find the guilty platform */
        i = 0;
        while (p.plt[i].instanceID != tmpID) i++;
        
        /* Act according to collision location */
        switch (collisionSide(p.plt[i].p, p.plt[i].o.s, b->p, b->b.o.s))
        {
        case 1: /* Bird collides with right side of platform */
        case 3: /* Bird collides with left side of platform */
            b->hVel = -b->hVel; /* Bounce off.. */
            b->dir *= -1; /* .. and face the other way */
            /* TODO: check if the velocity should also slow down after collisions */
            break;
        case 2: /* Bird collides with top of platform (aka stands on it) */
            b->p.y = p.plt[i].p.y - b->b.o.s.height; /* Snap to platform vertically */
        case 4: /* Bird bumps its head on platform */
            b->vVel = 0; /* Stop all vertical movement */
            break;
        default:
            /* Do nothing I guess */
            break;
        }
    } else { /* Falling */
        b->vVel -= b->b.vSpeed;
    }
}

/*
 * Updates the position of a player controlled bird
 */
void updateCharPos (bird *b, bird brd[MAXINSTANCES], platform plt[PLATFORMS]) {
    /* TODO: update hVel if on ground only, unless there's a change of direction. Test what hVel becomes in that case */
}

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