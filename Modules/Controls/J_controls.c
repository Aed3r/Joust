#include <stdlib.h>
#include <math.h>
#include <MLV/MLV_all.h>

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
    b->brd[m].deathTime = -1;
    b->brd[m].flapped = 0;

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
 * Handle a birds parameters at their death 
 */
void handleDeath (bird *b) {
    /* Get time of death */
    b->deathTime = MLV_get_time(); 

    if (b->b.isMob == 0) {
        /* Hide player bird at screen edge */
        b->p.x = SCREENWIDTH; 
        b->p.y = SCREENWIDTH;
    } /* Mobs "transform" into eggs and stay at the same place */

    /* Reset velocities */
    b->hVel = 0;
    b->vVel = 0;
}

/*
 * Returns which side of the rectangle defined by (p1, s1), the second rectangle (p2, s2) collides with
 * i.e : first rect is a platform and second a bird
 * 1: Right, 2: Top, 3: Left, 4: Bottom
 */
int collisionSide (point p1, size s1, point p2, size s2) {
    if (p2.y + s2.height >= p1.y) return 2; /* Top */
    else if (p2.y <= p1.y + s1.height) return 4; /* Bottom */
    else { /* One side */
        if (p2.x - s2.width <= p1.x) return 1; /* Right */
        else return 3; /* Left */
    }
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
 * TODO: handle more death related stuff here
 */
int joust (bird brd1, bird brd2) {
    if (brd1.p.y > brd2.p.y) return 1;
    else if (brd2.p.y > brd1.p.y) return -1;
    else return 0;
}

/*
 * Calculates the next x and y coordinates for any bird using its properties only
 * Handles collisions (initiates joust for chars colliding with mobs)
 * Detects screen edge and moves bird accordingly
 * TODO: put movement in loop to detect collisions
 */
void moveBird (bird *b, birds *brds, platforms p) {
    int tmpID, i, tmpVal;

    /* Set new positions now. Check for problems later */
    b->p.x += b->hVel;
    b->p.y -= b->vVel;

    if ((tmpID = birdCollision(*b, *brds)) != -1) { /* Colliding with other bird */
        /* Find the bird the collision happened with */
        i = 0;
        while (brds->brd[i].instanceID != tmpID) i++;

        tmpVal = joust(*b, brds->brd[i]); /* Get Joust result */

        if (b->b.isMob != brds->brd[i].b.isMob && tmpVal != 0) { /* Both birds are of different kind and not in a tie */
            if (tmpVal == 1) handleDeath(&brds->brd[i]); /* First bird won */
            else handleDeath(b); /* Second bird won */
            /* TODO: Handle scores */
        } else { /* Two birds of the same kind bounce off each other */
            b->hVel *= -1;
            b->dir *= -1;
            brds->brd[i].hVel *= -1;
            brds->brd[i].dir *= -1;
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
            b->hVel = -b->hVel; /* Bounce off */
            break;
        case 2: /* Bird collides with top of platform (aka stands on it) */
            b->p.y = p.plt[i].p.y - b->b.o.s.height; /* Snap to platform vertically */
            b->vVel = 0; /* Stop all vertical movement */
            break;
        case 4: /* Bird bumps its head on platform */
            b->p.y = p.plt[i].p.y + p.plt[i].o.s.height; /* Snap to platform end */
            b->vVel = -b->vVel; /* Bounce off */
            break;
        default:
            /* Do nothing I guess */
            break;
        }
    } else { /* Falling */
        b->vVel -= b->b.vSpeed;
    }

    if (b->p.y < 0) {
        b->p.y = 0;
        b->vVel = -b->vVel;
    }
    if (b->p.y + b->b.o.s.height > SCREENHEIGHT) {
        b->p.y = SCREENHEIGHT - b->b.o.s.height;
        b->vVel = 0; /* -b->vVel is a LOT more fun */
    }
    if (b->p.x < -b->b.o.s.width / 2) {
        b->p.x = SCREENHEIGHT + b->b.o.s.width / 2;
    }
    if (b->p.x > SCREENWIDTH + b->b.o.s.width / 2) {
        b->p.x = -b->b.o.s.width / 2;
    }

    if (b->hVel > MAXVEL) b->hVel = MAXVEL;
    if (b->vVel > MAXVEL) b->vVel = MAXVEL;
}

/*
 * Updates the position of a player controlled bird
 */
void updateCharPos (bird *b) {
    if (b->player == 1) {
        if(MLV_get_keyboard_state(MLV_KEYBOARD_a) == MLV_PRESSED){ /* Left */
            b->hVel -= b->b.hSpeed;
            b->dir = -1;
        }
        if(MLV_get_keyboard_state(MLV_KEYBOARD_d) == MLV_PRESSED){ /* Right */
            b->hVel += b->b.hSpeed;
            b->dir = 1;
        }
        if(MLV_get_keyboard_state(MLV_KEYBOARD_s) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_w) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_SPACE) == MLV_PRESSED){ /* Flap */
            if (!b->flapped) {
                b->vVel += b->b.flapStrength;
                b->flapped = 1;
            }
        } else b->flapped = 0;
    } else if (b->player == 2) {
        if(MLV_get_keyboard_state(MLV_KEYBOARD_LEFT) == MLV_PRESSED){ /* Left */
            b->hVel -= b->b.hSpeed;
            b->dir = -1;
        }
        if(MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT) == MLV_PRESSED){ /* Right */
            b->hVel += b->b.hSpeed;
            b->dir = 1;
        }
        if(MLV_get_keyboard_state(MLV_KEYBOARD_DOWN) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_UP) == MLV_PRESSED){ /* Flap */
            if (!b->flapped) {
                b->vVel += b->b.flapStrength;
                b->flapped = 1;
            }
        } else b->flapped = 0;
    }

    if (b->hVel > MAXVEL) b->hVel = MAXVEL;
    if (b->vVel > MAXVEL) b->vVel = MAXVEL;
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