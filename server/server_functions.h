#include "structs.h"
#include <math.h>
#include <errno.h>

void computeNearObjects(Player*, Object*);
int isWithinRange(int*, int*, int);
void append2ListNear(Near **pList, Near *pNew);
void append2ListPlayer(Player **pList, Player *pNew);
void append2ListObject(Object **pList, Object *pNew);
void clearListNear(Near *pList);
void clearListPlayer(Player *pList);
void clearListObject(Object *pList);
