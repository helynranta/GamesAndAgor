#include "server_unpackers.h"
#define UNKNOWNT 99 /* Unknown msg type */


/* unpack packet from network
   socket and len only used when client sends STATISTICS_MESSAGE, so we'll
   reply it back straight away without changing anything
*/
struct Packet unpackPacket(char *buf, struct sockaddr *from, int socket, socklen_t addrlen){

  int index = 0;

  /* uid */
  uint16_t uid = ntohs(*(uint16_t*)&buf[index]);
  index += sizeof(uint16_t);

  /* game time */
  uint32_t gameTime = ntohl(*(uint32_t*)&buf[index]);
  index += sizeof(uint32_t);

  /* message type */
  uint8_t msgType = *(uint8_t*)&buf[index];
  index += sizeof(uint8_t);

  /* payload length */
  //uint32_t payloadLength = ntohl(*(uint32_t*)&buf[index]);
  index += sizeof(uint32_t);

  /* message subtype */
  int subtype;
  struct Packet packet;

  /* General initialization for packet */
  packet.ID = uid;
  packet.gameTime = gameTime;
  packet.senderAddr = *from;
  packet.msgType = msgType;
  packet.error = 0;

  /* Check msg type and fill the struct based on it */
  switch (msgType) {
    case GAME_MESSAGE:
      packet.msgType = GAME_MESSAGE;
      subtype = *(uint8_t*)&buf[index];
      index += sizeof(uint8_t);
      printf("In unpackers_ subtype: %d\n", subtype);

      /* GAME msg subtypes */
      switch (subtype) {
        case JOIN:
          packet.subType = JOIN;
          return packet;
          break;

        case NICK:
          packet.subType = NICK;
          memcpy(packet.nick, &buf[index], MAX_NICK);
          packet.nick[11] = '\0';
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
      packet.ackID = ntohl(*(uint32_t*)&buf[index]);
      index += sizeof(uint32_t);
      packet.ACKTYPE = *(uint8_t*)&buf[index];

      switch (packet.ACKTYPE) {
        case JOIN:
        case NICK:
        case EXIT:
        case GAME_END:
        case POINTS:
        case PLAYER_DEAD:
        case PLAYER_OUT:
        case PING:
          /* move pointer past payloadlength */
          index += sizeof(uint32_t);

          /* next read  */
          return packet;
          break;
      }

      break;

    case PLAYER_MOVEMENT:
      packet.msgType = PLAYER_MOVEMENT;

      /* skip irrelevant info */
      index += sizeof(uint8_t);

      /* Get the player's position and direction */
      packet.posX = ntohs(*(uint16_t*)&buf[index]);
      index += sizeof(uint16_t);

      packet.posY = ntohs(*(uint16_t*)&buf[index]);
      index += sizeof(uint16_t);

      packet.dirX = ntohs(*(uint16_t*)&buf[index]);
      index += sizeof(uint16_t);

      packet.dirY = ntohs(*(uint16_t*)&buf[index]);

      return packet;
      break;

    case STATISTICS_MESSAGE:

      /* Send the buffer right back */
      sendto(socket, buf, strlen(buf), 0, from, addrlen);
      packet.msgType = STATISTICS_MESSAGE;
      /* Statistics a.k.a ping message */
      /* ping is in milliseconds */
      //packet.pingID = ntohs(*(uint16_t*)&buf[index]);
      //index += sizeof(uint16_t);
      //packet.ping = ntohs(*(uint16_t*)&buf[index]);

      return packet;
      break;

    default:
      printf("Unknown msg\n");
  }

  /* If we get here, error has occured */
  packet.error=1;
  return packet;

}


/*//PACK
*(uint16_t*)&buffer_holding_data[position_to_store] = htons(value_to_store);

//UNPACK
uint16_t retrieved_value = ntohs(*(uint16_t*)&buffer_holding_data[position_from_to_retrieve]);*/
