#include <MLV/MLV_all.h>
#include "J_screen.h"

/*
 * Displays all platforms
 */
void dispPlats(platforms p, int n){
	int i = 0;
	MLV_Image *image;
	for(i=0;i<bird.l;i++){
		image = MLV_load_image(p.plt[i].o.spritename);
		MLV_draw_image(image, p.plt[i].p.x, p.plt[i].p.y);
	}
}

/*
 * Displays all birds
 */
void dispBirds(birds bird, int n){
	int i = 0;
	MLV_Image *image;
	for(i=0;i<bird.l;i++){
		image = MLV_load_image(bird.brd[i].b.o.spritename);
		MLV_draw_image(image, bird.brd[i].p.x, bird.brd[i].p.y);
	}
}

/*
 * Displays both players lives and score
 */
//void dispStatus(bird brd[MAXINSTANCES], int n);

/*
 * Displays the message passed as parameter
 */
//void dispText(char msg[MAXMSGCHARS], int length);

/*
 * Displays the current frame according to the games state
 */
//void dispFrame(platform plt[PLATFORMS], int n, bird brd[MAXINSTANCES], int m);

/*
 * Displays a menu with buttons passed as parameters
 * Returns the index of the clicked button
 * Can display an image if given a valid path
 */
//int dispMenu(char *button[MAXMENUCHARS], char *imgPath);