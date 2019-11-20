#ifndef OBJECTS
#define OBJECTS

#include <stdlib.h>

#define OBJS 20
#define BIRDTYPES 20
#define PLATFORMS 6
#define MAXINSTANCES 50

typedef struct {
    int x;
    int y;
} point;

typedef struct {
    int width;
    int height;
} size;

typedef struct {
    int objectID; /* Positive */
    char spriteName[50];
    char name[20];
    size s;
} objectType; /* Platfom or bird types */

typedef struct {
    objectType o;
    float runSpeed;
    float glideSpeed;
    float flapStrength; /* Height reached with one flap press */
    int respawnTime; /* Seconds before the bird respawns (mob: egg, character: platform) */
    int isMob; /*0 = Player, 1 = Mob*/
    int aggressiveness; /* AI: percentage indicating likeliness to attack player and avoid platforms. 100 for the pterodactyl */
} birdType; /* Mobs or characters */

typedef struct {
    birdType b;
    int instanceID; /* Positive */
    point p;
    int dir; /* Facing left: -1; right: 1 */
    int velY; /* Vertical velocity */
    int player; /* Player 1's or 2's bird. -1 if it's a mob */
} bird; /* Instance of a bird type */

typedef struct {
    birdType p;
    int creationTime;
} egg;

typedef struct {
    objectType o;
    int instanceID;
    point p;
} platform; /* Platform instance */

/* Structs used to keep track of array sizes */

typedef struct {
    birdType brdT[BIRDTYPES];
    int l;
}birdTypes;

typedef struct {
    bird brd[MAXINSTANCES];
    int l;
}birds;

typedef struct {
    objectType objT[OBJS];
    int l;
}objectTypes;

typedef struct{
    platform plt[PLATFORMS];
    int l;
}platforms;

/*
 * Initializes array objT with predefined object types 
 * Returns 1 on successful import, 0 otherwise
 */ 
int importOBJs (objectTypes *o, char *filePath);

/*
 * Initializes array brdT with predefined bird types
 * Returns 1 on successful import, 0 otherwise
 */ 
int importBirdTypes (birdTypes *b, objectTypes *o, char *filePath);

#endif
