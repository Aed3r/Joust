#ifndef SCREEN
#define SCREEN

#include <stdlib.h>
#include <MLV/MLV_all.h>

#include "../Objects/J_objects.h"

#define MAXMSGCHARS 200
#define MAXMENUCHARS 50
#define SCREENSIZE 900

/*
 * Displays all platforms
 */
void dispPlats(platforms p, int n);

/*
 * Displays all birds
 */
void dispBirds(birds bird, int n);

/*
 * Displays both players lives and score
 */
void dispStatus(int nbjr, int score1, int score2, int vie1, int vie2);

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