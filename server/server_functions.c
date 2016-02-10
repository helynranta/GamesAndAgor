#define LIMIT_X 3
#define LIMIT_Y 4
#define LIMIT_R 5
#define OK 1
#define NOT_OK 0
#define PLIND 11 // Payload index location
#include "server_functions.h"

void gameInit(Game *pGame){
	pGame->gameTime = 0;
	pGame->nPlayers = 0;
	pGame->sPlayers = NULL;
	pGame->sObjects = NULL;
	pGame->sAcks = NULL;
	pGame->packetID =0;
	pGame->pingID = 0;
}

void ComputeNearParticles(Player *sPlayers, Object **sObjects){
	Player *p1 = NULL, *p2 = NULL;
	Near *temp = NULL;
	Object *pObj = *sObjects, *pT = NULL, *pPrev = NULL;

    int isIn = 0;
	if (sPlayers==NULL){return;}
	// Clear the previous Near lists of players
	for(p1 = sPlayers; p1 != NULL; p1 = p1->pNext){
		clearListNear(&(p1->nearPlayers));
		clearListNear(&(p1->nearObjects));
	}

	// Go through each player
	for(p1 = sPlayers; p1 != NULL; p1 = p1->pNext){
		if (p1->state != ALIVE){continue;}

		// Calculate distances to each player
		for(p2 = p1->pNext; p2 != NULL; p2 = p2->pNext){
			if (p2->state != ALIVE){continue;}

            isIn = isWithinRange(p1->location, p2->location, p1->scale, p2->scale);
			if(isIn){
				if(isIn == -1) { // That p1 fucker just ate p2.
                    eventEatPlayer(p1,p2);
                }
                else if (isIn == -2) {// p2 ate p1. p1 totally deserved it.
                    eventEatPlayer(p2,p1);
                }

                if(!(temp = calloc(1,sizeof(Near))))
					perror("calloc");

				temp->pParticle = p2;
				append2ListNear(&(p1->nearPlayers), temp);
				temp = NULL;
			}

			else if (isWithinRange(p1->location, p2->location, p2->scale, p1->scale)){
				if(!(temp = calloc(1,sizeof(Near))))
					perror("calloc");

				temp->pParticle = p1;
				append2ListNear(&(p2->nearPlayers), temp);
				temp = NULL;
			}
		}

		// Calculate distances to each object
		while(pObj != NULL){
			isIn = isWithinRange(p1->location, pObj->location, p1->scale,OBJ_SIZE);
			if(isIn < 0){ // EAT OBJECT
				pT = pObj;
				if(pPrev == NULL){ // if first in the list
					pObj = pObj->pNext;
					*sObjects = pObj;
				}
				else{ // put previous' pNext to point to pObj's pNext
					pPrev->pNext = pObj->pNext;
					pObj = pPrev; // put pObj back to previous
				}
				eventEatObject(p1, &pT);
				if(pObj == NULL){break;} // if we just ate the last fucker
				continue;
			}
			else if (isIn > 0){
				if(!(temp = calloc(1,sizeof(Near))))
					perror("calloc");
				temp->pParticle = pObj;
				append2ListNear(&(p1->nearObjects), temp);
				temp = NULL;
			}
			pPrev = pObj;
			pObj = pObj->pNext; // UPDATE pObj
		}
	}
}

void eventEatObject(Player *pPla, Object **pObj){
	pPla->scale += OBJ_SIZE;
	pPla->points += OBJ_SIZE;
	free(*pObj);
	*pObj = NULL;
}

int isWithinRange(uint16_t location1[2], uint16_t location2[2], uint32_t scale1,
	uint32_t scale2){
	long deltaX, deltaY;
	long loc1[] = {(long)location1[0], (long)location1[1]};
	long loc2[] = {(long)location2[0], (long)location2[1]};
	long sca1 = (long)scale1;
	long sca2 = (long)scale2;

	float range = sca1/PLA_SIZE, eucl = 0;
	float rangeY = range * SCREEN_X, rangeX = range * SCREEN_Y;


	// RECTANGLE
	deltaY = abs(loc1[1] - loc2[1] - floor(sca2/2));
	deltaX = abs(loc1[0] - loc2[0] - floor(sca2/2));

	/* Euclidean distance */
    eucl = sqrt(pow(deltaX,2) + pow(deltaY,2));
	printf("deltaX: %ld\t rangeX: %ld", deltaX, rangeX);
	printf("deltaY: %ld\t rangeY: %ld", deltaY, rangeY);
	if (eucl <  sca1 && sca1 > sca2)
        return -1;
  else if (eucl < sca2 && sca1 < sca2)
        return -2;
  else if (deltaX < rangeX && deltaY < rangeY)
		return 1;
	else
		return 0;
}

void eventEatPlayer(Player *eater, Player *eaten){
    eater->scale += floor(eaten->scale/2);
	eater->points += floor(eaten->scale/2);
    eaten->state = EATEN;
}

void addAck2List(Ack **pAckList, char *msg, uint32_t gameTime, int msgLength,
	uint32_t packetID, uint16_t toPlayerID){
    Ack *pAck = NULL;
    if ((pAck = calloc(1, sizeof(Ack))) == NULL) { perror("calloc"); }

    pAck->gameTimeSent = gameTime;
    memcpy(pAck->msg, msg, msgLength);
    pAck->msgLength = msgLength;
    pAck->packetID = packetID;
		pAck->toPlayerID = toPlayerID;
		pAck->pNext = NULL;

    /* Add the ack to the start of the linked list */
    append2ListAck(pAckList,pAck);
}

void append2ListAck(Ack **pList, Ack *pNew){
	if(pNew==NULL){return;}
	pNew->pNext = *pList;
	*pList = pNew;
}

void removeAck(Ack **pList, uint32_t ackID){
    Ack *p = *pList, *prev = NULL;

		if (p==NULL) {return;}
    else if (p->packetID == ackID){ // if first in list
        p = p->pNext;
        free(*pList);
        *pList = p;
    }
    else {
        while(p->packetID != ackID){
            if(p->pNext == NULL) {
                return;
            }
            else {
                prev = p;
                p = p->pNext;
						}
        }
				prev->pNext = p->pNext;

				free(p);
				p = NULL;
    }
}

void append2ListNear(Near **pList, Near *pNew){
	if(pNew==NULL){return;}
	pNew->pNext = *pList;
	*pList = pNew;
}

void append2ListPlayer(Player **pList, Player *pNew){
	if(pNew==NULL){return;}
	pNew->pNext = *pList;
	*pList = pNew;
}

void append2ListObject(Object **pList, Object *pNew){
	if(pNew==NULL){return;}
	pNew->pNext = *pList;
	*pList = pNew;
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

void newPlayer(Player **pList, struct Packet packet, uint16_t nPlayers){
	Player *p = NULL;
  if (!(p = calloc(1,sizeof(Player))))
    perror("Calloc");

  /* Get uid, nick, address from packet */
  p->ID = nPlayers+1;
  p->address = packet.senderAddr;

  //memcpy(p->nick, packet.nick, 12);
  // randomLocation(p->location);
	p->location[0]=(uint16_t)100;
	p->location[1]=(uint16_t)100;

	p->direction[0]=(uint16_t)0;
	p->direction[1]=(uint16_t)0;

  /* Set initial values */
  p->scale = 1;
  p->points = 0;
  p->state = JOINING;
  p->ping = 0;
  p->nearPlayers = NULL;
  p->nearObjects = NULL;

  /* Add new player to the list */
  append2ListPlayer(pList, p);
}

void respawnPlayer(Player *pPlayer){
	if(pPlayer->state == DEAD){
		randomLocation(pPlayer->location);
		pPlayer->state = ALIVE;
	}

}

int msgPacker(char *msgBuffer, Game *pGame, uint16_t toPlayerID, int msgType, uint8_t msgSubType, uint16_t outPlayerID, int status){
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
	char *pPL = &msgBuffer[PLIND];  // pointer to PAYLOAD

	memset(msgBuffer,'\0',BUFFERSIZE);

	/* HEADER */
	/* Switched the USER_ID to be the ID of the player, because communication problems */
	*(uint16_t*)&msgBuffer[ind] = htons(toPlayerID);
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
			if (plLength < 0)
				return -1;
			else if(msgSubType == JOIN)
				break;
			else{
				printf("ADDING TO sAcks\n");
				*(uint32_t*) &msgBuffer[ind] = htonl(plLength);
				addAck2List(&(pGame->sAcks),msgBuffer,pGame->gameTime,plLength,pGame->gameTime, toPlayerID);
				break;
			}
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

	return PLIND+plLength;
}

int gameMsgPacker(char *pPL, Game *pGame, uint16_t toPlayerID, uint8_t msgSubType, uint16_t outPlayerID){
	printf("Packing game MSG\n");
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
			printf("Packing game update\n");
			/* FIND CORRESPONDING PLAYER */
			pPlayer = getPlayer(toPlayerID, pPlayer);
			if(pPlayer == NULL){
				printf("NULIKKA, ei pelaajaa\n");
				return -1;
			}

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
			if (pPlayer->nearPlayers == NULL){
				printf("Ei pelaajia lähellä\n");
			}
			else{
				printf("Pakataan lähellä olevia pelaajia\n");
			}
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
			printf("Near Players: %d\n", nPlayers);

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

int ackPacker(char *pPL, Game *pGame, uint16_t toPlayerID, int msgSubType,
	int status){
	int ind = 0;

	*(uint32_t*) &pPL[ind] = htonl(pGame->gameTime);
	ind += sizeof(uint32_t);

	*(uint8_t*) &pPL[ind] = msgSubType;
	ind += sizeof(uint8_t);

	switch (msgSubType) {
		case JOIN:
			printf("PACKING ACK JOIN\n");
			*(uint8_t*) &pPL[ind] = status;
			ind += sizeof(uint8_t);

			if(status == OK){
				*(uint16_t*) &pPL[ind] = htons(toPlayerID);
				ind += sizeof(uint16_t);
			}

			return ind;
		case NICK:
			printf("PACKING ACK NICK\n");
			*(uint8_t*) &pPL[ind] = status;
			ind += sizeof(uint8_t);
			return ind;
		case PING:
			return ind;
	}
	return -1;
}

int statPacker(char *pPL, Game *pGame, uint16_t toPlayerID, int msgSubType){
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

Player *getPlayer(uint16_t playerID, Player *pPlayer){
	Player *pPla = pPlayer;
	while (pPla != NULL && pPla->ID != playerID)
		pPla = pPla->pNext;

	return pPla;
}

void randomLocation(uint16_t *location){
    srand(time(NULL));
    location[0] = rand() % LIMIT_X;
    location[1] = rand() % LIMIT_Y;
}

/* ltr to blame from the code below */
/* Sends game update to everyone */
void sendGameUpdate(Game *game, char *buf, int socket, socklen_t addrlen){
	Player *pPla = game->sPlayers;
	while(pPla != NULL){

		/* Pack msg */
		msgPacker(buf, game, pPla->ID, GAME_MESSAGE, GAME_UPDATE, 0, 0);
		/* Send msg */
		/* TODO: fix magic numeber 256 to --> SIZE or something */
		sendto(socket, buf, 256, 0, &pPla->address, addrlen);
		/* Move on to the next player */
		pPla = pPla->pNext;
	}
}

/* Check nick, new nick must be available */
/* Return Status: 1 OK,  0 NOT OK */
int checkNick(char *nick,Player *pPlayer){
	char lerppana[] = "lerppana";
	char Lerppana[] = "Lerppana";
	Player *tmp = pPlayer;
	int result = 1;

	while(tmp != NULL){
		result = strncmp(nick, pPlayer->nick, 12);
		if(result == 0){
			return 0;
		}
		result = strncmp(nick, lerppana, 12);
		if(result == 0){
			return 0;
		}
		result = strncmp(nick, Lerppana, 12);
		if(result == 0){
			return 0;
		}

		tmp = tmp->pNext;
	}
	return 1;
}

/* Function for resending unacked messages */
/* line 139 ack for help */
void resendMsg(int socket, socklen_t addrlen, Ack **ackList, Player *players) {
	Ack *ack = *ackList;
	Player *p = NULL;

	while(ack != NULL) {
		p = getPlayer(ack->toPlayerID, players);

		if(p == NULL) {
			printf("\n\n\nCouldn't find the player to whom the msg should be resent\n\n\n");
			continue;
		}
		sendto(socket, ack->msg, ack->msgLength, 0, &p->address, addrlen);
		ack = ack->pNext;
	}
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/* compare the ip adresses , returns 0 if they are the same */
int ipcmp(struct sockaddr *from, struct sockaddr *player){
	char s1[INET6_ADDRSTRLEN];
	char s2[INET6_ADDRSTRLEN];
	strcpy(s1, get_in_addr(from));
	strcpy(s2, get_in_addr(player));
	return strcmp(s1, s2);
}

// Tähän osote parametriks
/* Returns player ID if player exists already, else -1 */
int checkJoin(Player *pPlayer, struct sockaddr *from) {

	Player *tmp = pPlayer;
	while(tmp != NULL){

		// If tmp->address on sama ku osote parametri nii return 0
		/* If ip is found , return the player ID*/
		if((ipcmp(&tmp->address, from)) == 0 ) {
			return tmp->ID;
		}

		tmp = tmp->pNext;
	}

	return -1;
}

void checkEaten(Game *pGame, int udpFD, socklen_t addrlen){
	/* This function checks if player is eaten, sends him a message and changes
   * his status to eaten
	 * TODO: send msg to all players if someone has been eaten */
	char buf[BUFFERSIZE] = {0};
	int msgLen;

	Player *p = pGame->sPlayers;
	for(;p!=NULL;p=p->pNext){
		if(p->state != EATEN){ continue; } // we are only interested in EATEN states
		msgLen = msgPacker(buf, pGame, p->ID, GAME_MESSAGE, PLAYER_DEAD, p->ID, 0);
		if(!msgLen)
			perror("msgPacker");
		else {
			p->state = DEAD;
			sendto(udpFD,buf,msgLen,0,&p->address,addrlen);
		}
	}
}

/* Sends the whole buffer over tcp */
int sendAllTCP(int socket, char *buf, int *length) {
	int total = 0;
	int bytesleft = *length;

	int sent;

	/* Keep on sending while there's stuff to send */
	while (total < *length) {
		sent = send(socket, buf+total, bytesleft, 0);
		if(sent == -1) break;
		total += sent;
		bytesleft -= sent;
	}

	*length = total;

	return sent == -1?-1:0; /* -1 on failure, 0 OK */
}
