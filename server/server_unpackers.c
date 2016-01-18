#include "server_unpackers.h"
#define UNKNOWNT 99 /* Unknown msg type */


/* unpack packet from network */
struct Packet unpackPacket(char *buf, struct sockaddr_storage *from){

  int index = 0;

  /* uid */
  uint16_t uid = ntohs(*(uint16_t*)&buf[index]);
  index += sizeof(uint16_t);

  /* game time */
  uint32_t gameTime = ntohl(*(uint32_t*)&buf[index]);
  index += sizeof(uint32_t);

  /* message type */
  uint8_t msgType = ntohs(*(uint8_t*)&buf[index]);
  index += sizeof(uint8_t);

  /* payload length */
  uint32_t payloadLength = ntohl(*(uint32_t*)&buf[index]);
  index += sizeof(uint32_t);

  uint8_t subtype;
  struct Packet packet;

  /* General initialization for packet */
  packet.ID = uid;
  packet.gameTime = gameTime;
  packet.senderAddr = *from;
  packet.msgType = msgType;

  /* Check msg type and fill the struct based on it */
  switch (msgType) {
    case GAME:
      packet.msgType = GAME;
      subtype = ntohs(*(uint8_t*)&buf[index]);
      index += sizeof(uint8_t);

      /* */
      switch (subtype) {
        case JOIN:
          packet.subType = JOIN;
          return packet;
          break;

        case NICK:
          packet.subType = NICK;
          memcpy(packet.nick, &buf[index], MAX_NICK);
          return packet;
          break;

        case EXIT:
          packet.subType = EXIT;
          return packet;
          break;
      }
      break;

    case ACK:
      packet.msgType = ACK;
      index += sizeof(uint32_t); /* skip irrelevant packet_id */
      packet.ACKTYPE = ntohs(*(uint8_t*)&buf[index]);

      switch (packet.ACKTYPE) {
        case JOIN_ACK:
          return packet;
          break;
        case NICK_ACK:
          return packet;
          break;
      }

      break;

    case MOVEMENT:
      break;

    case STAT:
      break;

    default:
      printf("Unknown msg\n");
  }

}



/*//PACK
*(uint16_t*)&buffer_holding_data[position_to_store] = htons(value_to_store);

//UNPACK
uint16_t retrieved_value = ntohs(*(uint16_t*)&buffer_holding_data[position_from_to_retrieve]);*/

/* Main function to shut the compiler up */
int main(){}
/*
/* Message types
enum msg {GAME, ACK, MOVEMENT, CHAT, STAT};

/* Game message types
enum gameMsg {JOIN, NICK, WAIT, EXIT, INIT, START, STOP, GAME_END, GAME_UPDATE};
*/
