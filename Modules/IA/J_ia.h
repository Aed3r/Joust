#ifndef IA
#define IA

/*Gère l'ia des enemis peu agressifs*/
void movePassievIa(birds b);

/*Idem mais pour des enemis agressifs*/
void moveAggIa(birds b);

/*Gère la joute entre deux IA, si deux IA sont sur le point
 *de jouter elle essaie de s'esquiver, celle de droite (choix arbitraire)
 *devras essayer de passer en dessous de l'autre
 */
void IAJoust(bird a, bird b);

/*Renvoie l'altitude d'un oiseau b (parmis les trois prédefinies) 
 *Ou une valeur intermédiare si l'oiseau est entre deux altitudes
 * Air : 4, mid-air : 2, ground : 0
 */
int Altitude(bird b);

/*Renvoie 1 si l'oiseau est en dessou du joueur, 0 sinon*/
int isBelow(bird b);

/*Gère le comportement d'un oiseau qui augmente d'une altitude*/
void goUp(bird b);

/*Gère le comportement d'un oiseau qui descend d'une altitude*/
void goDown(bird b);
#endif