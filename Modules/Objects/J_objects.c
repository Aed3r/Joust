#include <J_objects.h>

/*
 * Initializes array objT with predefined object types 
 * Returns 1 on successful import, 0 otherwise
 */ 
int importOBJs (objectType oType[OBJS], char *filePath){
	int i;
	for(i=0;i<=OBJS;i++){
		if(oType[i] != NULL){
			objT[i] = oType[i];/* J'ai mis longtemps a comprendre ce que 
								* tu voulais faire mdr et je pense pas avoir réussi*/
		}else{
			return 0;
		}
	}
	return 1
}

/*
 * Initializes array brdT with predefined bird types
 * Returns 1 on successful import, 0 otherwise
 */ 
int importBirdTypes (birdType bType[OBJS], char *filePath){
	int i;
	for(i=0;i<=OBJS;i++){
		if(bType[i] != NULL){
			brdT[i] = bType[i];
		}else{
			return 0;
		}
	}
	return 1
}

/*
 * Initializes array plt with predefined platforms
 * Returns 1 on successful import, 0 otherwise
 */
int importPlatforms (platform plat[PLATFORMS], char *filePath){
	int i;
	for(i=0;i<PLATFORMS;i++){
		if(plat[i] != NULL){
			plt[i] = plat[i];
		}else{
			return 0;
		}
	}
	return 1;
}