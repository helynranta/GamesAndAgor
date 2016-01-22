#define LIMIT_X 3
#define LIMIT_Y 4
#define LIMIT_R 5
#define OK 1
#define NOT_OK 0
#define PLIND 11 // Payload index location
#include "server_functions.h"

void ComputeNearParticles(Player *sPlayers, Object *sObjects){
	Player *p1 = NULL, *p2 = NULL;
	Near *temp = NULL;
	Object *pObj;

	// Clear the previous Near lists of players
	for(p1 = sPlayers; p1->pNext != NULL; p1 = p1->pNext){
		clearListNear(&(p1->nearPlayers));
		clearListNear(&(p1->nearObjects));
	}

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

		// Calculate distances to each object
		for(pObj = sObjects; pObj != NULL; pObj = pObj->pNext){
			if(isWithinRange(p1->location, pObj->location, p1->scale)){
				if(!(temp = calloc(1,sizeof(Near))))
					perror("calloc");

				temp->pParticle = pObj;
				append2ListNear(&(p1->nearObjects), temp);
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

void newPlayer(Player **pList, struct Packet packet){
	Player *p = NULL;
  if (!(p = calloc(1,sizeof(Player))))
    perror("Calloc");

  /* Get uid, nick, address from packet */
  p->ID = packet.ID;
  p->address = packet.senderAddr;
  memcpy(p->nick, packet.nick, 12);
  //randomLocation(p->location);

  /* Set initial values */
  p->scale = 1;
  p->points = 0;
  p->state = 1;
  p->ping = 0;
  p->nearPlayers = NULL;
  p->nearObjects = NULL;

  /* Add new player to the list */
  append2ListPlayer(pList, p);
}


int msgPacker(char **msgBuffer, Game *pGame, int toPlayerID, int msgType,
	int msgSubType, int outPlayerID, int status){
	/* toPlayerID: to Which player (ID) the message is
	 * outPlayerID: ID of a player which is either OUT or DEAD
	 * (otherwise, set to e.g. 0)
	 * status: used only in (JOIN/NICK) ACK packets, (otherwise, set to e.g. 0)
	 * msgSubType: ACK or GAME_MESSAGE type (otherwise, set to e.g. 0)
	 *
	 * EXAMPLE: sending a message to player 1 that number 2 is out:
	 * 		msgPacker(buffer, sGame, 1, GAME_MESSAGE, PLAYER_OUT, 2, 0);
	 *
	 * Funtion returns int < 0 in case of a failure;
	 */

	int ind = 0, plLength;
	char **pPL = &msgBuffer[PLIND];  // pointer to PAYLOAD

	memset(msgBuffer,'\0',BUFFERSIZE);

	/* HEADER */
	*(uint16_t*)&msgBuffer[ind] = 0;
	ind += sizeof(uint16_t);
	*(uint32_t*)&msgBuffer[ind] = htonl(pGame->gameTime);
	ind += sizeof(uint32_t);
	*(uint8_t*) &msgBuffer[ind] = msgType;
	ind += sizeof(uint8_t);

	/* PAYLOAD */
	switch (msgType){
		case GAME_MESSAGE:
			plLength = gameMsgPacker(pPL, pGame, toPlayerID, msgSubType, outPlayerID);
			break;
	    case ACK:
			plLength = ackPacker(pPL, pGame, toPlayerID, msgSubType, status);
			break;
	    case STATISTICS_MESSAGE:
			plLength = statPacker(pPL, pGame, toPlayerID, msgSubType);
			break;
		default:
			plLength = -1;
			break;
	}

	/* PAYLOAD LENGTH TO HEADER */
	if (plLength < 0)
		return -1;
	else
		*(uint32_t*) &msgBuffer[ind] = htonl(plLength);

	return 0;
}

int gameMsgPacker(char **pPL, Game *pGame, int toPlayerID, int msgSubType,
	int outPlayerID){

	int ind = 0, nPlayers = 0, nObjects = 0, indNPla, indNObj;
	Near *pNear = NULL;
	Player *pPlayer = pGame->sPlayers, *pPla = NULL;
	Object *pObj = NULL;

	*(uint8_t*) &pPL[ind] = msgSubType;
	ind += sizeof(uint8_t);

	switch (msgSubType) {
		case GAME_END:  // similar to POINTS
		case POINTS:
			// Go through the list of players, add the IDs and points to buffer
			*(uint16_t*) &pPL[ind] = htons(pGame->nPlayers);
			ind += sizeof(uint16_t);
			for(; pPlayer != NULL; pPlayer = pPlayer->pNext){
				*(uint16_t*) &pPL[ind] = htons(pPlayer->ID);
				ind += sizeof(uint16_t);
				*(uint32_t*) &pPL[ind] = htonl(pPlayer->points);
				ind += sizeof(uint32_t);
			}
			return ind;
    	case GAME_UPDATE:
			/* FIND CORRESPONDING PLAYER */
			pPlayer = getPlayer(toPlayerID, pPlayer);

			/* PLAYERS OWN INFORMATION */
			*(uint16_t *) &pPL[ind] = htons(pPlayer->location[0]);
			ind += sizeof(uint16_t);
			*(uint16_t *) &pPL[ind] = htons(pPlayer->location[1]);
			ind += sizeof(uint16_t);
			*(uint16_t *) &pPL[ind] = htons(pPlayer->direction[0]);
			ind += sizeof(uint16_t);
			*(uint16_t *) &pPL[ind] = htons(pPlayer->direction[1]);
			ind += sizeof(uint16_t);
			*(uint32_t *) &pPL[ind] = htonl(pPlayer->scale);
			ind += sizeof(uint32_t);

			indNPla = ind;  // store index of nPlayers
			ind += sizeof(uint8_t);
			indNObj = ind;  // store index of nObjects
			ind += sizeof(uint16_t);

			/* PACK NEARBY OBJECTS */
			for(pNear = pPlayer->nearObjects; pNear != NULL; pNear = pNear->pNext, nObjects++){
				pObj = (Object *)(pNear->pParticle);
				*(uint16_t *) &pPL[ind] = htons(pObj->ID);
				ind += sizeof(uint16_t);
				*(uint16_t *) &pPL[ind] = htons(pObj->location[0]);
				ind += sizeof(uint16_t);
				*(uint16_t *) &pPL[ind] = htons(pObj->location[1]);
				ind += sizeof(uint16_t);
			}
			*(uint16_t *) &pPL[indNObj] = htons(nObjects);

			/* PACK NEARBY PLAYERS */
			for(pNear = pPlayer->nearPlayers; pNear != NULL; pNear = pNear->pNext, nPlayers++){
				pPla = (Player *)(pNear->pParticle);
				*(uint16_t *) &pPL[ind] = htons(pPla->ID);
				ind += sizeof(uint16_t);
				*(uint16_t *) &pPL[ind] = htons(pPla->location[0]);
				ind += sizeof(uint16_t);
				*(uint16_t *) &pPL[ind] = htons(pPla->location[1]);
				ind += sizeof(uint16_t);
				*(uint16_t *) &pPL[ind] = htons(pPla->direction[0]);
				ind += sizeof(uint16_t);
				*(uint16_t *) &pPL[ind] = htons(pPla->direction[1]);
				ind += sizeof(uint16_t);
				*(uint32_t *) &pPL[ind] = htons(pPla->scale);
				ind += sizeof(uint32_t);
			}
			*(uint8_t *) &pPL[indNPla] = nPlayers;

			return ind;
    	case PLAYER_DEAD:  // similar to PLAYER_OUT
    	case PLAYER_OUT:
			// add the corresponding playerID (given as parameter) to buffer
			*(uint16_t*) &pPL[ind] = htons(outPlayerID);
			ind += sizeof(uint16_t);
			return ind;
	}

	// If msgSubType is not identified
	return -1;
}

int ackPacker(char **pPL, Game *pGame, int toPlayerID, int msgSubType, int status){
	int ind = 0;

	*(uint32_t*) &pPL[ind] = htonl(pGame->gameTime);
	ind += sizeof(uint32_t);

	*(uint8_t*) &pPL[ind] = msgSubType;
	ind += sizeof(uint8_t);

	switch (msgSubType) {
		case JOIN:
			*(uint8_t*) &pPL[ind] = status;
			ind += sizeof(uint8_t);

			if(status == OK){
				*(uint16_t*) &pPL[ind] = toPlayerID;
				ind += sizeof(uint16_t);
			}

			return ind;
		case NICK:
			*(uint8_t*) &pPL[ind] = status;
			ind += sizeof(uint8_t);
			return ind;
		case PING:
			return ind;
	}
	return -1;
}

int statPacker(char **pPL, Game *pGame, int toPlayerID, int msgSubType){
	Player *pPla = getPlayer(toPlayerID, pGame->sPlayers);
	int ind = 0;
	if (pPla == NULL)
		return -1;
	else{
		if (pPla->ping > 0){
			*(uint16_t*) &pPL[ind] = htons(pPla->ping);
			ind += sizeof(uint16_t);
		}
		return ind;
	}
}

Player *getPlayer(int playerID, Player *pPlayer){
	Player *pPla = pPlayer;
	while (pPla != NULL && pPla->ID != playerID)
		pPla = pPla->pNext;

	return pPla;
}
