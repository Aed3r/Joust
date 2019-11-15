#include "obj_test.h"
#include "../J_objects.h"

void printOBJs(objectTypes *o) {
    int i;

    printf("Loaded Objects :\n");
    for (i = 0; i < o->l; i++) {
        printf(" - ID: %d ; Sprite: %s; Name: %s; Width: %d; Length: %d\n",
            o->objT[i].objectID, o->objT[i].spriteName, o->objT[i].name,
            o->objT[i].s.width, o->objT[i].s.height);
    }
}

void printBRDs(birdTypes *b) {
    int i;

    printf("Loaded Birds :\n");
    for (i = 0; i < b->l; i++) {
        printf(" - runSpeed: %f; glideSpeed: %f; flapStrength: %f; respawnTime: %d; isMob: %d; aggressiveness: %d\n",
            b->brdT[i].runSpeed, b->brdT[i].glideSpeed, b->brdT[i].flapStrength, 
            b->brdT[i].respawnTime, b->brdT[i].isMob, b->brdT[i].aggressiveness);
    }
}

int main() {
    objectTypes o;
    birdTypes b;

    importOBJs(&o, "../../Data/Files/objects");
    importBirdTypes(&b, "../../Data/Files/birds");

    printOBJs(&o);
    printf("%d", b.l);
    /*printBRDs(&b);*/

    exit(0);
}