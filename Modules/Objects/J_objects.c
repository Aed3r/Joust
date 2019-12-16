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