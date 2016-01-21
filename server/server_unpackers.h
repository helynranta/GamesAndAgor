#include "structs.h"

struct Packet unpackPacket(char *msg, struct sockaddr *from);

#define MAX_NICK 12

/*Struct for game msg/incoming packet */
struct Packet{
  int error;
  uint8_t subType;
  uint16_t posX, posY;
  uint16_t dirX, dirY;
  uint8_t ACKTYPE;
  char nick[MAX_NICK];
  uint16_t pingID;
  uint16_t ping;

  uint32_t gameTime; /* also known as ACK_PACKET_ID xD */
  uint16_t ID;
  uint8_t msgType;
	uint32_t payloadLength;
	struct sockaddr senderAddr;

};

enum ackType {JOIN_ACK, NICK_ACK, GAME_END_ACK, PLAYER_OUT_ACK, EXIT_ACK,};
//enum gameSubType {JOIN, NICK, EXIT};
