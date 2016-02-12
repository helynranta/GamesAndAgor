#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H
#include "structs.h"
#endif

#include <math.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
// #define PAYLOAD_INDEX

/* Global for the signal handler */
static int exitFlag = 0;

void gameInit(Game *pGame);
void ComputeNearParticles(Player *sPlayers, Object **sObjects);
void eventEatObject(Player *pPla, Object **pObj);
int isWithinRange(uint16_t location1[2], uint16_t location2[2], uint32_t scale1, uint32_t scale2);
void eventEatPlayer(Player *eater, Player *eaten);
void addAck2List(Ack **pAckList, char *msg, uint32_t gameTime, int msgLength, uint32_t packetID, uint16_t toPlayerID);
void append2ListAck(Ack **pList, Ack *pNew);
void removeAck(Ack **pList, uint32_t ackID);
void append2ListNear(Near **pList, Near *pNew);
void append2ListPlayer(Player **pList, Player *pNew);
void newObject(Object **pList, uint32_t *nObjects);
void append2ListObject(Object **pList, Object *pNew);
void clearListNear(Near **pList);
void clearListPlayer(Player **pList);
void clearListObject(Object **pList);
void removePlayer(Player **pList, uint32_t playerID);
void newPlayer(Game *game, struct Packet);
void respawnPlayer(Player *pPlayer);

/* ltr did this, blame me if something bad happens */
void sendGameUpdate(Game *game, char *buf, int socket, socklen_t addrlen);
int checkNick(char *nick, Player *playerlist);
int checkJoin(Player *pPlayer, struct sockaddr *from);
void checkEaten(Game *pGame, int udpFD, socklen_t addrlen);
int sendAllTCP(int socket, char *buf, int *length);
void resendMsg(int socket, socklen_t addrlen, Ack **ackList,Player *players);
void informTheDead(Game *game, char *buf, int socket, socklen_t addrlen);


/* PACKING FUNCTIONS */
int msgPacker(char *msgBuffer, Game *pGame, uint16_t toPlayerID, int msgType, uint8_t msgSubType, uint16_t outPlayerID, int status);
int gameMsgPacker(char *pPL, Game *pGame, uint16_t toPlayerID, uint8_t msgSubType, uint16_t outPlayerID);
int ackPacker(char *pPL, Game *pGame, uint16_t toPlayerID, int msgSubType, int status);
int statPacker(char *pPL, Game *pGame, uint16_t toPlayerID, int msgSubType);
Player *getPlayer(uint16_t playerID, Player *pPlayer);
void randomLocation(uint16_t *location);
void gameDestructor(Game *pGame);
void signalHandler(int signo);
void checkTimeOut(Game *pGame, char *msgBuffer, int socket, socklen_t addrlen);
