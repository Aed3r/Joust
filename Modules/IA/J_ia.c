#include "J_ia.h"

int isBelow(birds *b, int n){
	if(b->brd[0].p.y > b->brd[n].p.y) return 1;
	return 0;
}

void movePassiveIa(birds *b){
	int i;
	for(i=2;i<b->l;i++){
		if(isBelow(b, i)) b->brd[i].flapped = 1;
	}
}