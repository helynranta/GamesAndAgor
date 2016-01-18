#include "structs.h"
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
void headerPacker(int playerID, int gameTime, int msgType, int payloadLength, char **msgBuffer);
