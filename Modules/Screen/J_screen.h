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
 * Displays the menu with the image given as parameter
 * returns the number of player or 3 if the HIGHSCORE has been clicked
 */
int dispMenu();

/*
 * Clear the display by putting a black box on top of it
 */
int dispClear();

/*
 * Display the top 10 Highest score with name
 */
void dispScore();

#endif