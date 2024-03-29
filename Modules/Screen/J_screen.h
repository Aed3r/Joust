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
void dispBirds(birds bird, objectTypes oTs);

/*
 * Displays both players lives and score
 */
void dispStatus(int nbjr, int score, int vie);

/*
 * Displays the message passed as parameter
 */
void dispText(char msg[MAXMSGCHARS]);

/*
 * Displays the current frame according to the games state
 */
void dispFrame(platforms p, birds b, objectTypes oTs, char msg[MAXMSGCHARS], MLV_Image *bg);

/*
 * Displays the menu with the image given as parameter
 * returns the number of player or 3 if the HIGHSCORE has been clicked
 */
int dispMenu();

/*
 * Draw the background
 */
void dispClear(int isBlack, MLV_Image *bg);

/*
 * Display the top 10 Highest score with name
 */
int dispScore();

/*
 * Ask for the name of the best player in the game in order to save its score
 */
void dispAskScore();

#endif