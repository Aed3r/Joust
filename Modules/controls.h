#ifndef CONTROLS
#define CONTROLS

#include <stdlib.h>
#include <MLV/MLV_all.h>

#include "objects.h"

/*
 * Creates and initializes a new birdType instance using its object ID
 */ 
void spawnBird(int oID, birdType brdT[BIRDTYPES], bird birds[MAXINSTANCES], int x, int y);

//todo add collision detecteion function between two boxes


/*
 * Returns the instance ID of any platform colliding with the bird passed as param
 * Returns -1 otherwise
 */
int platCollision(bird *b, platform plt[PLATFORMS]);

/*
 * Returns the instance ID of any bird colliding with the bird passed as param
 * Returns -1 otherwise
 */
int birdCollision(bird *b, bird brd[MAXINSTANCES]);

/*
 * Returns the result of a joust:
 * 1 if brd1 is the winner
 * 0 for for a tie
 * -1 if brd2 is the winner
 */
int joust(bird *brd1, bird *brd2);

/*
 * Updates the position of a player controlled bird
 */
void updateCharPos(bird *b);

/*
 * Updates a mobs position
 */
void updateMobPos(bird *b);

/*
 * Updates the positions of all bird instances 
 */ 
void updatePos(bird brd[MAXINSTANCES]);

#endif