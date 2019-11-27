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
 * Loads the image with the given fileName
 */
MLV_Image* loadImage(char *fileName) {
	MLV_Image *tmpImg;
	FILE *file;
	char path[100];

	strcpy(path, "Data/Sprites/");
	strcat(path, fileName);

    if ((file = fopen(path, "r"))) {
        fclose(file);
        tmpImg = MLV_load_image(path);
		return tmpImg;
    }
    return 0;
}

/*
 * Displays all platforms
 */
void dispPlats(platforms p){
	int i = 0;
	MLV_Image *image;
	for(i=0;i<p.l;i++){
		image = loadImage(p.plt[i].o.spriteName); /*Charge l'image qui correspond a celle de notre Plateforme*/
		MLV_draw_image(image, p.plt[i].p.x, p.plt[i].p.y);
		MLV_free_image(image);
	}
}

/*
 * Displays all birds
 */
void dispBirds(birds bird){
	int i = 0;
	MLV_Image *image;
	for(i=0;i<bird.l;i++){
		image = loadImage(bird.brd[i].b.o.spriteName); /*On charge l'image qui correspond a notre oiseau*/
		MLV_draw_image(image, bird.brd[i].p.x, bird.brd[i].p.y);
		MLV_free_image(image);
	}
}

/*
 * Displays both players lives and score
 */

void dispStatus(int nbjr, int score1, int score2, int vie1, int vie2){
	int taille_interligne = 9, widthtxt, heightxt;
	char player1[50], player2[50];
	MLV_Font* font = MLV_load_font( "Data/Fonts/BebasNeue-Regular.ttf" , 20 );
	vspfunc(player1, "Player 1 : %d life \nScore : %d", vie1, score1);
	MLV_draw_adapted_text_box_with_font(
		10, 10,
		player1, font,taille_interligne,
		MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
		MLV_TEXT_LEFT
		);
	if(nbjr > 1){
		vspfunc(player2, "Player 2 : %d life \nScore : %d", vie1, score1);
		MLV_get_size_of_text("Player 2 : 3 life        ", &widthtxt, &heightxt);
		MLV_draw_adapted_text_box_with_font(
			(SCREENWIDTH - widthtxt), 10,
			player2, font, taille_interligne,
			MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
			MLV_TEXT_LEFT
		);
	}
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
}

/*
 * Displays the current frame according to the games state
 */
void dispFrame(platforms p, birds b, char statList[5]){
	dispPlats(p);
	dispBirds(b);
	dispStatus(statList[0],statList[1],statList[2],statList[3],statList[4]);
}

/*
 * Displays a menu with buttons passed as parameters
 * Returns the index of the clicked button
 * Can display an image if given a valid path
 */
int dispMenu(char *button[MAXMENUCHARS], char *imgPath);

/*
 * Clear the display by putting a black box on top of it
 */
int dispClear(){
	MLV_draw_filled_rectangle(0,0,SCREENWIDTH,SCREENHEIGHT,MLV_COLOR_BLACK);
}
