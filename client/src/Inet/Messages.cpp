#include <iostream>
#include <cstring>

#include "Inet/Messages.hpp"

IMessage::IMessage() {
}
IMessage::~IMessage() {
}

Message* MessageFactory::getMessageByType(MessageHeader * header,
		uint8_t * payload) {

	// TODO Make better solution here
	GameMessage* message;

	switch (header->message_type) {
	case MESSAGE_TYPE::GAME_MESSAGE:
		message = GameMessage::Unpack(*header, header->payload_length, payload);
		switch (message->getMessageType()) {
		case GAME_MESSAGE_TYPE::JOIN:
			return dynamic_cast<Join*>(message);
		case GAME_MESSAGE_TYPE::NICK:
			return dynamic_cast<Nick*>(message);
		case GAME_MESSAGE_TYPE::EXIT:
			return dynamic_cast<Exit*>(message);
		case GAME_MESSAGE_TYPE::RESTART:
			return dynamic_cast<Restart*>(message);
		case GAME_MESSAGE_TYPE::GAME_END:
			return dynamic_cast<GameEnd*>(message);
		case GAME_MESSAGE_TYPE::GAME_UPDATE:
			return dynamic_cast<GameUpdate*>(message);
		case GAME_MESSAGE_TYPE::POINTS:
			return dynamic_cast<Points*>(message);
		case GAME_MESSAGE_TYPE::PLAYER_DEAD:
			return dynamic_cast<PlayerDead*>(message);
		case GAME_MESSAGE_TYPE::PLAYER_OUT:
			return dynamic_cast<PlayerOut*>(message);
		default:
			return nullptr;
		}
		break;
	case MESSAGE_TYPE::ACK:

		break;
	case MESSAGE_TYPE::PLAYER_MOVEMENT:
		return nullptr;
	case MESSAGE_TYPE::PLAYER_CHAT_MESSAGE:
		break;
	case MESSAGE_TYPE::STATISTICS_MESSAGE:
		break;

	default:
		return nullptr;
	}
	return nullptr;
}

void Message::UnpackHeader(int socket_fd, struct MessageHeader *header,
		uint8_t* payload) {
	int bufferPosition = 0;
	uint8_t socketByteBuffer[BUFFER_SIZE];
	memset(socketByteBuffer, 0, sizeof(struct MessageHeader));
	header->addrlen = sizeof(header->sender_addr);

	int bytesRead = recvfrom(socket_fd, socketByteBuffer, BUFFER_SIZE, 0,
			reinterpret_cast<struct sockaddr*>(&header->sender_addr),
			&header->addrlen);
	if (bytesRead > 0) {

		// Unpack USER_ID (UINT_16)
		header->user_id = UnpackUINT16_T(socketByteBuffer, bufferPosition);
		std::cout << "Message.cpp: USER_ID: " << header->user_id << std::endl;
		bufferPosition += sizeof(uint16_t);

		// Unpack GAME_TIME (UINT_32)
		header->game_time = UnpackUINT16_T(socketByteBuffer, bufferPosition);
		std::cout << "Message.cpp: GAME TIME: " << header->game_time << std::endl;
		bufferPosition += sizeof(uint32_t);

		// Unpack MESSAGE_TYPE (UINT_8)
		header->message_type = UnpackUINT8_T(socketByteBuffer, bufferPosition);
		std::cout << "Message.cpp: Message type: " << unsigned(header->message_type)	<< std::endl;
		bufferPosition += sizeof(uint8_t);

		// Unpack PAYLOAD_LENGTH (UINT_32)
		header->payload_length = UnpackUINT32_T(socketByteBuffer, bufferPosition);
		std::cout << "Message.cpp: Payload length: " << header->payload_length << std::endl;
		bufferPosition += sizeof(uint32_t);

		// TODO Will I miss the last meaningful byte?
		// return remaining of the received message
		memcpy(payload, &socketByteBuffer[bufferPosition], header->payload_length);
	} else {
		std::cout << "Message.cpp: Nothing read from recvfrom()" << std::endl;
	}

	char s[INET_ADDRSTRLEN];
	std::cout << "Message.cpp: Got message from "
			<< inet_ntop(header->sender_addr.ss_family,
					reinterpret_cast<const void*>(&reinterpret_cast<struct sockaddr_in*>(reinterpret_cast<struct sockaddr*>(&header->sender_addr))->sin_addr),
					s, sizeof s) << std::endl;
}

//======= GAME_MESSAGE ========//
uint8_t GameMessage::UnpackHeader(uint8_t * payload) {
	// Unpack MSG_SUBTYPE (UINT_8)
	uint8_t messageSubtype;
	memcpy(&messageSubtype, payload, sizeof(uint8_t));
	std::cout << "Message.cpp: Message subtype" << messageSubtype << std::endl;
	return 1;
}

GameMessage* GameMessage::Unpack(MessageHeader header, uint32_t length, uint8_t * payload) {
	int readByteCount = 0;

	// Unpack MSG_SUBTYPE (UINT_8)
	uint8_t messageSubtype;
	memcpy(&messageSubtype, payload, sizeof(uint8_t));
	std::cout << "Message.cpp: Message subtype" << messageSubtype << std::endl;
	readByteCount += sizeof(uint8_t);

	// Unpack PAYLOAD_LENGTH (UINT_32)
	uint32_t payload_length;
	memcpy(&payload_length, &payload[readByteCount], sizeof(uint32_t));
	payload_length = ntohl(payload_length);
	std::cout << "Message.cpp: Payload length: " << payload_length << std::endl;
	readByteCount += sizeof(uint32_t);

	// Copy rest of the payload to new variable and pass it to next Unpacker
	uint32_t remainingPayloadLength = length - readByteCount;
	uint8_t * remainingPayload = static_cast<uint8_t *>(malloc(
			remainingPayloadLength));
	memcpy(remainingPayload, &payload[readByteCount], remainingPayloadLength);

	switch (messageSubtype) {
	case GAME_MESSAGE_TYPE::JOIN:
		return Join::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::NICK:
		return Nick::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::EXIT:
		return Exit::Unpack(header ,remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::GAME_END:
		return GameEnd::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::GAME_UPDATE:
		return GameMessage::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::POINTS:
		return Points::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::PLAYER_DEAD:
		return PlayerDead::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::PLAYER_OUT:
		return PlayerOut::Unpack(header, remainingPayloadLength, remainingPayload);
	default:
		return nullptr;
	}
	return nullptr;
}

void GameMessage::Pack(Message* message) {


}

//======= NICK ========//
Nick * Nick::Unpack(MessageHeader header, uint32_t length, uint8_t * payload) {
	Nick * player_nick = new Nick(header);

	// Unpack Nick (char [11])
	memcpy(&player_nick->nick, payload, length);
	player_nick->nick[player_nick->nick.length()] = '\0';
	std::cout << "Message.cpp: New player named" << player_nick->nick << " joined"
			<< std::endl;
	return player_nick;

}

//======= GAME_UPDATE ========//
GameUpdate * GameUpdate::Unpack(MessageHeader header, uint32_t length, uint8_t * payload) {
	int bufferPosition = 0;

	// Unpack OWN_POS_X
	uint16_t pos_x = UnpackUINT16_T(payload, bufferPosition);
	bufferPosition += sizeof(uint16_t);

	// Unpack OWN_POS_Y
	uint16_t pos_y = UnpackUINT16_T(payload, bufferPosition);
	bufferPosition += sizeof(uint16_t);

	// Unpack OWN_DIR_X
	uint16_t dir_x = UnpackUINT16_T(payload, bufferPosition);
	bufferPosition += sizeof(uint16_t);

	// Unpack OWN_DIR_Y
	uint16_t dir_y = UnpackUINT16_T(payload, bufferPosition);
	bufferPosition += sizeof(uint16_t);

	// Unpack PLAYER_COUNT
	uint16_t number_of_players = UnpackUINT16_T(payload, bufferPosition);
	bufferPosition += sizeof(uint16_t);

	// Unpack OBJECT_COUNT
	uint16_t number_of_objects = UnpackUINT16_T(payload, bufferPosition);
	bufferPosition += sizeof(uint16_t);

	// TODO Will I miss the last meaningful byte?
	// return remaining of the received message
	uint8_t * remainingPayload = static_cast<uint8_t *>(malloc(length - bufferPosition));
	memcpy(remainingPayload, &payload[bufferPosition], length - bufferPosition);

	return new GameUpdate(header, pos_x, pos_y, dir_x, dir_y, number_of_players, number_of_objects);

}

//======= POINTS ========//
Points * Points::Unpack(MessageHeader header, uint32_t length, uint8_t * payload) {
	Points * pointScoreObject = new Points(header);
	int readByteCount = 0;

	// Unpack player count (UINT_16)
	uint16_t player_count;
	memcpy(&player_count, payload, sizeof(uint16_t));
	std::cout << "Message.cpp: Point object count" << player_count << std::endl;
	readByteCount += sizeof(uint16_t);

	// Unpack player id and points to vector(UINT_16 and UINT_32)
	for (int i = 0; i < player_count; i++) {
		uint16_t player_id;
		uint32_t player_point;

		memcpy(&player_id, payload, sizeof(uint16_t));
		readByteCount += sizeof(uint16_t);
		pointScoreObject->player_ids.push_back(ntohs(player_id));
		readByteCount += sizeof(uint16_t);

		memcpy(&player_id, payload, sizeof(uint16_t));
		pointScoreObject->player_points.push_back(ntohl(player_point));
		readByteCount += sizeof(uint32_t);

		std::cout << "Message.cpp: Player " << player_id << " got " << player_point
				<< " points." << std::endl;

	}

	return pointScoreObject;

}
//
////======= PLAYER_DEAD ========//
//PlayerDead * PlayerDead::Unpack(uint32_t length, uint8_t * payload) {
//	// Unpack player id (UINT_16)
//	uint16_t playerID = UnpackUINT16_T(payload, 0);
//	std::cout << "Message.cpp: Played id: " << playerID << " died" << std::endl;
//
//	return new PlayerDead(playerID);
//}
//
////======= PLAYER_OUT ========//
//PlayerOut * PlayerOut::Unpack(uint32_t length, uint8_t * payload) {
//	// Unpack player id (UINT_16)
//	uint16_t playerID = UnpackUINT16_T(payload, 0);
//	std::cout << "Message.cpp: Played id: " << playerID << " had disappeared" << std::endl;
//
//	return new PlayerOut(playerID);
//}

