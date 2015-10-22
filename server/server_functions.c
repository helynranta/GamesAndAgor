#define LIMIT_X 3
#define LIMIT_Y 4
#define LIMIT_R 5
#include "server_functions.h"

void ComputeNearParticles(Player *sPlayers, Object *sObjects){
	Player *p1 = NULL, *p2 = NULL;
	Near *temp = NULL;
	// Object *pObj;

	// Go through each player
	for(p1 = sPlayers; p1->pNext != NULL; p1 = p1->pNext){
		// Calculate distances to each player
		for(p2 = p1->pNext; p2 != NULL; p2 = p2->pNext){
			if(isWithinRange(p1->location, p2->location, p1->scale)){
				if(!(temp = calloc(1,sizeof(Near))))
					perror("calloc");

				temp->pParticle = p2;
				append2ListNear(&(p1->nearPlayers), temp);
				temp = NULL;
			}

			else if (isWithinRange(p1->location, p2->location, p2->scale)){
				if(!(temp = calloc(1,sizeof(Near))))
					perror("calloc");

				temp->pParticle = p1;
				append2ListNear(&(p2->nearPlayers), temp);
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

void append2ListNear(Near **pList, Near *pNew){
	Near *temp = NULL, *p = NULL;
	p = *pList;  // pointer of the list
	temp = p;  // Store to temp
	p = pNew;  // New object should be the first
	p->pNext = temp;  // Previous first is now the second
	*pList = p;  // Update the pointer of the list
}

void append2ListPlayer(Player **pList, Player *pNew){
	// For functional explanation, see append2ListNear
	Player *temp = NULL, *p = NULL;
	p = *pList;
	temp = p;
	p = pNew;
	p->pNext = temp;
	*pList = p;
}

void append2ListObject(Object **pList, Object *pNew){
	// For functional explanation, see append2ListNear
	Object *temp = NULL, *p = NULL;
	p = *pList;
	temp = p;
	p = pNew;
	p->pNext = temp;
	*pList = p;
}

void clearListNear(Near **pList){
	Near *next = NULL, *p = NULL;
	p = *pList;
	while(p != NULL){
		next = p->pNext;
		free(p);
		p = next;
	}
	*pList = NULL;
}

void clearListPlayer(Player **pList){
	Player *next = NULL, *p = NULL;

	p = *pList;
	while(p != NULL){
		next = p->pNext;
		clearListNear(&(p->nearPlayers));
		clearListNear(&(p->nearObjects));
		free(p);
		p = next;
	}
	*pList = NULL;
}

void clearListObject(Object **pList){
	Object *next = NULL, *p = NULL;

	p = *pList;
	while(p != NULL){
		next = p->pNext;
		free(p);
		p = next;
	}
	*pList = NULL;
}

void newPlayer(Player **pList){
	Player *p = NULL;
	if (!(p = calloc(1,sizeof(Player))))
		perror("calloc");
	append2ListPlayer(pList, p);
	p->address = (struct sockaddr*)&(p->addressStorage);
}

void headerPacker(char **msgBuffer, int playerID, int gameTime, int msgType, ...){
	int index = 0, argList[10] = {-1}, msgSubType;
	int msgPayloadIndex, payloadLengthIndex;
	char *msg = *buffer;
	va_list ap;

	// Pack the header first
	*(uint16_t*)&msg[index] = htons(playerID);
	index += sizeof(uint16_t);
	*(uint32_t*)&msg[index] = htonl(gameTime);
	index += sizeof(uint32_t);
	*(uint8_t*)&msg[index] = msgType;
	index += sizeof(uint8_t);
	// *(uint32_t*)&msg[index] = htonl(payloadLength);
	// index += sizeof(uint32_t)

	// Store payload length's index and message's payload's index in different
	// variables
	payloadLengthIndex=index;
	msgPayloadIndex = index + sizeof(uint32_t);

	// Act based upon the message type
	// If the message type is one that contains payload, go through the rest of
	// the arguments
	switch (msgType) {
		case GAME:
			payloadLength = packGameMessage(playerID, &msg[msgPayloadIndex]);
			break;
		case ACK:
			break;
		case CHAT:
			break;
		case STAT:
			break;
	}

	switch (/* expression */) {
		case /* value */:
	}
	if(msgType == GAME || msgType == ACK){
		va_start(ap, msgType);
		msgSubType = va_arg(ap, int);
		// while()
	}


	// FOKIT


}
