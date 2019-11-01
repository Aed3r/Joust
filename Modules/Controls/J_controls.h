#ifndef CONTROLS
#define CONTROLS

#include <stdlib.h>

#include "../Objects/J_objects.h"

/*
 * Creates a new birdType instance using object ID
 * Returns the length of the birds array
 */ 
int spawnBird (int oID, birdType brdT[BIRDTYPES], int n, bird birds[MAXINSTANCES], int m, int x, int y, int dir, int player);

/*
 * Creates a new platform instance using the passed ID
 * Returns the length of the platforms array
 */ 
int createPlatform (int oID, objectType objT[OBJS], int n, int x, int y);

/*
 * Returns 1 if two rectangles defined by (p1, s1) and (p2, s2) collide
 */
int areColliding (point p1, size s1, point p2, size s2);

/*
 * Returns the instance ID of any platform colliding with the bird passed as param
 * Returns -1 otherwise
 */
int platCollision (bird *b, platform plt[PLATFORMS], int n);

/*
 * Returns the instance ID of any bird colliding with the bird passed as param
 * Returns -1 otherwise
 */
int birdCollision (bird *b, bird brd[MAXINSTANCES], int n);

/*
 * Returns the result of a joust:
 * 1 if brd1 is the winner
 * 0 for for a tie
 * -1 if brd2 is the winner
 */
int joust(bird *brd1, bird *brd2);

/*
 * Calculates the next x and y coordinates for any bird only using bird properties
 * Detects collisions (initiates joust for chars colliding with mobs)
 * Detects screen edge and moves bird accordingly (TODO: add header ref for screen size)
 */
void moveBird(bird *b, bird brd[MAXINSTANCES], platform plt[PLATFORMS]);

/*
 * Updates the position of a player controlled bird
 */
void updateCharPos(bird *b, bird brd[MAXINSTANCES], platform plt[PLATFORMS]);

/*
 * Updates a mobs position using "AI"
 * Detects potential obstacles and players
 * Either flies straight with small variations or towards the player, according to the aggressivity percentage
 */
void updateMobPos(bird *b, bird brd[MAXINSTANCES], platform plt[PLATFORMS]);

/*
 * Updates the positions of all bird instances 
 */ 
void updatePos(bird brd[MAXINSTANCES]);

#endif