/*
 * MessagesAck.cpp
 *
 *  Created on: Feb 1, 2016
 *      Author: ode
 */

#include "Inet/MessagesAck.hpp"

// Join functions
int JoinAck::PackSelf(uint8_t* payload) {

	int bufferPosition = getHeaderSize();

	// insert MSG_SUBTYPE to buffer
	PackUINT8ToPayload(static_cast<uint8_t>(getGameMessageType()), payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint8_t));

	CreateHeader(this, payload);

	return bufferPosition;
}

JoinAck* JoinAck::Unpack(MessageHeader header, uint32_t length, uint8_t* payload) {
	int bufferPosition = 0;

	uint8_t status = UnpackUINT8_T(payload, bufferPosition);
	bufferPosition += sizeof(uint8_t);

	uint8_t  id = UnpackUINT16_T(payload, bufferPosition);
	bufferPosition += sizeof(uint16_t);

	JoinAck* playerJoin = new JoinAck(header, status, id);

	std::cout << "MessagesAck.cpp: Got user ID " << playerJoin->id << " from server" << std::endl;

	return playerJoin;
}


// Nick functions
int NickAck::PackSelf(uint8_t* payload) {

	int bufferPosition = getHeaderSize();

	// insert MSG_SUBTYPE to buffer
	PackUINT8ToPayload(static_cast<uint8_t>(getGameMessageType()), payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint8_t));

	CreateHeader(this, payload);

	return bufferPosition;
}

NickAck* NickAck::Unpack(MessageHeader header, uint32_t length, uint8_t* payload) {


	int bufferPosition = 0;

	uint8_t status = UnpackUINT8_T(payload, bufferPosition);
	bufferPosition += sizeof(uint8_t);

	NickAck* playerNick = new NickAck(header, status);

	return playerNick;
}


// Exit functions
int ExitAck::PackSelf(uint8_t* payload) {

	int bufferPosition = getHeaderSize();

	// insert MSG_SUBTYPE to buffer
	PackUINT8ToPayload(static_cast<uint8_t>(getGameMessageType()), payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint8_t));

	CreateHeader(this, payload);

	return bufferPosition;
}

ExitAck* ExitAck::Unpack(MessageHeader header, uint32_t length, uint8_t* payload) {
	ExitAck* playerExit = new ExitAck(header);
	return playerExit;
}


// Restart functions
int RestartAck::PackSelf(uint8_t* payload) {

	int bufferPosition = getHeaderSize();

	// insert MSG_SUBTYPE to buffer
	PackUINT8ToPayload(static_cast<uint8_t>(getGameMessageType()), payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint8_t));

	CreateHeader(this, payload);

	return bufferPosition;
}

RestartAck* RestartAck::Unpack(MessageHeader header, uint32_t length, uint8_t* payload) {
	RestartAck* playerRestart = new RestartAck(header);
	return playerRestart;
}


// Game end functions
int GameEndAck::PackSelf(uint8_t* payload) {

	int bufferPosition = getHeaderSize();

	// insert MSG_SUBTYPE to buffer
	PackUINT8ToPayload(static_cast<uint8_t>(getGameMessageType()), payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint8_t));

	CreateHeader(this, payload);

	return bufferPosition;
}

GameEndAck* GameEndAck::Unpack(MessageHeader header, uint32_t length, uint8_t* payload) {
	GameEndAck* playerGameEnd = new GameEndAck(header);
	return playerGameEnd;
}


// Player dead functions
int PlayerDeadAck::PackSelf(uint8_t* payload) {

	int bufferPosition = getHeaderSize();

	// insert MSG_SUBTYPE to buffer
	PackUINT8ToPayload(static_cast<uint8_t>(getGameMessageType()), payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint8_t));

	CreateHeader(this, payload);

	return bufferPosition;
}

PlayerDeadAck* PlayerDeadAck::Unpack(MessageHeader header, uint32_t length, uint8_t* payload) {
	PlayerDeadAck* playerDead = new PlayerDeadAck(header);
	return playerDead;
}


// Player out functions
int PlayerOutAck::PackSelf(uint8_t* payload) {

	int bufferPosition = getHeaderSize();

	// insert MSG_SUBTYPE to buffer
	PackUINT8ToPayload(static_cast<uint8_t>(getGameMessageType()), payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint8_t));

	CreateHeader(this, payload);

	return bufferPosition;
}

PlayerOutAck* PlayerOutAck::Unpack(MessageHeader header, uint32_t length, uint8_t* payload) {
	PlayerOutAck* playerOut = new PlayerOutAck(header);
	return playerOut;
}
