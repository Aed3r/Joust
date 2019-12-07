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
    b->brd[m].lives = 3;
    b->brd[m].score = 0;
    b->brd[m].deathTime = -1;
    b->brd[m].flapped = 0;
    b->brd[m].gotStuck = 0;
    b->brd[m].onPlatform = 0;

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
        b->lives -= 1;
    } /* Mobs "transform" into eggs and stay at the same place */

    /* Reset velocities */
    b->hVel = 0;
    b->vVel = 0;
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
 * Handles the collision of two birds. Returns :
 * 1 if brd1 is the winner
 * 0 if there's no winner
 * -1 if brd2 is the winner
 * DOES NOT HANDLE physics
 */
int joust (bird *brd1, bird *brd2) {
    /* Make sure both birds are of different kind */
    if (brd1->b.isMob != brd2->b.isMob) {
        if (roundf(brd1->p.y) < roundf(brd2->p.y)) { /* brd1 won */
            handleDeath(brd2);
            return 1;
        } else if (roundf(brd2->p.y) < roundf(brd1->p.y)) { /* brd2 won */
            handleDeath(brd1);
            return -1;
        }
    }
    /* Both birds are players, mobs or there was a tie */
    return 0;
}

/*
 * Calculates the next x and y coordinates for any bird using its properties only
 * Handles collisions (initiates joust for chars colliding with mobs)
 * Detects screen edge and moves bird accordingly
 * TODO: put movement in loop to detect collisions
 */
void moveBird (bird *b, birds *brds, platforms p) {
    int tmpID, i, tmpVal, steps, done = 0;
    float angle;
    point increment;
    
    /* Apply gravity */
    if (!b->onPlatform) b->vVel -= b->b.vSpeed;

    /* Get angle to destination */
    angle = atan2(-b->vVel, b->hVel); 

    /* Get smallest increment */
    increment.x = cos(angle);
    increment.y = -sin(angle);

    /* The biggest absolute velocity determines the amount of steps to take */
    if (abs(b->hVel) > abs(b->vVel)) steps = abs(b->hVel);
    else steps = abs(b->vVel);

    /* Go to the destination step by step to check for collisions. If there are, stop */
    for (i = 0; i < steps && !done; i++) {
        /* Increment the X axis first */
        b->p.x += increment.x;

        /* Test for collision with another bird */
        if ((tmpID = birdCollision(*b, *brds)) != -1) { 
            /* Find the bird the collision happened with */
            i = 0;
            while (brds->brd[i].instanceID != tmpID) i++;
            /* Handle collision and get potential joust result */
            tmpVal = joust(b, &brds->brd[i]);
            /* Bounce surviving bird(s) off the other */
            switch (tmpVal) {
            case 1:
                b->hVel *= -1;
                b->dir *= -1;
                break;
            case 0:
                /* Move second bird according to first ones direction to prevent getting stuck */
                if (b->hVel > 0) brds->brd[i].p.x = b->p.x + b->b.o.s.width + 5;
                else brds->brd[i].p.x = b->p.x - brds->brd[i].b.o.s.width - 5;
                /* Bounce both birds off each other */
                brds->brd[i].hVel *= -1;
                brds->brd[i].dir *= b->dir;
                b->hVel *= -1;
                b->dir *= -1;
                break;
            case -1:
                brds->brd[i].hVel *= -1;
                brds->brd[i].dir *= -1;
                b->p.x += increment.x; /* compensate for the next move */
                break;
            }
            /* Go back one step and end progression */
            b->p.x -= increment.x;
            done = 1;
        }

        /* Test for collision with a platform */
        if ((tmpID = platCollision(*b, p, 0)) != -1) {
            /* Test if the bird wasn't able to free itself last round */
            if (b->gotStuck > 10) {
                /* Find platform */
                i = 0;
                while (p.plt[i].instanceID != tmpID) i++;
                /* Move bird to top of platform */
                b->p.y = p.plt[i].p.y - b->b.o.s.height - 1;
                b->gotStuck = 0;
            }
            /* Bounce off the other way */
            b->hVel *= -1;
            b->dir *= -1;
            b->p.x -= increment.x;
            b->gotStuck += 1;
            done = 1;
        } else b->gotStuck = 0;
        
        /* If bird hasn't collided horizontally yet */
        if (!done) {
            /* Increment the Y axis second */
            b->p.y -= increment.y;

            /* Test for collision with another bird */
            if ((tmpID = birdCollision(*b, *brds)) != -1) { 
                /* Find the bird the collision happened with */
                i = 0;
                while (brds->brd[i].instanceID != tmpID) i++;
                /* Handle collision and get potential joust result */
                tmpVal = joust(b, &brds->brd[i]);
                /* Bounce surviving bird(s) off the other */
                switch (tmpVal) {
                case 1:
                    b->vVel *= -1;
                    break;
                case 0:
                    /* Move second bird according to first ones direction to prevent getting stuck */
                    if (b->vVel > 0) brds->brd[i].p.y = b->p.y - brds->brd[i].b.o.s.height - 5;
                    else brds->brd[i].p.y = b->p.y + b->b.o.s.height + 5;
                    /* Bounce both birds off each other */
                    b->vVel *= -1;
                    brds->brd[i].vVel *= -1;
                    break;
                case -1:
                    brds->brd[i].vVel *= -1;
                    break;
                }
                /* Go back one step (if no dead) and end progression */
                if (tmpVal != -1) b->p.y += increment.y;
                done = 1;
            }

            /* Test for collision with a platform */
            if ((tmpID = platCollision(*b, p, 0)) != -1) {
                /* Find platform */
                i = 0;
                while (p.plt[i].instanceID != tmpID) i++;
                /* Act according to side (top or bottom) */
                /* Bird bumps its head on platform */
                if (increment.y >= 0) b->vVel *= -1;
                /* Bird lands on platform */
                else {
                    b->p.y = p.plt[i].p.y - b->b.o.s.height;
                    b->onPlatform = 1;
                    b->vVel = 0;
                }
                b->p.y += increment.y;
                done = 1;
            }
            
            /* Detect when bird not on platform anymore */
            if(b->onPlatform && platCollision(*b, p, 1) == -1) b->onPlatform = 0;
        }
    }

    /* Bumping top of screen */
    if (b->p.y < 0) {
        b->p.y = 0;
        b->vVel = -b->vVel;
    }
    /* Bumping bottom of screen */
    if (b->p.y + b->b.o.s.height > SCREENHEIGHT) {
        b->p.y = SCREENHEIGHT - b->b.o.s.height;
        b->vVel = 0; /* -b->vVel is a LOT more fun */
    }
    /* Bumping left side of screen */
    if (b->p.x < -b->b.o.s.width / 2) {
        b->p.x = SCREENHEIGHT + b->b.o.s.width / 2;
    }
    /* Bumping right side of screen */
    if (b->p.x > SCREENWIDTH + b->b.o.s.width / 2) {
        b->p.x = -b->b.o.s.width / 2;
    }

    /* Cap maximal velocities */
    if (b->hVel > MAXVEL) b->hVel = MAXVEL;
    if (b->vVel > MAXVEL) b->vVel = MAXVEL;
    if (b->hVel < -MAXVEL) b->hVel = -MAXVEL;
    if (b->vVel < -MAXVEL) b->vVel = -MAXVEL;
}

/*
 * Updates the position of a player controlled bird
 */
void updateCharPos (bird *b) {
    /* Player 1's input */
    if (b->player == 1 && b->deathTime == -1) {
        /* Test for flap */
        if(MLV_get_keyboard_state(MLV_KEYBOARD_s) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_w) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_SPACE) == MLV_PRESSED){ /* Flap */
            if (!b->flapped) {
                b->vVel += b->b.flapStrength;
                b->flapped = 1;
            }
        } else b->flapped = 0;
        /* Test for left */
        if(MLV_get_keyboard_state(MLV_KEYBOARD_a) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_q) == MLV_PRESSED){ /* Left */
            if (b->flapped || b->vVel == 0 || b->vVel == 1) b->hVel -= b->b.hSpeed; /* Prevent direction changes midair without flapping */
            b->dir = -1;
        }
        /* Test for right */
        if(MLV_get_keyboard_state(MLV_KEYBOARD_d) == MLV_PRESSED){ /* Right */
            if (b->flapped || b->vVel == 0 || b->vVel == 1) b->hVel += b->b.hSpeed; /* Prevent direction changes midair without flapping */
            b->dir = 1;
        }
    /* Player 2's input */
    } else if (b->player == 2 && b->deathTime == -1) {
        /* Test for flap */
        if(MLV_get_keyboard_state(MLV_KEYBOARD_DOWN) == MLV_PRESSED || MLV_get_keyboard_state(MLV_KEYBOARD_UP) == MLV_PRESSED){ /* Flap */
            if (!b->flapped) {
                b->vVel += b->b.flapStrength;
                b->flapped = 1;
            }
        } else b->flapped = 0;
        /* Test for left */
        if(MLV_get_keyboard_state(MLV_KEYBOARD_LEFT) == MLV_PRESSED){ /* Left */
            if (b->flapped || b->vVel == 0 || b->vVel == 1) b->hVel -= b->b.hSpeed; /* Prevent direction changes midair without flapping */
            b->dir = -1;
        }
        /* Test for right */
        if(MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT) == MLV_PRESSED){ /* Right */
            if (b->flapped || b->vVel == 0 || b->vVel == 1) b->hVel += b->b.hSpeed; /* Prevent direction changes midair without flapping */
            b->dir = 1;
        }
    }

    if (b->hVel > MAXVEL) b->hVel = MAXVEL;
    if (b->vVel > MAXVEL) b->vVel = MAXVEL;
}

/*
 * Updates a mobs position using "AI"
 * Detects potential obstacles and players
 * Either flies straight with small variations or towards the player, according to the aggressivity percentage
 */
void updateMobPos (bird *b, birds brd, platforms plt){

}

/*
 * Updates the positions of all bird instances 
 */ 
void updatePos (birds *brds, platforms plts) {
    int i;
    
    for (i = 0; i < brds->l; i++) {
        if (brds->brd[i].b.isMob) updateMobPos(&brds->brd[i], *brds, plts);
        else updateCharPos(&brds->brd[i]);
        moveBird(&brds->brd[i], brds, plts);
    }
}