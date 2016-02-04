/*
 * MessagesAck.cpp
 *
 *  Created on: Feb 1, 2016
 *      Author: ode
 */

#include "Inet/MessagesAck.hpp"


MessagesAck* MessagesAck::Unpack(MessageHeader header, uint32_t length, uint8_t * payload) {
//	std::cout << "========== UNPACK_MESSAGE_ACK_HEADER ==========" << std::endl;
//	std::cout << "MessageType: " << unsigned(header.message_type) << std::endl;
//	std::cout << "Length: " << length << std::endl;


	int readByteCount = 0;

	// Unpack PAYLOAD_LENGTH (UINT_32)
	uint32_t packetID;
	memcpy(&packetID, &payload[readByteCount], sizeof(uint32_t));
	packetID = ntohl(packetID);
//	std::cout << "MessageAck.cpp: Packet_ID: " << packetID << std::endl;
	readByteCount += sizeof(uint32_t);

	// Unpack MSG_SUBTYPE (UINT_8)
	uint8_t messageSubtype;
	memcpy(&messageSubtype, payload, sizeof(uint8_t));
//	std::cout << "MessageAck.cpp: Message subtype " << getSubMessageTypeAsString(messageSubtype) << std::endl;
	readByteCount += sizeof(uint8_t);

	// Copy rest of the payload to new variable and pass it to next Unpacker
	uint32_t remainingPayloadLength = length - readByteCount;
	uint8_t * remainingPayload = static_cast<uint8_t *>(malloc(remainingPayloadLength));
	memcpy(remainingPayload, &payload[readByteCount], remainingPayloadLength);

	switch (messageSubtype) {
	case GAME_MESSAGE_TYPE::JOIN:
//		std::cout << "ARGH ->" << getSubMessageTypeAsString(messageSubtype) << std::endl;
		return JoinAck::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::NICK:
		return NickAck::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::EXIT:
		return ExitAck::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::GAME_END:
		return GameEndAck::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::PLAYER_DEAD:
		return PlayerDeadAck::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::PLAYER_OUT:
		return PlayerOutAck::Unpack(header, remainingPayloadLength, remainingPayload);
	default:
		return nullptr;
	}
//	std::cout << "======== UNPACK_MESSAGE_ACK_HEADER_END ========" << std::endl;
	return nullptr;
}

// Join functions
int JoinAck::PackSelf(uint8_t* payload) {
	std::cout << "Sending -> AKC: " << getSubMessageTypeAsString(gameMessageType) << std::endl;

	int bufferPosition = getHeaderSize();

	// insert MSG_SUBTYPE to buffer
	PackUINT8ToPayload(static_cast<uint8_t>(getGameMessageType()), payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint8_t));
	CreateHeader(this, payload);
	return bufferPosition;
}

JoinAck* JoinAck::Unpack(MessageHeader header, uint32_t length, uint8_t* payload) {
	std::cout << "Receiving -> ACK: " << getSubMessageTypeAsString(header.message_type) << std::endl;

	int bufferPosition = 0;

	uint8_t status = UnpackUINT8_T(payload, bufferPosition);
	bufferPosition += sizeof(uint8_t);

	uint8_t  id = UnpackUINT16_T(payload, bufferPosition);
	bufferPosition += sizeof(uint16_t);

	JoinAck* playerJoin = new JoinAck(header, status, id);

//	std::cout << "MessagesAck.cpp: Got user ID " << playerJoin->id << " from server" << std::endl;

	return playerJoin;
}


// Nick functions
int NickAck::PackSelf(uint8_t* payload) {
	std::cout << "Sending -> AKC: " << getSubMessageTypeAsString(gameMessageType) << std::endl;

	int bufferPosition = getHeaderSize();

	// insert MSG_SUBTYPE to buffer
	PackUINT8ToPayload(static_cast<uint8_t>(getGameMessageType()), payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint8_t));

	CreateHeader(this, payload);

	return bufferPosition;
}

NickAck* NickAck::Unpack(MessageHeader header, uint32_t length, uint8_t* payload) {
	std::cout << "Receiving -> ACK: " << getSubMessageTypeAsString(header.message_type) << std::endl;

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
	std::cout << "Sending -> AKC: " << getSubMessageTypeAsString(gameMessageType) << std::endl;

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
