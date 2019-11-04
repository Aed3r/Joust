#include "J_objects.h"

/*
 * Initializes array objT with predefined object types 
 * Returns 1 on successful import, 0 otherwise
 */ 
int importOBJs (objectTypes o, char *filePath){

}

/*
 * Initializes array brdT with predefined bird types
 * Returns 1 on successful import, 0 otherwise
 */ 
int importBirdTypes (BirdTypes b, char *filePath){
	FILE *f;
	int i =0;
	if((f = fopen(filePath, "r")) == NULL){
		printf("Erreur dans l'ouverture du fichier !\n");
		exit(0);
	}else{
		while(fscanf(f,"%d %d %d %d %d %d %d\n",b.brdT[i].b, b.brdT[i].instanceID, b.brdT[i].))
	}
	
}

/*
 * Initializes array plt with predefined platforms
 * Returns 1 on successful import, 0 otherwise
 */
int importPlatforms (platforms p, char *filePath){

}