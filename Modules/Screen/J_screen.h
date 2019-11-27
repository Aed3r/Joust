#ifndef SCREEN
#define SCREEN

#include <stdio.h>
#include <stdarg.h>
#include <MLV/MLV_all.h>

#include "../Objects/J_objects.h"
#include "../values.h"

/*
 * Displays all platforms
 */
void dispPlats(platforms p);

/*
 * Displays all birds
 */
void dispBirds(birds bird);

/*
 * Displays both players lives and score
 */
void dispStatus(int nbjr, int score1, int score2, int vie1, int vie2);

/*
 * Displays the message passed as parameter
 */
void dispText(char msg[MAXMSGCHARS]);

/*
 * Displays the current frame according to the games state
 */
void dispFrame(platforms p, birds b, char statlist[5]);

/*
 * Displays a menu with buttons passed as parameters
 * Returns the index of the clicked button
 * Can display an image if given a valid path
 */
int dispMenu(char *button[MAXMENUCHARS], char *imgPath);

/*
 * Clear the display by putting a black box on top of it
 */
int dispClear();

#endif