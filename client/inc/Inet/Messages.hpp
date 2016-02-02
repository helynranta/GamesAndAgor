#ifndef _MESSAGES_HPP
#define _MESSAGES_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <vector>

#define BUFFER_SIZE 1500

class Message;
class Points;
class GameMessage;

enum MESSAGE_TYPE {
	GAME_MESSAGE = 0,
	ACK = 1,
	PLAYER_MOVEMENT = 2,
	PLAYER_CHAT_MESSAGE = 3,
	STATISTICS_MESSAGE = 4

};

enum GAME_MESSAGE_TYPE {
	JOIN = 0,
	NICK = 1,
	EXIT = 2,
	RESTART = 3,
	GAME_END = 4,
	GAME_UPDATE = 5,
	POINTS = 6,
	PLAYER_DEAD = 7,
	PLAYER_OUT = 8

};

inline uint32_t UnpackUINT32_T(uint8_t * payload, int bufferPosition) {
	uint32_t uint32Variable;
	memcpy(&uint32Variable, payload, sizeof(uint32_t));
	std::cout << "Location pointer: " << bufferPosition << std::endl;
	return ntohl(uint32Variable);
}

inline uint16_t UnpackUINT16_T(uint8_t * payload, int bufferPosition) {
	uint16_t uint16Variable;
	memcpy(&uint16Variable, payload, sizeof(uint16_t));
	std::cout << "Location pointer: " << bufferPosition << std::endl;
	return ntohs(uint16Variable);
}

inline uint8_t UnpackUINT8_T(uint8_t * payload, int bufferPosition) {
	uint8_t uint8Variable;
	memcpy(&uint8Variable, payload, sizeof(uint8_t));
	std::cout << "Location pointer: " << bufferPosition << std::endl;
	return uint8Variable;
}

inline void PackUINT32ToPayload(uint32_t variableToPack, uint8_t * payload,
		int bufferPosition) {
	uint32_t uint32Variable = htonl(variableToPack);
	memcpy(&payload[bufferPosition], &uint32Variable, sizeof(uint32_t));
	std::cout << "Location pointer: " << bufferPosition << std::endl;
}

inline void PackUINT16ToPayload(uint16_t variableToPack, uint8_t * payload,
		int bufferPosition) {
	uint16_t uint16Variable = htons(variableToPack);
	memcpy(&payload[bufferPosition], &uint16Variable, sizeof(uint16_t));
	std::cout << "Location pointer: " << bufferPosition << std::endl;
}

inline void PackUINT8ToPayload(uint8_t variableToPack, uint8_t * payload,
		int bufferPosition) {
	memcpy(&payload[bufferPosition], &variableToPack, sizeof(uint8_t));
	std::cout << "Location pointer: " << bufferPosition << std::endl;
}

inline uint8_t * CreateGameMessageACKHeader(uint32_t packetID,
		GAME_MESSAGE_TYPE type) {
	uint8_t * buffer = static_cast<uint8_t*>(malloc(
			sizeof(uint32_t) + sizeof(uint8_t))); // Size of header
	int bufferPosition = 0;
	PackUINT32ToPayload(packetID, buffer, bufferPosition);
	bufferPosition += sizeof(uint32_t);

	PackUINT8ToPayload(static_cast<uint8_t>(type), buffer, bufferPosition);
	return buffer;
}

struct MessageHeader {
		uint16_t user_id = 0;
		uint32_t game_time;
		uint8_t message_type;
		uint32_t payload_length;
		socklen_t addrlen;
		struct sockaddr_storage sender_addr;
};

class IMessage {
	public:
		IMessage();
		virtual ~IMessage();
		virtual void Pack(Message*) = 0;
		virtual void Update() = 0;
		static Message * Unpack(MessageHeader, uint32_t, uint8_t*);
		virtual void Ack(uint8_t*, int) = 0;
};

class Message: public IMessage {
	public:
		inline Message(uint32_t pPacketID, MESSAGE_TYPE pMessageType) {
			packetID = pPacketID;
			messageType = pMessageType;
		}
		inline ~Message() {
      ;
		}
		void Pack() {
		}
		;

		inline void Update() {
			;
		}
		;

		inline void Ack(uint8_t* payload, int bufferPosition) {
		}
		;

		static void UnpackHeader(int socket_fd, struct MessageHeader*,
				uint8_t*);

		static void Unpack(MessageHeader, uint32_t, uint8_t*);

		inline uint32_t getpacketID() const {
			return packetID;
		}
		;
		inline MESSAGE_TYPE getMessageType() const {
			return messageType;
		}
		;
	protected:
		uint32_t packetID;
		MESSAGE_TYPE messageType;

};

class GameMessage: public Message {
	public:

		inline GameMessage(MessageHeader header, GAME_MESSAGE_TYPE pGameMessageType) :
				Message(header.game_time, MESSAGE_TYPE::GAME_MESSAGE) {
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

		inline void Ack(uint8_t* payload, int bufferPosition) {
			// insert MESSAGE_TYPE to buffer
			uint8_t type = static_cast<uint8_t>(GAME_MESSAGE_TYPE::JOIN);
			PackUINT8ToPayload(type, payload, bufferPosition);
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

		void Ack(uint8_t* payload, int bufferPosition) {
			// insert MESSAGE_TYPE to buffer
			uint8_t type = static_cast<uint8_t>(GAME_MESSAGE_TYPE::NICK);
			PackUINT8ToPayload(type, payload, bufferPosition);
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

		void Ack(uint8_t* payload, int bufferPosition) {
			// insert MESSAGE_TYPE to buffer
			uint8_t type = static_cast<uint8_t>(GAME_MESSAGE_TYPE::JOIN);
			payload = CreateGameMessageACKHeader(packetID, getGameMessageType());
			PackUINT8ToPayload(type, payload, bufferPosition);
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

		void Ack(uint8_t* payload, int bufferPosition) {
			// insert MESSAGE_TYPE to buffer
			uint8_t type = static_cast<uint8_t>(GAME_MESSAGE_TYPE::JOIN);
			PackUINT8ToPayload(type, payload, bufferPosition);
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

		inline static GameEnd * Unpack(MessageHeader header, uint32_t lenght,
				uint8_t * payload) {
			return new GameEnd(header, Points::Unpack(header, lenght, payload));
		}
		;

		void Ack(uint8_t* payload, int bufferPosition) {
			// insert MESSAGE_TYPE to buffer
			uint8_t type = static_cast<uint8_t>(GAME_MESSAGE_TYPE::JOIN);
			PackUINT8ToPayload(type, payload, bufferPosition);
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
		inline static PlayerDead* Unpack(MessageHeader header, uint32_t length,
				uint8_t* payload) {
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
		inline static PlayerOut * Unpack(MessageHeader header, uint32_t length,
				uint8_t* payload) {
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
		inline GameUpdate(MessageHeader header, uint16_t pPos_x,
				uint16_t pPos_y, uint16_t pDir_x, uint16_t pDir_y,
				uint8_t pNumber_of_players, uint16_t pNnumber_of_objects) :
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
		inline GamePlayer(uint16_t pPlayerID, uint16_t pPos_x, uint16_t pPos_y,
				uint16_t pDir_x, uint16_t pDir_y, uint32_t pSize) {
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
		inline GameObject(uint16_t pObjectID, uint16_t pLoc_x,
				uint16_t pLoc_y) {
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
