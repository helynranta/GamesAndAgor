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
// #define PAYLOAD_INDEX




void ComputeNearParticles(Player *sPlayers, Object *sObjects);
int isWithinRange(int location1[2], int location2[2], int scale1, int scale2);
void eventEat(Player *eater, Player *eaten);
void addAck2List(Ack **pAckList, char *msg, int gameTime, int msgLength, int packetID);
void append2ListAck(Ack **pList, Ack *pNew);
void removeAck(Ack **pList, uint32_t ackID);
void append2ListNear(Near **pList, Near *pNew);
void append2ListPlayer(Player **pList, Player *pNew);
void append2ListObject(Object **pList, Object *pNew);
void clearListNear(Near **pList);
void clearListPlayer(Player **pList);
void clearListObject(Object **pList);
void newPlayer(Player **pList, struct Packet packet, int nPlayers);

/* ltr did this, blame me if something bad happens */
void sendGameUpdate(Game *game, char *buf, int socket, socklen_t addrlen);
int checkNick(char *nick, Player *playerlist);


/* PACKING FUNCTIONS */
int msgPacker(char *msgBuffer, Game *pGame, int toPlayerID, int msgType,
    int msgSubType, int outPlayerID, int status);
int gameMsgPacker(char *pPL, Game *pGame, int toPlayerID,
    int msgSubType, int outPlayerID);
int ackPacker(char *pPL, Game *pGame, int toPlayerID, int msgSubType, int status);
int statPacker(char *pPL, Game *pGame, int toPlayerID, int msgSubType);
Player *getPlayer(int playerID, Player *pPlayer);
void randomLocation(int *location);
