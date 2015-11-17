#include "server_unpackers.h"
#define UNKNOWNT 99 /* Unkonw msg type */

int unpackPacket(char *buf){
  /* Get user id from packet */
  int index = 0, msgtype = UNKNOWNT;
  uint16_t uid = ntohs(*(uint16_t*)&buf[index]);

  /* Get Game time from packet */
  index += sizeof(uint16_t);
  uint32_t gameTime = ntohl(*(uint32_t*)&buf[index]);
}

int getMsgType(char *msg){

}

/*//PACK
*(uint16_t*)&buffer_holding_data[position_to_store] = htons(value_to_store);

//UNPACK
uint16_t retrieved_value = ntohs(*(uint16_t*)&buffer_holding_data[position_from_to_retrieve]);*/

/* Main function to shut the compiler up */
int main(){}
