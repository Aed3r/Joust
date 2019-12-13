#ifndef IA
#define IA
#include "../Objects/J_objects.h"
/*Renvoie 1 si l'oiseau est en dessou du joueur1 , 0 sinon */
int isBelow(birds b, int n);

/*Gère l'ia des enemis peu agressifs*/
void movePassiveIa(bird *b, birds brd);

/*Idem mais pour des enemis agressifs*/
void moveAggIa(bird *b, birds brd);

#endif