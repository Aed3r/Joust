#ifndef OBJECTS
#define OBJECTS

#include <stdlib.h>
#include <stdio.h>
#include <MLV/MLV_all.h>

#include "../values.h"

typedef struct {
    float x;
    float y;
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
    float hSpeed; /* Horizontal Speed */
    float vSpeed; /* Vertical Speed */
    float flapStrength; /* Height reached with one flap press. TODO: useless now */
    int isMob; /* 0 = Player, 1 = Mob*/
    int aggressiveness; /* AI: 0 is passive 1 is aggressive*/
    int value; /* What score increment the player gets by killing this bird */
} birdType; /* Mobs or characters */

typedef struct {
    birdType b;
    int instanceID; /* Positive, automatically set */
    point p;
    int dir; /* Facing left: -1; right: 1 */
    float hVel; /* Horizontal velocity */
    float vVel; /* Vertical velocity */
    int player; /* Player 1's or 2's bird. -1 if it's a mob */
    int lives; /* Bird lives if they are a player */
    int score; /* Player score */
    int deathTime; /* Time of last death. Initializes to -1 */
    int flapped; /* 1: player just pressed flap, 0 otherwise */
    int gotStuck; /* 1: player was stuck in platform last frame, 0 otherwise. Failsafe */
    int onPlatform; /* Indicates when bird, well, is on platform... */
} bird; /* Instance of a bird type */

typedef struct {
    objectType o;
    int instanceID;
    point p;
    int outOfBounds; /* Wether or not the platform is outside the screen */
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

int saveScore (int score, char *name);

#endif
