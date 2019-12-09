#ifndef IA
#define IA

/*Renvoie 1 si l'oiseau est en dessou du joueur1 , 0 sinon */
int isBelow(birds b, int n);

/*Gère l'ia des enemis peu agressifs*/
void movePassievIa(birds b);

/*Idem mais pour des enemis agressifs*/
void moveAggIa(birds b);

#endif