#ifndef _MESSAGES_HPP
#define _MESSAGES_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>

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
//	 std::cout << "Location pointer: " << bufferPosition << std::endl;
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
		uint32_t gameTime = 0;
		uint8_t message_type = 0;
		uint32_t payload_length = 0;
		socklen_t addrlen = 0;
		struct sockaddr_storage sender_addr = { };
};

class IMessage {
	public:
		IMessage();
		virtual ~IMessage();
		virtual void Update() = 0;
		// Currently only Join and Nick need Packing functions
		virtual int PackSelf(uint8_t*) = 0;
		static Message * Unpack(MessageHeader, uint32_t, uint8_t*);
};

class Message: public IMessage {
	public:
		// Constructors & Destructor
		inline Message(MessageHeader header, MESSAGE_TYPE pMessageType) {
			gameTime = header.gameTime;
			messageHeaderUserID = header.user_id;
			messageType = pMessageType;
		};

		inline ~Message() {};

		void Update() = 0;

		inline int PackSelf(uint8_t * payload) {
			return 0;
		}

		static void UnpackHeader(int socket_fd, struct MessageHeader*, uint8_t*);


		inline uint32_t getgameTime() const {
			return gameTime;
		};

		inline uint32_t getMessageHeaderUserID() const {
			return messageHeaderUserID;
		};

		inline uint32_t getPayloadSize() const {
			return payloadSize;
		};

		inline int addPayloadSize(int size) {
			payloadSize += size;
			return size;
		};

		inline MESSAGE_TYPE getMessageType() const {
			return messageType;
		};

		static int CreateHeader(Message * message, uint8_t * buffer);

		static uint32_t getHeaderSize() {
			return headerSize;
		};

	protected:
		uint32_t gameTime;
		uint32_t messageHeaderUserID;
		uint32_t payloadSize = 0;
		MESSAGE_TYPE messageType;
		static const uint32_t headerSize = 11;

};

class GameMessage: public Message {
	public:
		// Constructor & Destructor
		inline GameMessage(MessageHeader header, GAME_MESSAGE_TYPE pGameMessageType) :
				Message(header, MESSAGE_TYPE::GAME_MESSAGE) {
			gameMessageType = pGameMessageType;
		};

		inline ~GameMessage() {};

		inline void Update(){};


		static GameMessage * Unpack(MessageHeader, uint32_t, uint8_t*);

		inline GAME_MESSAGE_TYPE getGameMessageType() const {
			return gameMessageType;
		};


	protected:
		GAME_MESSAGE_TYPE gameMessageType;
};

class Join: public GameMessage {
	public:
		// Constructor & Destructor
		inline Join(MessageHeader header) :
				GameMessage(header, GAME_MESSAGE_TYPE::JOIN) {};

		inline ~Join() {};

		int PackSelf(uint8_t * payload);

		static Join * Unpack(MessageHeader, uint32_t, uint8_t*);

};

class Nick : public GameMessage {
	public:
		// Constructors & Destructor
		inline Nick(MessageHeader header) :	GameMessage(header, GAME_MESSAGE_TYPE::NICK){};

		inline Nick(MessageHeader header, std::string nickName) :
				GameMessage(header, GAME_MESSAGE_TYPE::NICK) {

			memset(&nick, '\0', Nick::sizeOfNick());
			if(nickName.size() < sizeOfNick()){
				memcpy(&nick, nickName.c_str(), nickName.size());
			}else{
				memcpy(&nick, nickName.c_str(), sizeOfNick() - 1); // Take just 11 chars from parameter
			}
		};

		inline ~Nick() {};

		int PackSelf(uint8_t * payload);

		static Nick * Unpack(MessageHeader, uint32_t, uint8_t*);

		inline std::string getNickname(){
			return std::string(nick);
		};

		inline static uint32_t sizeOfNick(){
			return 12;
		};

	private:
		char nick[12];
};

class Exit: public GameMessage {
	public:
		// Constructor & Destructor
		inline Exit(MessageHeader header) : GameMessage(header, GAME_MESSAGE_TYPE::EXIT) {};
		inline ~Exit() {};

		int Ack(uint8_t * payload);
};

class Restart: public GameMessage {
	public:
		// Constructor & Destructor
		inline Restart(MessageHeader header) : GameMessage(header, GAME_MESSAGE_TYPE::RESTART) {};
		inline ~Restart() {};

		static Restart * Unpack(MessageHeader, uint32_t, uint8_t*);
};

// =========  POINTS =========  //
class Points: public GameMessage {
	public:
		// Constructor & Destructor
		inline Points(MessageHeader header) : GameMessage(header, GAME_MESSAGE_TYPE::POINTS) {};
		inline ~Points() {};

		int PackSelf(uint8_t * payload);

		static Points * Unpack(MessageHeader, uint32_t, uint8_t*);
	private:
		std::vector<int> player_ids;
		std::vector<int> player_points;
};

// =========  GAME_END =========  //
class GameEnd: public GameMessage {
	public:
		// Constructor & Destructor
		inline GameEnd(MessageHeader header, Points * pPoints) :
				GameMessage(header, GAME_MESSAGE_TYPE::GAME_END) {
			points = pPoints;
		};
		inline ~GameEnd() {};

		inline static GameEnd * Unpack(MessageHeader header, uint32_t lenght, uint8_t * payload) {
			return new GameEnd(header, Points::Unpack(header, lenght, payload));
		}

		inline int PackSelf(uint8_t * payload) {
			return 0;
		};

	private:
		Points * points;
};

// =========  PLAYER_DEAD =========  //
class PlayerDead: public GameMessage {
	public:
		inline PlayerDead(MessageHeader header, uint16_t id) :
				GameMessage(header, GAME_MESSAGE_TYPE::PLAYER_DEAD) {
			playerID = static_cast<uint8_t>(id);
		};

		inline ~PlayerDead() {};

		int PackSelf(uint8_t * payload);

		static PlayerDead* Unpack(MessageHeader header, uint32_t length, uint8_t* payload);
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
		};
		inline ~PlayerOut() {};

		static PlayerOut * Unpack(MessageHeader, uint32_t, uint8_t*);

		int PackSelf(uint8_t * payload);
	private:
		uint16_t playerID;
		inline uint16_t getPlayerID() {
			return playerID;
		};
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
		};

		inline ~GameUpdate() {};

		int PackSelf(uint8_t * payload);

		static GameUpdate * Unpack(MessageHeader, uint32_t, uint8_t*);

		inline uint16_t getPosX(){ return pos_x; };

		inline uint16_t getPosY(){ return pos_y; };

		inline uint16_t getDirX(){ return dir_x; };

		inline uint16_t getDirY(){ return dir_y; };

		inline uint8_t getNumberOfPlayers() {
			return number_of_players;
		};

		inline uint16_t getNumberOfObjects() {
			return number_of_objects;
		};

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
		};

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
		};

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

inline std::string getMessageTypeAsString(uint8_t messageType){
	switch (messageType) {
		case MESSAGE_TYPE::GAME_MESSAGE:
			return "GAME_MESSAGE";
		case MESSAGE_TYPE::ACK:
			return "ACK";
		case MESSAGE_TYPE::PLAYER_CHAT_MESSAGE:
			return "PLAYER_CHAT_MESSAGE";
		case MESSAGE_TYPE::PLAYER_MOVEMENT:
			return "PLAYER_MOVEMENT";
		case MESSAGE_TYPE::STATISTICS_MESSAGE:
			return "STATISTICS_MESSAGE";
	}
	return "Cannot determinate";

}

inline std::string getSubMessageTypeAsString(uint8_t subType) {
	switch (subType) {
	case GAME_MESSAGE_TYPE::JOIN:
		return "JOIN";
	case GAME_MESSAGE_TYPE::NICK:
		return "NICK";
	case GAME_MESSAGE_TYPE::GAME_UPDATE:
		return "GAME_UPDATE";
	case GAME_MESSAGE_TYPE::GAME_END:
		return "GAME_END";
	case GAME_MESSAGE_TYPE::PLAYER_DEAD:
		return "PLAYER_DEAD";
	case GAME_MESSAGE_TYPE::PLAYER_OUT:
		return "PLAYER_OUT";
	case GAME_MESSAGE_TYPE::POINTS:
		return "POINTS";
	case GAME_MESSAGE_TYPE::RESTART:
		return "RESTART";

	}
	return "Cannot determinate";

}

#endif
