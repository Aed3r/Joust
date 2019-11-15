#include <stdio.h>

#include "J_objects.h"

/*
 * Initializes array objT with predefined object types 
 * Returns 1 on successful import, 0 otherwise
 */ 
int importOBJs (objectTypes *o, char *filePath){
	FILE *f;
	int i = 0;
	if((f = fopen(filePath, "r")) == NULL){
		printf("Erreur dans l'ouverture du fichier \"%s\"!\n", filePath);
		return 0;
	}else{
		while(fscanf(f,"%d %s %s %d %d", &o->objT[i].objectID, o->objT[i].spriteName, o->objT[i].name, &o->objT[i].s.width, &o->objT[i].s.height) == 5) {
			o->l++;
			i++;
		}
	}
	return 1;
}

/*
 * Initializes array brdT with predefined bird types
 * Returns 1 on successful import, 0 otherwise
 */ 
int importBirdTypes (birdTypes *b, char *filePath){
	FILE *f;
	int i = 0;
	if((f = fopen(filePath, "r")) == NULL){
		printf("Erreur dans l'ouverture du fichier \"%s\"!\n", filePath);
		return 0;
	}else{
		while(fscanf(f,"%d %f %f %f %d %d %d", &b->brdT[i].o.objectID, &b->brdT[i].runSpeed, &b->brdT[i].glideSpeed, &b->brdT[i].flapStrength, &b->brdT[i].respawnTime, &b->brdT[i].isMob, &b->brdT[i].aggressiveness) == 7){
			b->l++;
			i++;
		}
	}
	return 1;
}