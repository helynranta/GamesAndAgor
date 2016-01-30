#ifndef _MESSAGES_HPP
#define _MESSAGES_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <vector>

#define BUFFER_SIZE 15000
#define HEADER_SIZE 88

class Message;
class Points;
class GameMessage;

enum MESSAGE_TYPE {
	GAME_MESSAGE = 0, ACK = 1, PLAYER_MOVEMENT = 2, PLAYER_CHAT_MESSAGE = 3, STATISTICS_MESSAGE = 4

};

enum GAME_MESSAGE_TYPE {
	JOIN = 0, NICK = 1, EXIT = 2, RESTART = 3, GAME_END = 4, GAME_UPDATE = 5, POINTS = 6, PLAYER_DEAD = 7, PLAYER_OUT = 8

};

inline uint32_t UnpackUINT32_T(uint8_t * payload, int bufferPosition) {
	uint32_t uint32Variable;
	memcpy(&uint32Variable, &payload[bufferPosition], sizeof(uint32_t));
	// std::cout << "Location pointer: " << bufferPosition << std::endl;
	return ntohl(uint32Variable);
}

inline uint16_t UnpackUINT16_T(uint8_t * payload, int bufferPosition) {
	uint16_t uint16Variable;
	memcpy(&uint16Variable, &payload[bufferPosition], sizeof(uint16_t));
	// std::cout << "Location pointer: " << bufferPosition << std::endl;
	return ntohs(uint16Variable);
}

inline uint8_t UnpackUINT8_T(uint8_t * payload, int bufferPosition) {
	uint8_t uint8Variable;
	memcpy(&uint8Variable, &payload[bufferPosition], sizeof(uint8_t));
	return uint8Variable;
}

inline void PackUINT32ToPayload(uint32_t variableToPack, uint8_t * payload, int bufferPosition) {
	uint32_t uint32Variable = htonl(variableToPack);
	std::cout << variableToPack << std::endl;
	memcpy(&payload[bufferPosition], &uint32Variable, sizeof(uint32_t));
	// std::cout << "Location pointer: " << bufferPosition << std::endl;
}

inline void PackUINT16ToPayload(uint16_t variableToPack, uint8_t * payload, int bufferPosition) {
	uint16_t uint16Variable = htons(variableToPack);
	memcpy(&payload[bufferPosition], &uint16Variable, sizeof(uint16_t));
	// std::cout << "Location pointer: " << bufferPosition << std::endl;
}

inline void PackUINT8ToPayload(uint8_t variableToPack, uint8_t * payload, int bufferPosition) {
	memcpy(&payload[bufferPosition], &variableToPack, sizeof(uint8_t));
	// std::cout << "Location pointer: " << bufferPosition << std::endl;
}

inline uint8_t * CreateGameMessageACKHeader(uint32_t packetID, GAME_MESSAGE_TYPE type) {
	uint8_t * buffer = static_cast<uint8_t*>(malloc(sizeof(uint32_t) + sizeof(uint8_t))); // Size of header
	int bufferPosition = 0;
	PackUINT32ToPayload(packetID, buffer, bufferPosition);
	bufferPosition += sizeof(uint32_t);

	PackUINT8ToPayload(static_cast<uint8_t>(type), buffer, bufferPosition);
	return buffer;
}

struct MessageHeader {
		uint16_t user_id = 0;
		uint32_t game_time = 0;
		uint8_t message_type = 0;
		uint32_t payload_length = 0;
		socklen_t addrlen = 0;
		struct sockaddr_storage sender_addr = {};
};

class IMessage {
	public:
		IMessage();
		virtual ~IMessage();
		virtual void Pack(Message*) = 0;
		virtual void Update() = 0;
		static Message * Unpack(MessageHeader, uint32_t, uint8_t*);
		virtual int Ack(uint8_t*) = 0;
};

class Message: public IMessage {
	public:
		inline Message(MessageHeader header, MESSAGE_TYPE pMessageType) {
			packetID = header.game_time;
			userID = header.user_id;
			messageType = pMessageType;
		}
		inline ~Message() {
			;
		}
		void Pack(Message *) {
		}
		;

		inline void Update() {
			;
		}
		;

		inline int Ack(uint8_t* payload) {
			return 0;
		}
		;

		static void UnpackHeader(int socket_fd, struct MessageHeader*, uint8_t*);

		static void Unpack(MessageHeader, uint32_t, uint8_t*);

		inline uint32_t getpacketID() const {
			return packetID;
		}
		;
		inline uint32_t getUserID() const {
			return userID;
		}
		;

		inline uint32_t getPayloadSize() const {
			return payloadSize;
		}
		;

		inline int addPayloadSize(int size) {
			payloadSize += size;
			return size;
		}
		;

		inline MESSAGE_TYPE getMessageType() const {
			return messageType;
		}
		;

		inline static int CreateHeader(Message * message, uint8_t * buffer) {
			int bufferPosition = 0;

			// insert USER_ID to buffer
			uint16_t userID = static_cast<uint16_t>(message->getUserID());
			PackUINT16ToPayload(userID, buffer, bufferPosition);
			bufferPosition += sizeof(uint16_t);

			// insert GAME_TIME to buffer
			uint32_t gameTime = static_cast<uint32_t>(message->getpacketID());
			PackUINT32ToPayload(gameTime, buffer, bufferPosition);
			bufferPosition += sizeof(uint32_t);
			//int asd = bufferPosition - (sizeof(uint32_t));
			//std::cout <<  "Buffer pos: " << bufferPosition << " checked position: " << asd << " :: "  << UnpackUINT32_T(buffer, asd) << std::endl;

			// insert MESSAGE_TYPE to buffer
			std::cout << "MessageType: " << message->getMessageType() << std::endl;
			uint8_t type = static_cast<uint8_t>(message->getMessageType());
			PackUINT8ToPayload(type, buffer, bufferPosition);
			bufferPosition += sizeof(uint8_t);
			//asd = bufferPosition - (sizeof(uint8_t));
			//std::cout <<  "Buffer pos: " << bufferPosition << " checked position: " << asd << " :: "  << UnpackUINT8_T(buffer, asd) << std::endl;

			// insert payloadSize to buffer
			uint32_t payloadSize = static_cast<uint32_t>(message->getPayloadSize());
			PackUINT32ToPayload(payloadSize, buffer, bufferPosition);
			bufferPosition += sizeof(uint32_t);

			return bufferPosition;
		}

		uint32_t getHeaderSize(){
			return headerSize;
		}



	protected:
		uint32_t packetID;
		uint32_t userID;
		uint32_t payloadSize = 0;
		MESSAGE_TYPE messageType;
		static const uint32_t headerSize = 11;

};

class GameMessage: public Message {
	public:

		inline GameMessage(MessageHeader header, GAME_MESSAGE_TYPE pGameMessageType) :
				Message(header, MESSAGE_TYPE::GAME_MESSAGE) {
			gameMessageType = pGameMessageType;
		}
		;

		inline ~GameMessage() {
			;
		}
		;

		void Pack(Message *);

		inline void Update() {
		}
		;

		inline void UnpackPayload() {
		}
		;

		static GameMessage * Unpack(MessageHeader, uint32_t, uint8_t*);

		static uint8_t UnpackHeader(uint8_t*);

		inline GAME_MESSAGE_TYPE getGameMessageType() const {
			return gameMessageType;
		}
		;
	protected:
		GAME_MESSAGE_TYPE gameMessageType;
};

class Join: public GameMessage {
	public:
		inline Join(MessageHeader header) :
				GameMessage(header, GAME_MESSAGE_TYPE::JOIN) {
			;
		}
		;
		inline ~Join() {
			;
		}
		;

		inline int PackSelf(uint8_t * payload){

			int bufferPosition = getHeaderSize();


			// insert MSG_SUBTYPE to buffer
			std::cout << "GameMessageType: " << getGameMessageType() << std::endl;
			PackUINT8ToPayload(static_cast<uint8_t>(getGameMessageType()), payload, bufferPosition);
			bufferPosition += addPayloadSize(sizeof(uint8_t));

//			// insert USER_ID to buffer
//			PackUINT32ToPayload(getUserID(), payload, bufferPosition);
//			bufferPosition += addPayloadSize(sizeof(uint32_t));
//
//			// insert GAME_MESSAGE_TYPE to buffer
//			PackUINT8ToPayload(static_cast<uint8_t>(GAME_MESSAGE_TYPE::JOIN), payload, bufferPosition);
//			bufferPosition += addPayloadSize(sizeof(uint8_t));

			// insert PAYLOAD_SIZE to buffer
//			PackUINT32ToPayload(0, payload, bufferPosition);
//			bufferPosition += addPayloadSize(sizeof(uint32_t));

			CreateHeader(this, payload);
			std::cout << "Whole message size: " << bufferPosition << " and shit: " << this->getPayloadSize() << std::endl;

			return bufferPosition;
		};


		inline int Ack(uint8_t * payload) {
			int bufferPosition = 0;
			bufferPosition += CreateHeader(this, payload);

			// insert USER_ID to buffer
			uint32_t userID = static_cast<uint32_t>(getUserID());
			PackUINT32ToPayload(userID, payload, bufferPosition);
			bufferPosition += sizeof(uint32_t);

			// insert GAME_MESSAGE_TYPE to buffer
			uint8_t type = static_cast<uint8_t>(GAME_MESSAGE_TYPE::JOIN);
			PackUINT8ToPayload(type, payload, bufferPosition);
			return bufferPosition;
		}
		;
};

class Nick: public GameMessage {
	public:
		inline Nick(MessageHeader header) :
				GameMessage(header, GAME_MESSAGE_TYPE::NICK) {
			;
		}
		;
		inline ~Nick() {
			;
		}
		;

		int Ack(uint8_t* payload) {

			int bufferPosition = 0;
			bufferPosition = Message::CreateHeader(dynamic_cast<Message*>(this), payload);

			// insert MESSAGE_TYPE to buffer
			uint8_t type = static_cast<uint8_t>(GAME_MESSAGE_TYPE::NICK);
			PackUINT8ToPayload(type, payload, bufferPosition);
			return bufferPosition;
		}
		;

		static Nick * Unpack(MessageHeader, uint32_t, uint8_t*);
	private:
		std::string nick;
};

class Exit: public GameMessage {
	public:
		inline Exit(MessageHeader header) :
				GameMessage(header, GAME_MESSAGE_TYPE::EXIT) {

		}
		;
		inline ~Exit() {
			;
		}
		;

		int Ack(uint8_t* payload) {
			int bufferPosition = 0;
			// insert MESSAGE_TYPE to buffer
			uint8_t type = static_cast<uint8_t>(GAME_MESSAGE_TYPE::JOIN);
			payload = CreateGameMessageACKHeader(packetID, getGameMessageType());
			PackUINT8ToPayload(type, payload, bufferPosition);
			return bufferPosition;
		}
		;
};

class Restart: public GameMessage {
	public:
		inline Restart(MessageHeader header) :
				GameMessage(header, GAME_MESSAGE_TYPE::RESTART) {
			;
		}
		;
		inline ~Restart() {
			;
		}
		;

		int Ack(uint8_t* payload) {
			int bufferPosition = 0;
			// insert MESSAGE_TYPE to buffer
			uint8_t type = static_cast<uint8_t>(GAME_MESSAGE_TYPE::JOIN);
			PackUINT8ToPayload(type, payload, bufferPosition);
			return bufferPosition;
		}
		;

		static Restart * Unpack(MessageHeader, uint32_t, uint8_t*);
};

// =========  POINTS =========  //
class Points: public GameMessage {
	public:
		inline Points(MessageHeader header) :
				GameMessage(header, GAME_MESSAGE_TYPE::POINTS) {
			;
		}
		;
		inline ~Points() {
			;
		}
		;
		static Points * Unpack(MessageHeader, uint32_t, uint8_t*);
	private:
		std::vector<int> player_ids;
		std::vector<int> player_points;
};

// =========  GAME_END =========  //
class GameEnd: public GameMessage {
	public:
		inline GameEnd(MessageHeader header, Points * pPoints) :
				GameMessage(header, GAME_MESSAGE_TYPE::GAME_END) {
			points = pPoints;
		}
		;
		inline ~GameEnd() {
			;
		}
		;

		inline static GameEnd * Unpack(MessageHeader header, uint32_t lenght, uint8_t * payload) {
			return new GameEnd(header, Points::Unpack(header, lenght, payload));
		}
		;

		int Ack(uint8_t* payload) {
			int bufferPosition = 0;
			// insert MESSAGE_TYPE to buffer
			uint8_t type = static_cast<uint8_t>(GAME_MESSAGE_TYPE::JOIN);
			PackUINT8ToPayload(type, payload, bufferPosition);
			return bufferPosition;
		}
		;

	private:
		Points * points;
};

// =========  PLAYER_DEAD =========  //
class PlayerDead: public GameMessage {
	public:
		inline PlayerDead(MessageHeader header, uint16_t id) :
				GameMessage(header, GAME_MESSAGE_TYPE::PLAYER_DEAD) {
			playerID = static_cast<uint8_t>(id);
		}
		;
		inline ~PlayerDead() {
			;
		}
		;
		inline static PlayerDead* Unpack(MessageHeader header, uint32_t length, uint8_t* payload) {
			uint16_t playerID = UnpackUINT16_T(payload, 0);
			return new PlayerDead(header, playerID);
		}
		;

	private:
		uint16_t playerID;
		inline uint16_t getPlayerID() {
			return playerID;
		}
		;
};

// =========  PLAYER_OUT =========  //
class PlayerOut: public GameMessage {
	public:
		inline PlayerOut(MessageHeader header, uint16_t id) :
				GameMessage(header, GAME_MESSAGE_TYPE::PLAYER_OUT) {
			playerID = static_cast<uint8_t>(id);
		}
		;
		inline ~PlayerOut() {
			;
		}
		;
		inline static PlayerOut * Unpack(MessageHeader header, uint32_t length, uint8_t* payload) {
			uint16_t playerID = UnpackUINT16_T(payload, 0);
			return new PlayerOut(header, playerID);
		}
		;
	private:
		uint16_t playerID;
		inline uint16_t getPlayerID() {
			return playerID;
		}
		;
};

class GameUpdate: public GameMessage {
	public:
		inline GameUpdate(MessageHeader header, uint16_t pPos_x, uint16_t pPos_y, uint16_t pDir_x, uint16_t pDir_y, uint8_t pNumber_of_players,
				uint16_t pNnumber_of_objects) :
				GameMessage(header, GAME_MESSAGE_TYPE::GAME_UPDATE) {
			pos_x = pPos_x;
			pos_y = pPos_y;
			dir_x = pDir_x;
			dir_y = pDir_y;
			number_of_players = pNumber_of_players;
			number_of_objects = pNumber_of_players;
		}
		;
		inline ~GameUpdate() {
			;
		}
		;
		static GameUpdate * Unpack(MessageHeader, uint32_t, uint8_t*);
	private:
		uint16_t pos_x;
		uint16_t pos_y;
		uint16_t dir_x;
		uint16_t dir_y;
		uint8_t number_of_players;
		uint16_t number_of_objects;
		std::vector<int> players;
		std::vector<int> objects;

};

class GamePlayer {
	public:
		inline GamePlayer(uint16_t pPlayerID, uint16_t pPos_x, uint16_t pPos_y, uint16_t pDir_x, uint16_t pDir_y, uint32_t pSize) {
			playerID = pPlayerID;
			pos_x = pPos_x;
			pos_y = pPos_y;
			dir_x = pDir_x;
			dir_y = pDir_y;
			size = pSize;
		}
		;
	private:
		uint16_t playerID;
		uint16_t pos_x;
		uint16_t pos_y;
		uint16_t dir_x;
		uint16_t dir_y;
		uint32_t size;
};

class GameObject {
	public:
		inline GameObject(uint16_t pObjectID, uint16_t pLoc_x, uint16_t pLoc_y) {
			objectID = pObjectID;
			loc_x = pLoc_x;
			loc_y = pLoc_y;
		}
	private:
		uint16_t objectID;
		uint16_t loc_x;
		uint16_t loc_y;
};

class MessageFactory {
	public:
		static MessageFactory& getInstance() {
			static MessageFactory instance;
			return instance;
		}
		Message* getMessageByType(MessageHeader*, uint8_t*);
	private:
		MessageFactory() {
		}
		;
		MessageFactory(MessageFactory const&) = delete;
		void operator=(MessageFactory const&) = delete;
};

class ChatMessage: public Message {
	public:
		ChatMessage();
		virtual ~ChatMessage();
		static ChatMessage * Unpack(MessageHeader, uint32_t, uint8_t*);
};


#endif
