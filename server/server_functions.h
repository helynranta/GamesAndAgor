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
// #define PAYLOAD_INDEX


void ComputeNearParticles(Player *sPlayers, Object *sObjects);
int isWithinRange(int location1[2], int location2[2], int scale);
void append2ListNear(Near **pList, Near *pNew);
void append2ListPlayer(Player **pList, Player *pNew);
void append2ListObject(Object **pList, Object *pNew);
void clearListNear(Near **pList);
void clearListPlayer(Player **pList);
void clearListObject(Object **pList);
void newPlayer(Player **pList);

/* PACKING FUNCTIONS */
int msgPacker(char **msgBuffer, Game *pGame, int toPlayerID, int msgType,
    int msgSubType, int outPlayerID, int status);
int gameMsgPacker(char **pPL, Game *pGame, int toPlayerID,
    int msgSubType, int outPlayerID);
int ackPacker(char **pPL, Game *pGame, int toPlayerID, int msgSubType, int status);
int statPacker(char **pPL, Game *pGame, int toPlayerID, int msgSubType);
Player *getPlayer(int playerID, Player *pPlayer);
