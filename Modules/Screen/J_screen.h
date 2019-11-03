#ifndef SCREEN
#define SCREEN

#include <stdlib.h>
#include <MLV/MLV_all.h>

#include "../Objects/J_objects.h"

#define MAXMSGCHARS 200
#define MAXMENUCHARS 50

/*
 * Displays all platforms
 */
void dispPlats(platform plt[PLATFORMS], int n);

/*
 * Displays all birds
 */
void dispBirds(bird brd[MAXINSTANCES], int n);

/*
 * Displays both players lives and score
 */
void dispStatus(bird brd[MAXINSTANCES], int n);

/*
 * Displays the message passed as parameter
 */
void dispText(char msg[MAXMSGCHARS], int length);

/*
 * Displays the current frame according to the games state
 */
void dispFrame(platform plt[PLATFORMS], int n, bird brd[MAXINSTANCES], int m);

/*
 * Displays a menu with buttons passed as parameters
 * Returns the index of the clicked button
 * Can display an image if given a valid path
 */
int dispMenu(char *button[MAXMENUCHARS], char *imgPath);

#endif