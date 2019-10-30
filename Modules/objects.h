#ifndef OBJECTS
#define OBJECTS

#include <stdlib.h>
#include <MLV/MLV_all.h>

#define OBJS 20
#define BIRDTYPES 20
#define PLATFORMS 6
#define MAXINSTANCES 50

typedef struct {
    int typeID; /* Positive */
    char spritePath[50];
    char name[20];
    int width;
    int height;
} objectType; /* Platfoms or birds */

typedef struct {
    objectType o;
    float runSpeed;
    float glideSpeed;
    float flapStrength; /* Height reached with one flap press */
    int respawnTime; /* Seconds before the bird respawns (mob: egg, character: platform) */
} birdType; /* Mobs or characters */

typedef struct {
    birdType b;
    int instanceID; /* Positive */
    int x;
    int y;
    int dir; /* Facing left: -1; right: 1 */
    int flaps; /* Amount of flap presses */
    int isMob;
    int player; /* Player 1's or 2's bird. -1 if it's a mob */
} bird; /* Instance of a bird type */

typedef struct {
    objectType o;
    int instanceID;
    int x;
    int y;
} platform; /* Platform instance */

extern objectType objT[OBJS];
extern birdType brdT[BIRDTYPES];
extern bird brd[MAXINSTANCES];
extern platform plt[PLATFORMS];

/*
 * Initializes array objT with predefined object types 
 * Returns 1 on successful import, 0 otherwise
 */ 
int importOBJs (objectType oType[OBJS], char *filePath);

/*
 * Initializes array brdT with predefined bird types
 * Returns 1 on successful import, 0 otherwise
 */ 
int importBirdTypes (birdType bType[OBJS], char *filePath);

/*
 * Initializes array plt with predefined platforms
 * Returns 1 on successful import, 0 otherwise
 */
int importPlatforms (platform plat[PLATFORMS], char *filePath);

#endif