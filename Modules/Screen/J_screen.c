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
		if (vie <= 0) vspfunc(player1, "Joueur 1 : MORT \nScore : %d", score);
		else vspfunc(player1, "Joueur 1 : %d vies \nScore : %d", vie, score);
		MLV_draw_adapted_text_box_with_font(
			10, 10,
			player1, font,taille_interligne,
			MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			MLV_TEXT_LEFT
		);
	} else if (nbjr == 2){
		if (vie <= 0) vspfunc(player2, "Joueur 2 : DEAD \nScore : %d", score);
		else vspfunc(player2, "Joueur 2 : %d vies \nScore : %d", vie, score);
		MLV_get_size_of_text("Joueur 2 : 3 vies        ", &widthtxt, &heightxt);
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
	MLV_Font* font = MLV_load_font( "Data/Fonts/BebasNeue-Regular.ttf" , 60);
	MLV_draw_text_box_with_font( 
		0,0,
		SCREENWIDTH,SCREENHEIGHT,
		msg, font, 9,
		MLV_ALPHA_TRANSPARENT, MLV_COLOR_WHITE, MLV_ALPHA_TRANSPARENT,
		MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
		);
	MLV_free_font(font);
}

/*
 * Draw the background
 */
void dispClear(int isBlack, MLV_Image *bg){
	if (isBlack) MLV_draw_filled_rectangle(0, 0, SCREENWIDTH, SCREENHEIGHT, MLV_COLOR_BLACK);
	else MLV_draw_image(bg, 0, 0);
}

/*
 * Displays the current frame according to the games state
 */
void dispFrame(platforms p, birds b, objectTypes oTs, char msg[MAXMSGCHARS], MLV_Image *bg){
	dispClear(0, bg);
	dispPlats(p);
	dispBirds(b, oTs);
	if(strcmp("", msg) != 0) dispText(msg);
}

/*
 * Displays a menu with the image given as parameter
 * returns the number of player, 3 if the display highscore button has been clicked or 0 if the user wan't to quit
 */
int dispMenu(char *filepath){
	MLV_Font* font = MLV_load_font( "Data/Fonts/BebasNeue-Regular.ttf" , 30);
	MLV_Image *imageMenu;
	MLV_Event event = MLV_get_event(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	int x = 0, y = 0, ok = 0, nbrj = 1, compteur = 1; 
	char text[25];
	/* Fond du menu */
	if ((imageMenu = loadImage(filepath)) == 0) return 0;
	/* Bouttons */
	while(ok == 0){
		vspfunc(text, "Nombre de joueurs : %d", nbrj);
		dispClear(1, NULL);
		MLV_draw_image(imageMenu, SCREENWIDTH / 2 - MLV_get_image_width(imageMenu) / 2,0);
		/*Displaying 4 "Buttons" to act as a menu*/
		MLV_draw_text_box_with_font(
			(SCREENWIDTH * 0.2), (SCREENHEIGHT * 0.51),
			(SCREENWIDTH * 0.6), (SCREENHEIGHT * 0.08),
			"Démarrer la partie", font, 9,
			MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
		);
		MLV_draw_text_box_with_font(
			(SCREENWIDTH * 0.2), (SCREENHEIGHT * 0.61),
			(SCREENWIDTH * 0.6), (SCREENHEIGHT * 0.08),
			text, font, 9,
			MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
		);
		MLV_draw_text_box_with_font(
			(SCREENWIDTH * 0.2), (SCREENHEIGHT * 0.71),
			(SCREENWIDTH * 0.6), (SCREENHEIGHT * 0.08),
			"Sauvegarder une partie", font, 9,
			MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
		);
		MLV_draw_text_box_with_font(
			(SCREENWIDTH * 0.2), (SCREENHEIGHT * 0.81),
			(SCREENWIDTH * 0.6), (SCREENHEIGHT * 0.08),
			"Charger une parie", font, 9,
			MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
		);
		MLV_draw_text_box_with_font(
			(SCREENWIDTH * 0.2), (SCREENHEIGHT * 0.91),
			(SCREENWIDTH * 0.6), (SCREENHEIGHT * 0.08),
			"Afficher les meilleurs scores", font, 9,
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
		} while(event != MLV_MOUSE_BUTTON); /*When you have a event that is MLV_MOUSE_BUTTON stop the while loop*/
		/*Detect Where the user clicked on the screen and act accordingly*/
		if (x > (SCREENWIDTH * 0.2) && x < (SCREENWIDTH * 0.8) && y > (SCREENHEIGHT * 0.51)
			&& y < (SCREENHEIGHT * 0.59)){
			return nbrj;
		}else if (x > (SCREENWIDTH * 0.2) && x < (SCREENWIDTH * 0.8) && y > (SCREENHEIGHT * 0.61)
			&& y < (SCREENHEIGHT * 0.69)){
			if(nbrj == 1 && compteur == 1){
				nbrj = 2;
			}else if(compteur == 1) nbrj = 1;
			if(compteur == 1){
				compteur = 2;
			}else compteur = 1;
		}else if (x > (SCREENWIDTH * 0.2) && x < (SCREENWIDTH * 0.8) && y > (SCREENHEIGHT * 0.71)
			&& y < (SCREENHEIGHT * 0.79)){
			return 4;
		}else if (x > (SCREENWIDTH * 0.2) && x < (SCREENWIDTH * 0.8) && y > (SCREENHEIGHT * 0.81)
			&& y < (SCREENHEIGHT * 0.89)){
			return 5;
		}else if (x > (SCREENWIDTH * 0.2) && x < (SCREENWIDTH * 0.8) && y > (SCREENHEIGHT * 0.91)
			&& y < (SCREENHEIGHT * 0.99)){
			return 3;
		}
	}	
	return nbrj;
	MLV_free_font(font);
}
int dispScore(){
	FILE *f;
	MLV_Font* font = MLV_load_font( "Data/Fonts/BebasNeue-Regular.ttf" , 30);
	int MeilleursScores[10], tmpScore, i, j, nbrScore=0;
	char MeilleursScoresNom[10][10], tmpName[10], msg[100];
	/*Initialising the two list to prevent random number inside*/
	for(i=0; i<10; i++) MeilleursScores[i] = 0; 
	for(i=0; i<10; i++) strcpy(MeilleursScoresNom[i], ""); 
	if((f = fopen("Data/Files/score.txt", "r")) == NULL){
		printf("Erreur dans l'ouverture du fichier score.txt !\n");
		return 0;
	}else{
		while(fscanf(f, "%s %d", tmpName, &tmpScore) == 2){ /*While it can read a lign in the .txt*/
			if(nbrScore < 10) nbrScore ++; /*Used to know if there is less than 10 score saved*/
			for(i=0;i<10;i++){
				if(tmpScore > MeilleursScores[9-i]){
					for(j=1;j<=9-i;j++){
						MeilleursScores[j-1] = MeilleursScores[j];
						strncpy(MeilleursScoresNom[j-1], MeilleursScoresNom[j], 10);
					}
					MeilleursScores[9-i] = tmpScore;
					strncpy(MeilleursScoresNom[9-i], tmpName, 10);
					i = 10;
				}
			}
		}
		if(nbrScore == 0) dispText("Aucun score sauvegardé pour le moment\n Commencez vite une partie !");
		else dispClear(1, NULL);
		/*Display the top 10 scores (or all of them if there are less than 10)*/
		for(i=0;i<nbrScore;i++){
			vspfunc(msg, "%s %d", MeilleursScoresNom[9-i], MeilleursScores[9-i]);
			MLV_draw_text_box_with_font(
			(SCREENWIDTH * 0.2), (SCREENHEIGHT * i * 0.1),
			(SCREENWIDTH * 0.6), (SCREENHEIGHT * 0.1),
			msg, font, 9,
			MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
		);
		}
	}
	MLV_actualise_window();
	MLV_wait_keyboard(NULL, NULL, NULL);
	return 1;
}

void dispAskScore(birds b){
	MLV_Font* font = MLV_load_font( "Data/Fonts/BebasNeue-Regular.ttf" , 30);
	char *name;
	/*Utilise une boite de saisie afin de récuperer le nom du joueur*/
	MLV_wait_input_box_with_font(
			(SCREENWIDTH * 0.2), (SCREENHEIGHT * 0.45),
			(SCREENWIDTH * 0.6), (SCREENHEIGHT * 0.1),
			MLV_COLOR_BLUE, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			"Please enter your name (10 char max):",
			&name, font
		);
	saveScore(b.brd[0].score, name);
}