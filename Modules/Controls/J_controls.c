#include <stdlib.h>
#include <math.h>
#include <MLV/MLV_all.h>
#include <stdlib.h>
#include <time.h>

#include "../Objects/J_objects.h"
#include "../values.h"
#include "../IA/J_ia.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

/*
 * Creates a new birdType instance using its object ID
 * Returns the length of the birds array
 */ 
int spawnBird (int oID, birdTypes bt, birds *b, int x, int y, int dir, int player) {
    int i = 0, n = bt.l, m = b->l, a;
    a = rand() % 2;
    /* Search the object corresponding to the passed id */
    while (i < n && bt.brdT[i].o.objectID != oID) i++;

    /* Test if the passed id couldn't be found */
    if (bt.brdT[i].o.objectID != oID) return -1;

    /* Set bird instance parameters */
    b->brd[m].b = bt.brdT[i];
    if(b->brd[m].b.isMob){
        if (a == 1){
            b->brd[m].hVel = 10;
            b->brd[m].dir = 1;
        }else {
            b->brd[m].hVel = -10;
            b->brd[m].dir = -1;
        }
        b->brd[m].lives = 2; /* Only used technically. Please don't modify */
    }else{
        b->brd[m].hVel = 0;
        b->brd[m].dir = dir;
        b->brd[m].lives = PLAYERLIVES;
    }
    b->brd[m].vVel = 0;
    b->brd[m].instanceID = m;
    b->brd[m].p.x = x;
    b->brd[m].p.y = y;
    b->brd[m].player = player;
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
int createPlatform (int oID, objectTypes ot, platforms *p, int x, int y, int outOfBounds) {
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
    p->plt[m].outOfBounds = outOfBounds;

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
void handleDeath (bird *b, objectTypes oT) {
    b->lives -= 1;
    if (b->lives > 0)  {
        /* Get time of death */
        b->deathTime = MLV_get_time(); 

        if (b->b.isMob == 0) {
            /* Hide player bird at screen edge */
            b->p.x = SCREENWIDTH + 10; 
            b->p.y = SCREENHEIGHT + 10;
        } else {
            /* Mobs "transform" into eggs and stay at the same place. Change size */
            b->b.o.s.width = oT.objT[4].s.width;
            b->b.o.s.height = oT.objT[4].s.height;
        }

        /* Reset velocities */
        b->hVel = 0;
        b->vVel = 0;
    } else {
        /* Indefinitely move the bird off-screen */
        b->p.x = SCREENWIDTH + 10; 
        b->p.y = SCREENHEIGHT + 10;
        /* Indefinitely reset velocities */
        b->hVel = 0;
        b->vVel = 0;
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
            if (brds.brd[i].deathTime != -1 && brds.brd[i].b.isMob) printf("(%f %f) [%d %d] vs (%f %f) [%d %d]", 
            b.p.x, b.p.y, b.b.o.s.width, b.b.o.s.height, 
            brds.brd[i].p.x, brds.brd[i].p.y, brds.brd[i].b.o.s.width, brds.brd[i].b.o.s.height);

            if (areColliding(b.p, b.b.o.s, brds.brd[i].p, brds.brd[i].b.o.s)) {
                printf(" -> colliding\n");
                return brds.brd[i].instanceID;
            } else if (brds.brd[i].deathTime != -1 && brds.brd[i].b.isMob) printf(" -> not colliding\n");
        }
    }

    return -1;
}

/*
 * Handles the collision of two birds. Returns :
 * 1 if brd1 is the winner
 * 0 if there's a tie or both birds are of the same type
 * -1 if brd2 is the winner
 * 2 if a bird collects an egg or a dead player is involved
 * DOES NOT HANDLE ALL PHYSICS
 */
int joust (bird *brd1, bird *brd2, objectTypes oT) {
    /* One of the birds is a dead player */
    if ((!brd1->b.isMob && (brd1->lives == 0 || brd1->deathTime != -1)) ||
        (!brd2->b.isMob && (brd2->lives == 0 || brd2->deathTime != -1))) return 2;

    /* brd2 collects the egg of brd1 */
    if (brd1->deathTime != -1 && brd2->deathTime == -1 && brd1->b.isMob && !brd2->b.isMob) {
        brd2->score += brd1->b.value * 2;
        handleDeath(brd1, oT);
        return 2;
    }

    /* brd1 collects the egg of brd2 */
    if (brd2->deathTime != -1 && brd1->deathTime == -1 && brd2->b.isMob && !brd1->b.isMob) {
        brd1->score += brd2->b.value * 2;
        handleDeath(brd2, oT);
        return 2;
    }

    /* Check if both birds are players, mobs or there was a tie */
    /* Does not check if the platform they're on is the same */
    if (brd1->b.isMob == brd2->b.isMob || (brd1->onPlatform && brd2->onPlatform)) return 0;

    /* brd1 wins */
    if (brd1->p.y < brd2->p.y) { 
        handleDeath(brd2, oT);
        brd1->score += brd2->b.value;
        return 1;
    } 

    /* brd2 wins */
    if (brd2->p.y < brd1->p.y) { 
        handleDeath(brd1, oT);
        brd2->score += brd1->b.value;
        return -1;
    }

    /* Tie */
    if (brd2->p.y == brd1->p.y) return 0;

    return 0;
}

/*
 * Returns the platform instance ID with the least birds nearby
 */
int findFreePlat(birds brds, platforms p) {
    int tmpPlats[PLATFORMS], i, j, n = 0, good;
    point tmpP;
    size tmpS;

    /* Iterate all platforms */
    for (i = 0; i < p.l; i++) {
        /* Only consider visible platforms */
        if (!p.plt[i].outOfBounds) {
            good = 1;
            /* Check if any bird over the platform */
            for (j = 0; j< brds.l; j++) {
                /* Only consider living birds */
                if (brds.brd[j].lives > 0 && brds.brd[j].deathTime == -1) {
                    tmpP.x = p.plt[i].p.x;
                    tmpP.y = p.plt[i].p.y - brds.brd[j].b.o.s.height * 3;
                    tmpS.width = p.plt[i].o.s.width;
                    tmpS.height = brds.brd[j].b.o.s.height * 3 + 10;
                    if (areColliding(tmpP, tmpS, brds.brd[j].p, brds.brd[j].b.o.s)) good = 0;
                } else good = 0;
            }
            if (good) {
                tmpPlats[n] = p.plt[i].instanceID;
                n++;
            }
        }
    }
    return tmpPlats[rand() % (n - 1)];
}

/*
 * Calculates the next x and y coordinates for any bird using its properties only
 * Handles collisions (initiates joust for chars colliding with mobs)
 * Detects screen edge and moves bird accordingly
 */
void moveBird (bird *b, birds *brds, platforms p, objectTypes oT) {
    int tmpID, i, tmpVal, steps, done = 0;
    float angle;
    point increment;

    /* Test if the bird wasn't able to free itself in the last rounds and prevent getting stuck */
    if (b->lives > 0 && b->deathTime == -1 && (tmpID = platCollision(*b, p, 0)) != -1) {
        if (b->gotStuck > 10) {
            /* Move bird to top of platform */
            b->p.y = p.plt[tmpID].p.y - b->b.o.s.height - 1;
            b->gotStuck = 0;
        } else b->gotStuck += 1;
    } else b->gotStuck = 0;
    
    /* Dead players shouldn't move. But mobs should (as eggs) */
    if (b->lives > 0 && !(b->deathTime != -1 && !b->b.isMob)){

        /* Reset mob if cooldown ran out */
        if (b->deathTime != -1 && MLV_get_time() >= b->deathTime + MOBTIMEOUT * 1000) {
            b->deathTime = -1;
            b->hVel = b->b.hSpeed;
            b->lives = 2;
            b->b.o.s.width = oT.objT[b->b.o.objectID].s.width;
            b->b.o.s.height = oT.objT[b->b.o.objectID].s.height;
        }

        /* Apply gravity if bird is not on a platform */
        if(platCollision(*b, p, 2) == -1) {
            b->vVel -= b->b.vSpeed;
            b->onPlatform = 0;
        }

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
                /* Handle collision and get joust result */
                tmpVal = joust(b, &brds->brd[tmpID], oT);

                /* Bounce surviving bird(s) off the/each other */
                switch (tmpVal) {
                case 1:
                    b->hVel *= -1;
                    b->dir *= -1;
                    break;
                case 0:
                    /* Move second bird according to first ones direction to prevent getting stuck */
                    if (b->hVel > 0) brds->brd[tmpID].p.x = b->p.x + b->b.o.s.width + 5;
                    else brds->brd[tmpID].p.x = b->p.x - brds->brd[tmpID].b.o.s.width - 5;
                    /* Bounce both birds off each other */
                    brds->brd[tmpID].hVel *= -1;
                    brds->brd[tmpID].dir *= b->dir;
                    b->hVel *= -1;
                    b->dir *= -1;
                    break;
                case -1:
                    brds->brd[tmpID].hVel *= -1;
                    brds->brd[tmpID].dir *= -1;
                    b->p.x += increment.x; /* compensate for the next move */
                    break;
                case 2:
                default:
                    /* Do nothing */
                    break;
                }
                /* Go back one step and end progression */
                b->p.x -= increment.x;
                done = 1;
            }

            /* Test for collision with a platform */
            if ((tmpID = platCollision(*b, p, 0)) != -1) {
                /* Bounce off the other way */
                b->hVel *= -1;
                b->dir *= -1;
                b->p.x -= increment.x;
                done = 1;
            }
            
            /* If bird hasn't collided horizontally yet */
            if (!done) {
                /* Increment the Y axis second */
                b->p.y -= increment.y;

                /* Test for collision with another bird */
                if ((tmpID = birdCollision(*b, *brds)) != -1) { 
                    /* Handle collision and get joust result */
                    tmpVal = joust(b, &brds->brd[tmpID], oT);

                    /* Bounce surviving bird(s) off the/each other */
                    switch (tmpVal) {
                    case 1:
                        b->vVel *= -1;
                        break;
                    case 0:
                        /* Move second bird according to first ones direction to prevent getting stuck */
                        if (b->vVel > 0) brds->brd[tmpID].p.y = b->p.y - brds->brd[tmpID].b.o.s.height - 5;
                        else brds->brd[tmpID].p.y = b->p.y + b->b.o.s.height + 5;
                        /* Bounce both birds off each other */
                        b->vVel *= -1;
                        brds->brd[tmpID].vVel *= -1;
                        break;
                    case -1:
                        brds->brd[tmpID].vVel *= -1;
                        break;
                    case 2:
                    default:
                        /* Do nothing */
                        break;
                    }
                    /* Go back one step (if no dead) and end progression */
                    if (tmpVal != -1) b->p.y += increment.y;
                    done = 1;
                }

                /* Test for collision with a platform */
                if ((tmpID = platCollision(*b, p, 0)) != -1) {
                    /* Act according to side (top or bottom) */
                    /* Bird bumps its head on platform */
                    if (increment.y >= 0) {
                        b->vVel *= -1;
                    } else { /* Bird lands on platform */
                        b->p.y = p.plt[tmpID].p.y - b->b.o.s.height;
                        b->vVel = 0;
                        b->onPlatform = 1;
                    }
                    b->p.y += increment.y;
                    done = 1;
                }
            }
        }

        /* Bumping top of screen */
        if (b->p.y < 0 && b->lives > 0 && b->deathTime == -1) {
            b->p.y = 0;
            b->vVel = -b->vVel;
        }
        /* Bumping bottom of screen */
        if (b->p.y + b->b.o.s.height > SCREENHEIGHT && b->lives > 0 && b->deathTime == -1) {
            b->p.y = SCREENHEIGHT - b->b.o.s.height;
            b->vVel = 0; /* -b->vVel is a LOT more fun */
        }
        /* Bumping left side of screen */
        if (b->p.x < -b->b.o.s.width / 2 && b->lives > 0 && b->deathTime == -1) {
            b->p.x = SCREENWIDTH - b->b.o.s.width / 2;
        }
        /* Bumping right side of screen */
        if (b->p.x > SCREENWIDTH - b->b.o.s.width / 2 && b->lives > 0 && b->deathTime == -1) {
            b->p.x = -b->b.o.s.width / 2;
        }

        /* Cap maximal velocities */
        if (b->hVel > MAXVEL) b->hVel = MAXVEL;
        if (b->vVel > MAXVEL) b->vVel = MAXVEL;
        if (b->hVel < -MAXVEL) b->hVel = -MAXVEL;
        if (b->vVel < -MAXVEL) b->vVel = -MAXVEL;
    
    /* Respawn player after cooldown */
    } else if (!b->b.isMob) {
        /* Prevent respawn if no lives left, otherwise check for time since death */
        if (b->lives > 0 && MLV_get_time() >= b->deathTime + PLYRTIMEOUT * 1000) {
            /* Find a free platform */
            tmpID = findFreePlat(*brds, p);
            /* Move bird to that platform */   
            b->p.x = p.plt[tmpID].p.x + (p.plt[tmpID].o.s.width / 2) - (b->b.o.s.width / 2);
            b->p.y = p.plt[tmpID].p.y - b->b.o.s.height;
            b->deathTime = -1;
        }
    }
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

    /* Cap maximal velocities */
    if (b->hVel > MAXVEL) b->hVel = MAXVEL;
    if (b->vVel > MAXVEL) b->vVel = MAXVEL;
    if (b->hVel < -MAXVEL) b->hVel = -MAXVEL;
    if (b->vVel < -MAXVEL) b->vVel = -MAXVEL;
}

/*
 * Updates a mobs position using "AI"
 * Detects potential obstacles and players
 * Either flies straight with small variations or towards the player, according to the aggressivity percentage
 */
void updateMobPos (bird *b, birds brd, platforms plt){
    if(b->b.aggressiveness == 1) moveAggIa(b, brd);
    else movePassiveIa(b);

    /* Cap maximal velocities*/
    if (b->hVel > MAXVEL) b->hVel = MAXVEL;
    if (b->vVel > MAXVEL) b->vVel = MAXVEL;
    if (b->hVel < -MAXVEL) b->hVel = -MAXVEL;
    if (b->vVel < -MAXVEL) b->vVel = -MAXVEL;
}

/*
 * Updates the positions of all bird instances 
 */ 
void updatePos (birds *brds, platforms plts, objectTypes oT) {
    int i;
    
    for (i = 0; i < brds->l; i++) {
        if (brds->brd[i].b.isMob) updateMobPos(&brds->brd[i], *brds, plts);
        else updateCharPos(&brds->brd[i]);
        moveBird(&brds->brd[i], brds, plts, oT);
    }
}