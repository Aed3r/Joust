#ifndef CONTROLS
#define CONTROLS

#include <stdlib.h>
#include <math.h>
#include <MLV/MLV_time.h>

#include "../Objects/J_objects.h"
#include "../values.h"
#include "../IA/J_ia.h"

/*
 * Creates a new birdType instance using object ID
 * Returns the length of the birds array
 */ 
int spawnBird (int oID, birdTypes bt, birds *b, int x, int y, int dir, int player);

/*
 * Creates a new platform instance using the passed ID
 * Returns the length of the platforms array
 */ 
int createPlatform (int oID, objectTypes ot, platforms *p, int x, int y, int outOfBounds);

/*
 * Returns 1 if two rectangles defined by (p1, s1) and (p2, s2) collide
 */
int areColliding (point p1, size s1, point p2, size s2);


/*
 * Handle a birds parameters at their death 
 */
void handleDeath (bird *b);

/*
 * Returns the instance ID of any platform colliding with the bird passed as param
 * Returns -1 otherwise
* yOffset lets you test for relative positions
 */
int platCollision (bird b, platforms p, int yOffset);

/*
 * Returns the instance ID of any bird colliding with the bird passed as param
 * Returns -1 otherwise
 */
int birdCollision (bird b, birds brds);

/*
 * Handles the collision of two birds. Returns :
 * 1 if brd1 is the winner
 * 0 if there's no winner
 * -1 if brd2 is the winner
 */
int joust(bird *brd1, bird *brd2);

/*
 * Returns the platform instance ID with the least birds nearby
 */
int findFreePlat(birds brds, platforms p);

/*
 * Calculates the next x and y coordinates for any bird only using bird properties
 * Detects collisions (initiates joust for chars colliding with mobs)
 * Detects screen edge and moves bird accordingly
 */
void moveBird (bird *b, birds *brds, platforms p);

/*
 * Updates the position of a player controlled bird
 */
void updateCharPos(bird *b);

/*
 * Updates a mobs position using "AI"
 * Detects potential obstacles and players
 * Either flies straight with small variations or towards the player, according to the aggressivity percentage
 */
void updateMobPos (bird *b, birds brd, platforms plt);

/*
 * Updates the positions of all bird instances 
 */ 
void updatePos (birds *brds, platforms plts);

#endif