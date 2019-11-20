#include "J_obj_test.h"
#include "../J_objects.h"

/* Affiche tous les attributs de tous les types d'objets */
void printOBJs(objectTypes *o) {
    int i;

    printf("Loaded Objects :\n");
    for (i = 0; i < o->l; i++) {
        printf(" - ID: %d ; Sprite: %s; Name: %s; Width: %d; Height: %d\n",
            o->objT[i].objectID, o->objT[i].spriteName, o->objT[i].name,
            o->objT[i].s.width, o->objT[i].s.height);
    }
}

/* Affiche tous les attributs de tous les types d'oiseau */
void printBRDs(birdTypes *b) {
    int i;

    printf("Loaded Birds :\n");
    for (i = 0; i < b->l; i++) {
        printf(" - ID: %d ; Sprite: %s; Name: %s; Width: %d; Height: %d; runSpeed: %f; glideSpeed: %f; flapStrength: %f; respawnTime: %d; isMob: %d; aggressiveness: %d\n",
            b->brdT[i].o.objectID, b->brdT[i].o.spriteName, b->brdT[i].o.name, 
            b->brdT[i].o.s.width, b->brdT[i].o.s.height,
            b->brdT[i].runSpeed, b->brdT[i].glideSpeed, b->brdT[i].flapStrength, 
            b->brdT[i].respawnTime, b->brdT[i].isMob, b->brdT[i].aggressiveness);
    }
}

int main() {
    objectTypes o;
    birdTypes b;

    importOBJs(&o, "../../Data/Files/objects");
    importBirdTypes(&b, &o, "../../Data/Files/birds");

    printOBJs(&o);
    printBRDs(&b);
    
    /* Ne modifie que l'oiseau, pas l'objet utilisé pour*/
    strcpy(b.brdT[1].o.name, "TEST"); 

    printf("------------\n");

    printOBJs(&o);
    printBRDs(&b);

    exit(0);
}