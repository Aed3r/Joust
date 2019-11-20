#include "J_ctr_test.h"

#include "../J_controls.h"
#include "../../Objects/J_objects.h"

void printP(platforms *p) {
    int i;

    printf("Platform instances (%d) :\n", p->l);
    for (i = 0; i < p->l; i++) {
        printf("  - ID: %d; Name : %s; X: %d; Y: %d\n", 
        p->plt[i].instanceID, p->plt[i].o.name, p->plt[i].p.x, p->plt[i].p.y);
    }
}

int main() {
    objectTypes oT;
    birdTypes bT;
    birds b;
    platforms p;

    importOBJs(&oT, "../../Data/Files/objects");
    importBirdTypes(&bT, &oT, "../../Data/Files/birds");

    createPlatform(1, oT, &p, 50, 50);

    printP(&p);

    exit(0);
}