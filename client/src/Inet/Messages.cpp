#include <iostream>
#include <cstring>

#include "Inet/Messages.hpp"
#include "Inet/MessageAck.hpp"
#include "Inet/InetConnection.hpp"

IMessage::IMessage() {
}
IMessage::~IMessage() {
}

Message* MessageFactory::getMessageByType(MessageHeader * header, uint8_t * payload) {

	// TODO Make better solution here
	Message* message = nullptr;

	#ifdef MESG_TEST
		//	cout << " ================== HEADER INFO ================== "<< endl;
		//	cout << "GameTime: " << header->gameTime << endl;
		//	cout << "UserID: " << header->user_id << endl;
		//	cout << "MessageType: " << getMessageTypeAsString(header->message_type) << endl;
		//	cout << "PayloadLength: " << unsigned(header->payload_length) << endl;
		//	cout << " ================ HEADER INFO END ================ "<< endl;
	#endif

	switch (header->message_type) {
	case MESSAGE_TYPE::GAME_MESSAGE:
		message = GameMessage::Unpack(*header, header->payload_length, payload);
		break;
	case MESSAGE_TYPE::ACK:
		message = MessageAck::Unpack(*header, header->payload_length, payload);
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
	return message;
}

void Message::UnpackHeader(int socket_fd, struct MessageHeader *header, uint8_t* payload) {
	#ifdef MESG_TEST
		//	cout << "============= UNPACK_MESSAGE_HEADER =============" << endl;
	#endif
	int bufferPosition = 0;
	uint8_t socketByteBuffer[BUFFER_SIZE];
	memset(socketByteBuffer, 0, sizeof(struct MessageHeader));
	memset(socketByteBuffer, 0, BUFFER_SIZE);
	header->addrlen = sizeof(header->sender_addr);

	int bytesRead = recvfrom(socket_fd, socketByteBuffer, BUFFER_SIZE, 0, reinterpret_cast<struct sockaddr*>(&header->sender_addr), &header->addrlen);
	if (bytesRead > 0) {

		// Unpack USER_ID (UINT_16)
		header->user_id = UnpackUINT16_T(socketByteBuffer, bufferPosition);
		bufferPosition += sizeof(uint16_t);

		// Unpack GAME_TIME (UINT_32)
		header->gameTime = UnpackUINT32_T(socketByteBuffer, bufferPosition);
		bufferPosition += sizeof(uint32_t);

		// Unpack MESSAGE_TYPE (UINT_8)
		header->message_type = UnpackUINT8_T(socketByteBuffer, bufferPosition);
		bufferPosition += sizeof(uint8_t);

		// Unpack PAYLOAD_LENGTH (UINT_32)
		header->payload_length = UnpackUINT32_T(socketByteBuffer, bufferPosition);
		bufferPosition += sizeof(uint32_t);

		#ifdef MESG_TEST
			//cout << "Message.cpp: USER_ID: " << header->user_id << endl;
			//cout << "Message.cpp: GAME TIME: " << header->gameTime << endl;
			//cout << "Message.cpp: Message type: " << getMessageTypeAsString(header->message_type) << endl;
			//cout << "Message.cpp: Payload length: " << header->payload_length << endl;
		#endif
		memcpy(payload, &socketByteBuffer[bufferPosition], header->payload_length);
	} else {
		std::cout << "Message.cpp: Nothing read from recvfrom()" << std::endl;
	}
	#ifdef MESG_TEST
		//	char s[INET_ADDRSTRLEN];
		//	cout << "Message.cpp: Got message from "
		//			<< inet_ntop(header->sender_addr.ss_family,
		//					reinterpret_cast<const void*>(&reinterpret_cast<struct sockaddr_in*>(reinterpret_cast<struct sockaddr*>(&header->sender_addr))->sin_addr),
		//					s, sizeof s) << endl;
		//	cout << "==================== END ========================" << endl;
	#endif
}

// CreateHeader inserts header information to buffer.
// CreateHeader always writes to the beginning of the given buffer so make sure the object offsets their data so there is enough space for message header.
// One can query header size using int Message::getHeaderSize()
int Message::CreateHeader(Message * message, uint8_t * buffer) {
	int bufferPosition = 0;

	// insert USER_ID to buffer
	uint16_t userID = static_cast<uint16_t>(message->getMessageHeaderUserID());
	PackUINT16ToPayload(userID, buffer, bufferPosition);
	bufferPosition += sizeof(uint16_t);

	// insert GAME_TIME to buffer
	uint32_t gameTime = static_cast<uint32_t>(message->getGameTime());
	PackUINT32ToPayload(gameTime, buffer, bufferPosition);
	bufferPosition += sizeof(uint32_t);

	#ifdef MESG_TEST
		//cout << "MessageType: " << message->getMessageType() << endl;
	#endif
	uint8_t type = static_cast<uint8_t>(message->getMessageType());
	PackUINT8ToPayload(type, buffer, bufferPosition);
	bufferPosition += sizeof(uint8_t);

	// insert payloadSize to buffer
	uint32_t payloadSize = static_cast<uint32_t>(message->getPayloadSize());
	PackUINT32ToPayload(payloadSize, buffer, bufferPosition);
	bufferPosition += sizeof(uint32_t);

	return bufferPosition;
}

//======= GAME_MESSAGE ========//
GameMessage* GameMessage::Unpack(MessageHeader header, uint32_t length, uint8_t * payload) {

	#ifdef MESG_TEST
	//	std::cout << "========== UNPACK_GAME_MESSAGE_HEADER ==========" << std::endl;
	#endif
	int readByteCount = 0;
	// Unpack MSG_SUBTYPE (UINT_8)
	uint8_t messageSubtype;
	memcpy(&messageSubtype, payload, sizeof(uint8_t));
	#ifdef MESG_TEST
	//	std::cout << "Message.cpp - GameMessage::Unpack - Message subtype: " << getSubMessageTypeAsString(messageSubtype) << std::endl;
	#endif
	readByteCount += sizeof(uint8_t);

	//	// Unpack PAYLOAD_LENGTH (UINT_32)
	//	uint32_t payload_length;
	//	memcpy(&payload_length, &payload[readByteCount], sizeof(uint32_t));
	//	payload_length = ntohl(payload_length);
	//	std::cout << "Message.cpp - GameMessage::Unpack - Payload length: " << payload_length << std::endl;
	//	readByteCount += sizeof(uint32_t);

	// Copy rest of the payload to new variable and pass it to next Unpacker
	uint32_t remainingPayloadLength = length - readByteCount;
	uint8_t * remainingPayload = static_cast<uint8_t *>(malloc(remainingPayloadLength));
	memcpy(remainingPayload, &payload[readByteCount], remainingPayloadLength);
	#ifdef MESG_TEST
		std::cout << "Receiving -> GameMessage: " << getSubMessageTypeAsString(messageSubtype) << std::endl;
	#endif
	switch (messageSubtype) {
	case GAME_MESSAGE_TYPE::JOIN:
		return Join::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::NICK:
		return Nick::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::EXIT:
		return Exit::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::GAME_END:
		return GameEnd::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::GAME_UPDATE:
		return GameUpdate::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::POINTS:
		return Points::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::PLAYER_DEAD:
		return PlayerDead::Unpack(header, remainingPayloadLength, remainingPayload);
	case GAME_MESSAGE_TYPE::PLAYER_OUT:
		return PlayerOut::Unpack(header, remainingPayloadLength, remainingPayload);
	default:
		return nullptr;
	}
	#ifdef MESG_TEST
		//cout << "================== UNPACK_GAME_MESSAGE_HEADER_END ==============" << endl;
	#endif
	return nullptr;
}

//======= Join ========//
Join * Join::Unpack(MessageHeader header, uint32_t length, uint8_t * payload) {
	#ifdef MESG_TEST
		cout << "Receiving -> GAME_MESSAGE: " << getSubMessageTypeAsString(header.message_type) << endl;
	#endif
	Join * playerJoin = new Join(header);
	return playerJoin;
}

int Join::PackSelf(uint8_t * payload) {
	#ifdef MESG_TEST
		cout << "Sending -> GAME_MESSAGE: " << getSubMessageTypeAsString(gameMessageType) << endl;
	#endif
	int bufferPosition = getHeaderSize();

	// insert MSG_SUBTYPE to buffer
	#ifdef MESG_TEST
		//	cout << "GameMessageType: " << getGameMessageType() << std::endl;
	#endif
	PackUINT8ToPayload(static_cast<uint8_t>(getGameMessageType()), payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint8_t));

	CreateHeader(this, payload);
	#ifdef MESG_TEST
		//cout << "Whole message size: " << bufferPosition << " and shit: " << this->getPayloadSize() << std::endl;
	#endif
	return bufferPosition;
}

//======= NICK ========//
Nick * Nick::Unpack(MessageHeader header, uint32_t length, uint8_t * payload) {
	#ifdef MESG_TEST
		cout << "Receiving -> GAME_MESSAGE: " << getSubMessageTypeAsString(header.message_type) << endl;
	#endif
	Nick * playerNick = new Nick(header);

	// Unpack Nick (char [11])
	memcpy(&playerNick->nick, payload, sizeOfNick());
	playerNick->nick[sizeOfNick() - 1] = '\0';
	#ifdef MESG_TEST
		//	std::cout << "Message.cpp: New player named" << playerNick->nick << " joined" << std::endl;
	#endif
	return playerNick;
}

int Nick::PackSelf(uint8_t * payload) {
	#ifdef MESG_TEST
		std::cout << "Sending -> GAME_MESSAGE: " << getSubMessageTypeAsString(getGameMessageType()) << std::endl;
	#endif
	int bufferPosition = getHeaderSize();

	// insert MSG_SUBTYPE to buffer
	#ifdef MESG_TEST
		//cout << "Nick - GameMessageType: " << unsigned(getGameMessageType()) << std::endl;
	#endif
	PackUINT8ToPayload(static_cast<uint8_t>(getGameMessageType()), payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint8_t));

	// insert NICK to buffer
	memcpy(payload + bufferPosition, nick, sizeOfNick());
	bufferPosition += addPayloadSize(sizeOfNick());

	CreateHeader(this, payload);
	#ifdef MESG_TEST
		//	std::cout << "Whole message size: " << bufferPosition << " and shit: " << this->getPayloadSize() << std::endl;
	#endif
	return bufferPosition;
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

	// Unpack OWN_SIZE
	uint32_t size = UnpackUINT32_T(payload, bufferPosition);
	bufferPosition += sizeof(uint32_t);

	// Unpack PLAYER_COUNT
	uint8_t number_of_players = UnpackUINT8_T(payload, bufferPosition);
	bufferPosition += sizeof(uint8_t);

	// Unpack OBJECT_COUNT
	uint16_t number_of_objects = UnpackUINT16_T(payload, bufferPosition);
	bufferPosition += sizeof(uint16_t);

	// TODO: Remove this stupid buffer copying. Implementation has changed since design and this kind of design is pointless here.
	// return remaining of the received message, if there is any

	#ifdef MESG_TEST
		//	std::cout << "Messages.hpp - GameUpdate::Unpack players: " << unsigned(number_of_players) << std::endl;
		//	std::cout << "Messages.hpp - GameUpdate::Unpack objects: " << number_of_objects << std::endl;
	#endif

	uint8_t * remainingPayload = 0;

	////TODO comment
	int remainingPayloadSize = (length - bufferPosition); // Need to calculate this here. Inside if statement (length - bufferPosition) return some really bizzare values.
	if( remainingPayloadSize > 0) {
		remainingPayload = static_cast<uint8_t *>(malloc(remainingPayloadSize));
		memcpy(remainingPayload, &payload[bufferPosition], length - bufferPosition);
	}

	bufferPosition = 0;

	vector<GamePlayer*> playerObjects;
	vector<GameObject*> gameObjects;
	// Get GameObjects
	int iterator = 0;
	while ( iterator < number_of_objects){
		gameObjects.push_back(GameObject::Unpack(remainingPayload, bufferPosition));
		bufferPosition += GameObject::getBufferReadSizeInBytes();
		iterator++;
	}

	// Get GamePlayers
	iterator = 0;
	while ( iterator < number_of_players){
		playerObjects.push_back(GamePlayer::Unpack(remainingPayload, bufferPosition));
		bufferPosition += GamePlayer::getBufferReadSizeInBytes();
		iterator++;
	}

	return new GameUpdate(
		header, pos_x, pos_y, dir_x, dir_y,
		size, number_of_players, number_of_objects,
		playerObjects, gameObjects
	);
}

int GameUpdate::PackSelf(uint8_t * payload) {
	return 0;
}

//======= POINTS ========//
Points * Points::Unpack(MessageHeader header, uint32_t length, uint8_t * payload) {
	Points * pointScoreObject = new Points(header);
	int readByteCount = 0;

	// Unpack player count (UINT_16)
	uint16_t player_count;
	memcpy(&player_count, payload, sizeof(uint16_t));
	//	std::cout << "Message.cpp: Point object count" << player_count << std::endl;
	readByteCount += sizeof(uint16_t);

	// Unpack player id and points to vector(UINT_16 and UINT_32)
	for (int i = 0; i < player_count; i++) {
		uint16_t player_id;
		uint32_t player_point;
		string player_nick;

		memcpy(&player_id, payload, sizeof(uint16_t));
		readByteCount += sizeof(uint16_t);
		pointScoreObject->player_ids.push_back(ntohs(player_id));

		memcpy(&player_nick, payload, sizeof(char)*12);
		readByteCount += sizeof(char)*12;
		pointScoreObject->player_nicks.push_back(player_nick);

		readByteCount += sizeof(uint16_t);
		memcpy(&player_id, payload, sizeof(uint16_t));
		pointScoreObject->player_points.push_back(ntohl(player_point));

		readByteCount += sizeof(uint32_t);
		#ifdef MESG_TEST
			std::cout << "Message.cpp: Player " << player_id << " got " <<
			": " << player_nick << " got" << player_point << " points." << std::endl;
		#endif
	}
	return pointScoreObject;
}
int Points::PackSelf(uint8_t * payload) {
	return 0;
}

int Ping::PackSelf(uint8_t * payload) {
	#ifdef MESG_TEST
		std::cout << "Sending -> PING: "<< std::endl;
	#endif
	int bufferPosition = getHeaderSize();
	// insert MSG_SUBTYPE to buffer
	#ifdef MESG_TEST
		//	std::cout << "GameMessageType: " << getGameMessageType() << std::endl;
	#endif
	bufferPosition = CreateHeader(this, payload);
	return bufferPosition;
}

int Move::PackSelf(uint8_t * payload) {
	#ifdef MESG_TEST
		std::cout << "Sending -> MOVE: "<< std::endl;
	#endif
	int bufferPosition = getHeaderSize();

	// insert EVENT_TYPE to buffer
	PackUINT8ToPayload(eventType, payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint8_t));

	// insert POS_X to buffer
	PackUINT16ToPayload(posX, payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint16_t));

	// insert POX_Y to buffer
	PackUINT16ToPayload(posY, payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint16_t));

	// insert DIR_X to buffer
	PackUINT16ToPayload(dirX, payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint16_t));

	// insert DIR_Y to buffer
	PackUINT16ToPayload(dirY, payload, bufferPosition);
	bufferPosition += addPayloadSize(sizeof(uint16_t));

	CreateHeader(this, payload);
	#ifdef MESG_TEST
//		std::cout << "Whole message size: " << bufferPosition << " and shit: " << this->getPayloadSize() << std::endl;
	#endif
	return bufferPosition;
}

////======= EXIT ========//
int Exit::Ack(uint8_t* payload) {
	int bufferPosition = 0;
	// insert MESSAGE_TYPE to buffer
	uint8_t type = static_cast<uint8_t>(GAME_MESSAGE_TYPE::EXIT);
	payload = CreateGameMessageACKHeader(gameTime, getGameMessageType());
	PackUINT8ToPayload(type, payload, bufferPosition);
	return bufferPosition;
}

////======= PLAYER_DEAD ========//
PlayerDead* PlayerDead::Unpack(MessageHeader header, uint32_t length, uint8_t* payload) {

	#ifdef MESG_TEST
		cout << "Receiving -> GAME_MESSAGE: " << getSubMessageTypeAsString(header.message_type) << endl;
	#endif
	// Unpack player id (UINT_16)
	uint16_t playerID = UnpackUINT16_T(payload, 0);

	return new PlayerDead(header, playerID);
}

int PlayerDead::PackSelf(uint8_t * payload) {
	return 0;
}

//
//======= PLAYER_OUT ========//
PlayerOut * PlayerOut::Unpack(MessageHeader header, uint32_t length, uint8_t * payload) {

	#ifdef MESG_TEST
		cout << "Receiving -> GAME_MESSAGE: " << getSubMessageTypeAsString(header.message_type) << endl;
	#endif
	// Unpack player id (UINT_16)
	uint16_t playerID = UnpackUINT16_T(payload, 0);

	return new PlayerOut(header, playerID);
}

int PlayerOut::PackSelf(uint8_t * payload) {
	return 0;
}

ChatMessage * ChatMessage::Unpack(MessageHeader, uint32_t, uint8_t*){
	return nullptr;
}
