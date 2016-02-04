#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H
#include "structs.h"
#endif

struct Packet unpackPacket(char *msg, struct sockaddr *from, int s, socklen_t len);




enum ackType {JOIN_ACK, NICK_ACK, GAME_END_ACK, PLAYER_OUT_ACK, EXIT_ACK,};
//enum gameSubType {JOIN, NICK, EXIT};
