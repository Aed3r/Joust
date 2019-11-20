#include <MLV/MLV_all.h>
#include <string.h>
#include "J_screen.h"

/*
 * Displays all platforms
 */
void dispPlats(platforms p, int n){
	int i = 0;
	MLV_Image *image;
	for(i=0;i<platforms.l;i++){
		image = MLV_load_image(p.plt[i].o.spriteName); /*Charge l'image qui correspond a celle de notre Plateforme*/
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
		image = MLV_load_image(bird.brd[i].b.o.spriteName);/*On charge l'image qui correspond a notre oiseau*/
		MLV_draw_image(image, bird.brd[i].p.x, bird.brd[i].p.y);
	}
}

/*
 * Displays both players lives and score
 */
void dispStatus(int nbjr, int score1, int score2, int vie1, int vie2){
	int taille_interligne = 9;
	char *player1, *player2;
	asprintf(%player1, "PLayer 1 : %d life \nScore : %d",vie1,score1);
	asprintf(%player2, "PLayer 2 : %d life \nScore : %d",vie2,score2);
	MLV_draw_adapted_text_box(
		10, 10,
		player1, taille_interligne,
		MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
		MLV_TEXT_LEFT
		);

}
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