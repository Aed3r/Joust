#include "J_objects.h"

/*
 * Loads the image with the given fileName
 */
MLV_Image* loadImage(char *fileName) {
	MLV_Image *tmpImg;
	FILE *file;
	char path[200];

	strcpy(path, "Data/Sprites/");
	strcat(path, fileName);

	/* Test if file exists */
    if ((file = fopen(path, "r"))) {
        fclose(file);
		/* Load image in file */
        tmpImg = MLV_load_image(path);
		return tmpImg;
    }
	printf("Error loading image %s!\n", path);
	return 0;
}

/*
 * Initializes array objT with predefined object types 
 * Returns 1 on successful import, 0 otherwise
 */ 
int importOBJs (objectTypes *o, char *filePath){
	FILE *f;
	char tmpSpritePath[200];

	if((f = fopen(filePath, "r")) == NULL){
		printf("Erreur dans l'ouverture du fichier \"%s\"!\n", filePath);
		return 0;
	}else{
		o->l = 0;
		while(fscanf(f, "%d %s %s %d %d", &o->objT[o->l].objectID, tmpSpritePath, o->objT[o->l].name, &o->objT[o->l].s.width, &o->objT[o->l].s.height) == 5) {
			if ((o->objT[o->l].sprite = loadImage(tmpSpritePath)) == 0) exit(EXIT_FAILURE);
			MLV_resize_image(o->objT[o->l].sprite, o->objT[o->l].s.width, o->objT[o->l].s.height);
			o->l++;
		}
	}

	fclose(f);
	return 1;
}

/*
 * Initializes array brdT with predefined bird types
 * Returns 1 on successful import, 0 otherwise
 */ 
int importBirdTypes (birdTypes *b, objectTypes *o, char *filePath){
	FILE *f;
	int i = 0, j = 0, tmpObjID;

	if((f = fopen(filePath, "r")) == NULL){
		printf("Erreur dans l'ouverture du fichier \"%s\"!\n", filePath);
		return 0;
	}else{
		b->l = 0; /*&b->brdT[i].o.objectID*/
		while(fscanf(f, "%d %f %f %f %d %d %d", 
				&tmpObjID, &b->brdT[i].hSpeed, &b->brdT[i].vSpeed, 
				&b->brdT[i].flapStrength, &b->brdT[i].isMob, 
				&b->brdT[i].aggressiveness, &b->brdT[i].value) == 7){
			while (o->objT[j].objectID != tmpObjID) j++;
			b->brdT[i].o = o->objT[j];
			b->l++;
			i++;
		}
	}
	fclose(f);
	return 1;
}

/*Write a score associated with a name at the end of the score.txt file
Return 0 if there was an error*/
int saveScore (int score, char *name){
	FILE *f;
	/*Try to open the file then add the new score and name at the end of the file*/
	if((f = fopen("Data/Files/score.txt", "a")) == NULL){
		printf("Erreur dans l'ouverture du fichier score.txt !\n");
		return 0;
	}else{
		fprintf(f, "%s %d\n", name, score);
	}
	fclose(f);
	return 1;
}

/*
 * Unload all sprites 
 */
void unloadSprites(objectTypes *oT) {
	int i;
	for (i = 0; i < oT->l; i++) MLV_free_image(oT->objT[i].sprite);
}

/*
 * Saves the games current state to a file
 */
void saveGameState(birds b, int waveCounter, int nbjr) {
	int i;
	FILE *f;

	f = fopen("Data/Files/savestate", "w");

	fprintf(f, "%d\n", waveCounter);
	fprintf(f, "%d\n", nbjr);
	fprintf(f, "%d %f %f %d %f %f %d %d\n", b.brd[0].b.o.objectID, b.brd[0].p.x,
			b.brd[0].p.y, b.brd[0].dir, b.brd[0].hVel, b.brd[0].vVel,
			b.brd[0].lives, b.brd[0].score);
	if (nbjr == 2) {
		fprintf(f, "%d %f %f %d %f %f %d %d\n", b.brd[1].b.o.objectID, b.brd[1].p.x,
				b.brd[1].p.y, b.brd[1].dir, b.brd[1].hVel, b.brd[1].vVel,
				b.brd[1].lives, b.brd[1].score);
	}
}

/*
 * Load a saved gamestate
 */ 
int loadGameState(birds *b, int *waveCounter, int *nbjr) {
	FILE *f;
	/* Try to open the file */
	if((f = fopen("Data/Files/savestate", "r")) == NULL){
		printf("Erreur dans l'ouverture du fichier score.txt !\n");
		return 0;
	}else{
		fscanf(f, "%d", )
	}
}