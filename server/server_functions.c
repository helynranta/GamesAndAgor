#define LIMIT_X 3
#define LIMIT_Y 4
#define LIMIT_R 5
#include "server_functions.h"

void ComputeNearObjects(Player *sPlayers, Object *sObjects){
	Player *p1, *p2;
	Near *temp;
	// Object *pObj;

	// Go through each player
	for(p1 = sPlayers; p1->pNext != NULL; p1 = p1->pNext){
		// Calculate distances to each player
		for(p2 = p1->pNext; p2 != NULL; p2 = p2->pNext){
			if(isWithinRange(p1->location, p2->location, p1->scale)){
				if(!(temp = malloc(sizeof(Near))))
					perror("malloc");
				memset(temp,0,sizeof(Near));

				temp->pParticle = p2;
				append2ListNear(p1->nearPlayers, temp);
				temp = NULL;
			}

			else if (isWithinRange(p1->location, p2->location, p2->scale)){
				if(!(temp = malloc(sizeof(Near))))
					perror("malloc");
				memset(temp,0,sizeof(Near));

				temp->pParticle = p1;
				append2ListNear(p2->nearPlayers, temp);
				temp = NULL;
			}
		}
	}
}

int isWithinRange(int location1[2], int location2[2], int scale){
	int deltaX, deltaY;
	/* Which kind of calculation do we want to implement? Rectangle or circle?
	 * Rectangle would minimize packet size (no particles misclassified as near)
	// RECTANGLE
	deltaX = abs(location2[0] - location1[0]);
	deltaY = abs(location2[1] - location1[1]);

	if (deltaX < LIMIT_X * scale && deltaY < LIMIT_Y * scale)
		return 1;
	else
		return 0;
	*/

	// CIRCLE
	deltaX = pow(location2[0] - location1[0], 2);
	deltaY = pow(location2[1] - location1[1], 2);

	if(deltaX + deltaY < pow(LIMIT_R * scale, 2))
		return 1;
	else
		return 0;

}

void append2ListNear(Near *pList, Near *pNew){
	Near *temp;
	temp = pList;
	pList = pNew;
	pList->pNext = temp;
}

void append2ListPlayer(Player *pList, Player *pNew){
	Player *temp;
	temp = pList;
	pList = pNew;
	pList->pNext = temp;
}

void append2ListObject(Object *pList, Object *pNew){
	Object *temp;
	temp = pList;
	pList = pNew;
	pList->pNext = temp;
}

void clearListNear(Near *pList){
	Near *next, *p;

	p=pList;
	while(p != NULL){
			next = p->pNext;
			free(p);
			p = next;
	}
}

void clearListPlayer(Player *pList){
	Player *next, *p;

	p=pList;
	while(p != NULL){
			next = p->pNext;
			clearListNear(p->nearPlayers);
			clearListNear(p->nearObjects);
			free(p);
			p = next;
	}
}

void clearListObject(Object *pList){
	Object *next, *p;

	p=pList;
	while(p != NULL){
			next = p->pNext;
			free(p);
			p = next;
	}
}
