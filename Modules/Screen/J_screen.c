#include "J_screen.h"

/*
 * Create a formated string and put it in a char * varable printf style
 */
static int vspfunc(char *str, char *format, ...){
	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(str, format, aptr); 
	va_end(aptr);

	return ret;
}

/*
 * Displays all platforms
 */
void dispPlats(platforms p){
	int i = 0;
	for(i=0;i<p.l;i++){
		MLV_draw_image(p.plt[i].o.sprite, p.plt[i].p.x, p.plt[i].p.y);
		/*MLV_draw_text(p.plt[i].p.x, p.plt[i].p.y, "%d", MLV_COLOR_WHITE, p.plt[i].instanceID);*/
	}
}

/*
 * Displays all birds
 */
void dispBirds(birds bird, objectTypes oTs){
	int i = 0, j;
	for(i=0;i<bird.l;i++){
		/*On charge l'image qui correspond a notre oiseau*/	

		if(!bird.brd[i].b.isMob) {
			/* Display a players status */
			dispStatus(bird.brd[i].player, bird.brd[i].score, bird.brd[i].lives);
		}

		if (bird.brd[i].b.isMob && bird.brd[i].deathTime != -1) {
			/* Display egg instead of bird. Find egg object */
			j = 0;
			while (j < oTs.l && oTs.objT[j].objectID != 5) j++;
			MLV_draw_image(oTs.objT[j].sprite, bird.brd[i].p.x, bird.brd[i].p.y);
		} else {
			/* Display a bird */
			if (bird.brd[i].dir == 1) MLV_vertical_image_mirror(bird.brd[i].b.o.sprite);
			MLV_draw_image(bird.brd[i].b.o.sprite, bird.brd[i].p.x, bird.brd[i].p.y);
			if (bird.brd[i].dir == 1) MLV_vertical_image_mirror(bird.brd[i].b.o.sprite);
		}
	}
}

/*
 * Displays both players lives and score
 */

void dispStatus(int nbjr, int score, int vie){
	int taille_interligne = 9, widthtxt, heightxt;
	char player1[50], player2[50];
	MLV_Font* font = MLV_load_font( "Data/Fonts/BebasNeue-Regular.ttf" , 20 );
	
	if(nbjr == 1){
		if (vie <= 0) vspfunc(player1, "Player 1 : DEAD \nScore : %d", score);
		else vspfunc(player1, "Player 1 : %d lives \nScore : %d", vie, score);
		MLV_draw_adapted_text_box_with_font(
			10, 10,
			player1, font,taille_interligne,
			MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			MLV_TEXT_LEFT
		);
	} else if (nbjr == 2){
		if (vie <= 0) vspfunc(player2, "Player 2 : DEAD \nScore : %d", score);
		else vspfunc(player2, "Player 2 : %d lives \nScore : %d", vie, score);
		MLV_get_size_of_text("Player 2 : 3 lives        ", &widthtxt, &heightxt);
		MLV_draw_adapted_text_box_with_font(
			(SCREENWIDTH - widthtxt), 10,
			player2, font, taille_interligne,
			MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			MLV_TEXT_LEFT
		);
	}
	MLV_free_font(font);
}

/*
 * Displays the message passed as parameter
 * Pour ce faire on crée une boite de texte qui recouvre tout l'écran avec
 * un message centré dans la boite (passé en paramètre)
 */
void dispText(char msg[MAXMSGCHARS]){
	MLV_Font* font = MLV_load_font( "Data/Fonts/BebasNeue-Regular.ttf" , 30);
	MLV_draw_text_box_with_font( 
		0,0,
		SCREENWIDTH,SCREENHEIGHT,
		msg, font, 9,
		MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
		MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
		);
	MLV_free_font(font);
}

/*
 * Clear the display by putting a black box on top of it
 */
void dispClear(){
	MLV_draw_filled_rectangle(0,0,SCREENWIDTH,SCREENHEIGHT,MLV_COLOR_BLACK);
}

/*
 * Displays the current frame according to the games state
 */
void dispFrame(platforms p, birds b, objectTypes oTs){
	dispClear();
	dispPlats(p);
	dispBirds(b, oTs);
}

/*
 * Displays a menu with the image given as parameter
 * returns the number of player or 3 if the HIGHSCORE has been clicked
 */
int dispMenu(char *filepath){
	MLV_Font* font = MLV_load_font( "Data/Fonts/BebasNeue-Regular.ttf" , 30);
	MLV_Image *imageMenu;
	MLV_Event event;
	int x = 0, y = 0, ok = 0, nbrj = 1, compteur = 1; 
	char text[25];
	/* Fond du menu */
	if ((imageMenu = loadImage(filepath)) == 0) return 0;
	/* Bouttons */
	while(ok == 0){
		vspfunc(text, "Nombre de joueurs : %d", nbrj);
		dispClear();
		MLV_draw_image(imageMenu, 0,0);
		MLV_draw_text_box_with_font(
			(SCREENWIDTH * 0.2), (SCREENHEIGHT * 0.52),
			(SCREENWIDTH * 0.6), (SCREENHEIGHT * 0.1),
			"Démarrer la partie", font, 9,
			MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
		);
		MLV_draw_text_box_with_font(
			(SCREENWIDTH * 0.2), (SCREENHEIGHT * 0.64),
			(SCREENWIDTH * 0.6), (SCREENHEIGHT * 0.1),
			text, font, 9,
			MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
		);
		MLV_draw_text_box_with_font(
			(SCREENWIDTH * 0.2), (SCREENHEIGHT * 0.76),
			(SCREENWIDTH * 0.6), (SCREENHEIGHT * 0.1),
			"Afficher les meilleurs scores", font, 9,
			MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
		);
		MLV_draw_text_box_with_font(
			(SCREENWIDTH * 0.2), (SCREENHEIGHT * 0.88),
			(SCREENWIDTH * 0.6), (SCREENHEIGHT * 0.1),
			"Quitter le jeu ", font, 9,
			MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
		);
		MLV_actualise_window();
		do{
			event = MLV_get_event( /*Récupère un événement*/
				NULL, NULL, NULL,
				NULL, NULL,
				&x, &y, NULL,
				NULL
				);
		} while(event != MLV_MOUSE_BUTTON);
		if (x > (SCREENWIDTH * 0.2) && x < (SCREENWIDTH * 0.8) && y > (SCREENHEIGHT * 0.52)
			&& y < (SCREENHEIGHT * 0.62)){
			return nbrj;
		}else if (x > (SCREENWIDTH * 0.2) && x < (SCREENWIDTH * 0.8) && y > (SCREENHEIGHT * 0.64)
			&& y < (SCREENHEIGHT * 0.74)){
			if(nbrj == 1 && compteur == 1){
				nbrj = 2;
			}else if(compteur == 1) nbrj = 1;
			if(compteur == 1){
				compteur = 2;
			}else compteur = 1;
		}else if (x > (SCREENWIDTH * 0.2) && x < (SCREENWIDTH * 0.8) && y > (SCREENHEIGHT * 0.76)
			&& y < (SCREENHEIGHT * 0.86)){
			return 3;
		}else if (x > (SCREENWIDTH * 0.2) && x < (SCREENWIDTH * 0.8) && y > (SCREENHEIGHT * 0.88)
			&& y < (SCREENHEIGHT * 0.98)){
			return 0;
		}
	}	
	return nbrj;
	MLV_free_font(font);
}