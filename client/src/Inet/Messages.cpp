#include <iostream>
#include <cstring>

#include "Inet/Messages.hpp"

IMessage::IMessage() {
}
IMessage::~IMessage() {
}

Message* MessageFactory::getMessageByType(struct MessageHeader * header,
		uint8_t * payload) {

	GameMessage* message = GameMessage::Unpack(header->payload_length, payload);

	switch (header->message_type) {
	case MESSAGE_TYPE::GAME_MESSAGE:
		switch (message->getMessageType()) {
		case GameMessage::JOIN:
			return dynamic_cast<Join*>(message);
		case GameMessage::NICK:
			return dynamic_cast<Nick*>(message);
		case GameMessage::EXIT:
			return dynamic_cast<Exit*>(message);
		case GameMessage::RESTART:
			return dynamic_cast<Restart*>(message);
		case GameMessage::GAME_END:
			return dynamic_cast<GameEnd*>(message);
		case GameMessage::GAME_UPDATE:
			return dynamic_cast<GameUpdate*>(message);
		case GameMessage::POINTS:
			return dynamic_cast<Points*>(message);
		case GameMessage::PLAYER_DEAD:
			return dynamic_cast<PlayerDead*>(message);
		case GameMessage::PLAYER_OUT:
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
		return new GameMessage();
	}
	return new GameMessage();
}

void Message::UnpackHeader(int socket_fd, struct MessageHeader *header,
		uint8_t* payloadBuffer) {
	uint8_t byteBuffer[BUFFER_SIZE];
	int readByteCount = 0;
	memset(byteBuffer, 0, BUFFER_SIZE);
	memset(header, 0, sizeof(struct MessageHeader));
	header->addrlen = sizeof(header->sender_addr);

	int bytesRead = recvfrom(socket_fd, byteBuffer, BUFFER_SIZE, 0,
			reinterpret_cast<struct sockaddr*>(&header->sender_addr),
			&header->addrlen);
	if (bytesRead > 0) {

		// Unpack USER_ID (UINT_16)
		uint16_t uint16_tmp;
		memcpy(&uint16_tmp, &byteBuffer[readByteCount], sizeof(uint16_t));
		header->user_id = ntohs(uint16_tmp);
		std::cout << "USER_ID: " << header->user_id << std::endl;
		readByteCount += sizeof(uint16_t);
		std::cout << "Location pointer: " << readByteCount << std::endl;

		// Unpack GAME_TIME (UINT_32)
		uint32_t gameTime;
		memcpy(&gameTime, &byteBuffer[readByteCount], sizeof(uint32_t));
		header->game_time = ntohl(gameTime);
		std::cout << "GAME TIME: " << header->game_time << std::endl;
		readByteCount += sizeof(uint32_t);
		std::cout << "Location pointer: " << readByteCount << std::endl;

		// Unpack MESSAGE_TYPE (UINT_8)
		memcpy(&header->message_type, &byteBuffer[readByteCount],
				sizeof(uint8_t));
		std::cout << "Message type: " << unsigned(header->message_type)
				<< std::endl;
		readByteCount += sizeof(uint8_t);
		std::cout << "Location pointer: " << readByteCount << std::endl;

		// Unpack PAYLOAD_LENGTH (UINT_32)
		uint32_t payloadLength;
		memcpy(&payloadLength, &byteBuffer[readByteCount], sizeof(uint32_t));
		header->payload_length = ntohl(payloadLength);
		std::cout << "Payload length: " << header->payload_length << std::endl;
		readByteCount += sizeof(uint32_t);
		std::cout << "Location pointer: " << readByteCount << std::endl;

		// TODO Will I miss the last meaningful byte?
		memcpy(payloadBuffer, &byteBuffer[readByteCount],
				header->payload_length);
	} else {
		std::cout << "Nothing read from recvfrom()" << std::endl;
	}

	char s[INET_ADDRSTRLEN];
	std::cout << "Got message from "
			<< inet_ntop(header->sender_addr.ss_family,
					reinterpret_cast<const void*>(&reinterpret_cast<struct sockaddr_in*>(reinterpret_cast<struct sockaddr*>(&header->sender_addr))->sin_addr),
					s, sizeof s) << std::endl;
}

//======= GAME_MESSAGE ========//
uint8_t GameMessage::UnpackHeader(uint8_t * payload) {
	// Unpack MSG_SUBTYPE (UINT_8)
	uint8_t messageSubtype;
	memcpy(&messageSubtype, payload, sizeof(uint8_t));
	std::cout << "Message subtype" << messageSubtype << std::endl;
	return 1;
}

GameMessage* GameMessage::Unpack(uint32_t length, uint8_t * payload) {
	int readByteCount = 0;

	// Unpack MSG_SUBTYPE (UINT_8)
	uint8_t messageSubtype;
	memcpy(&messageSubtype, payload, sizeof(uint8_t));
	std::cout << "Message subtype" << messageSubtype << std::endl;
	readByteCount += sizeof(uint8_t);

	// Unpack PAYLOAD_LENGTH (UINT_32)
	uint32_t payload_length;
	memcpy(&payload_length, &payload[readByteCount], sizeof(uint32_t));
	payload_length = ntohl(payload_length);
	std::cout << "Payload length: " << payload_length << std::endl;
	readByteCount += sizeof(uint32_t);

	// Copy rest of the payload to new variable and pass it to next Unpacker
	uint32_t remainingPayloadLength = length - readByteCount;
	uint8_t * remainingPayload = static_cast<uint8_t *>(malloc(
			remainingPayloadLength));
	memcpy(remainingPayload, &payload[readByteCount],
			remainingPayloadLength);

	switch (messageSubtype) {
	case GameMessage::JOIN:
		return new Join();
	case GameMessage::NICK:
		return Nick::Unpack(remainingPayloadLength, remainingPayload);
	case GameMessage::EXIT:
		return new Exit();
	case GameMessage::GAME_END:
		return new GameEnd();
	case GameMessage::GAME_UPDATE:
		return GameMessage::Unpack(remainingPayloadLength, remainingPayload);
	case GameMessage::POINTS:
		return Points::Unpack(remainingPayloadLength, remainingPayload);
	case GameMessage::PLAYER_DEAD:
		return PlayerDead::Unpack(remainingPayloadLength, remainingPayload);
	case GameMessage::PLAYER_OUT:
		return PlayerOut::Unpack(remainingPayloadLength, remainingPayload);
	default:
		return nullptr;
	}
	return nullptr;
}

void GameMessage::Pack(Message* message){
<<<<<<< HEAD
	GameMessage * gameMessage = dynamic_cast<GameMessage*>(message);
=======
	GameMessage gameMessage = dynamic_cast<GameMessage>(message)
>>>>>>> 44436417565b53ccb671b97ca896a31476d72059
}

//======= NICK ========//
Nick * Nick::Unpack(uint32_t length, uint8_t * payload) {
	Nick * player_nick = new Nick();

	// Unpack Nick (char [11])
	memcpy(&player_nick->nick, payload, length);
	player_nick->nick[player_nick->nick.length()] = '\0';
	std::cout << "New player named" << player_nick->nick << " joined"
			<< std::endl;
	return player_nick;

}

//======= GAME_UPDATE ========//
GameUpdate * GameUpdate::Unpack(uint32_t length, uint8_t * payload) {
	return new GameUpdate();

}

//======= POINTS ========//
Points * Points::Unpack(uint32_t length, uint8_t * payload) {
	Points * pointScoreObject = new Points();
	int readByteCount = 0;

	// Unpack player count (UINT_16)
	uint16_t player_count;
	memcpy(&player_count, payload, sizeof(uint16_t));
	std::cout << "Point object count" << player_count << std::endl;
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

		std::cout << "Player " << player_id << " got " << player_point
				<< " points." << std::endl;

	}

	return pointScoreObject;

}

//======= PLAYER_DEAD ========//
PlayerDead * PlayerDead::Unpack(uint32_t length, uint8_t * payload) {
	return new PlayerDead();
}

//======= PLAYER_OUT ========//
PlayerOut * PlayerOut::Unpack(uint32_t length, uint8_t * payload) {
	return new PlayerOut();
}
