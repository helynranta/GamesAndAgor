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

enum MESSAGE_TYPE {
	GAME_MESSAGE = 0,
	ACK = 1,
	PLAYER_MOVEMENT = 2,
	PLAYER_CHAT_MESSAGE = 3,
	STATISTICS_MESSAGE = 4

};

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
	//virtual Message UnpackPayload(uint32_t, uint8_t*) = 0;
	static Message * Unpack(uint32_t, uint8_t*);
};

class Message: public IMessage {
public:
	inline Message() {
		;
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

	static void UnpackHeader(int socket_fd, struct MessageHeader*, uint8_t*);

	static void Unpack(uint32_t, uint8_t*);
};

class GameMessage: public Message {
public:

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

	inline GameMessage() {
		;
	}
	;

	inline ~GameMessage() {
		;
	}
	;

	void Pack(Message *) {

	}
	;

	inline void Update() {
	}
	;

	inline void UnpackPayload() {
	}
	;

	static GameMessage * Unpack(uint32_t, uint8_t*);

	static uint8_t UnpackHeader(uint8_t*);

	inline int getMessageType() const {
		return message_type;
	}
	;

private:
	GAME_MESSAGE_TYPE message_type;

};

class Join: public GameMessage {
public:
	inline Join() {
		;
	}
	;
	inline ~Join() {
		;
	}
	;
};

class Nick: public GameMessage {
public:
	inline Nick() {
		;
	}
	;
	inline ~Nick() {
		;
	}
	;
	static Nick * Unpack(uint32_t, uint8_t*);
private:
	std::string nick;
};

class Exit: public GameMessage {
public:
	inline Exit() {
		;
	}
	;
	inline ~Exit() {
		;
	}
	;
};

class Restart: public GameMessage {
public:
	inline Restart() {
		;
	}
	;
	inline ~Restart() {
		;
	}
	;
	static Restart * Unpack(uint32_t, uint8_t*);
};

class GameEnd: public GameMessage {
public:
	inline GameEnd() {
		;
	}
	;
	inline ~GameEnd() {
		;
	}
	;
};

class Points: public GameMessage {
public:
	inline Points() {
		;
	}
	;
	inline ~Points() {
		;
	}
	;
	static Points * Unpack(uint32_t, uint8_t*);
private:
	std::vector<int> player_ids;
	std::vector<int> player_points;
};

class PlayerDead: public GameMessage {
public:
	inline PlayerDead() {
		;
	}
	;
	inline ~PlayerDead() {
		;
	}
	;
	static PlayerDead* Unpack(uint32_t, uint8_t*);
};

class PlayerOut: public GameMessage {
public:
	inline PlayerOut() {
		;
	}
	;
	inline ~PlayerOut() {
		;
	}
	;
	static PlayerOut * Unpack(uint32_t, uint8_t*);
};

class GameUpdate: public GameMessage {
public:
	inline GameUpdate() {
		;
	}
	;
	inline ~GameUpdate() {
		;
	}
	;
	static GameUpdate * Unpack(uint32_t, uint8_t*);
private:
	uint16_t pos_x;
	uint16_t pos_y;
	uint16_t dir_x;
	uint16_t dir_y;
	uint8_t number_of_players;
	uint16_t number_of_objects;

};

class GameObject: public GameMessage {
public:
	inline GameObject() {
		;
	}
	;
	inline ~GameObject() {
		;
	}
	;
private:
	uint16_t object_id;
	uint16_t loc_x;
	uint16_t loc_y;
};

class GamePlayer: public GameMessage {
public:
	inline GamePlayer() {
		;
	}
	;
	inline ~GamePlayer() {
		;
	}
	;
private:
	uint16_t pos_x;
	uint16_t pos_y;
	uint16_t dir_x;
	uint16_t dir_y;
	uint32_t size;
};

class MessageFactory {
public:
	static MessageFactory& getInstance() {
		static MessageFactory instance;
		return instance;
	}
	Message* getMessageByType(struct MessageHeader*, uint8_t*);
private:
	MessageFactory() {
	}
	;
	MessageFactory(MessageFactory const&) = delete;
	void operator=(MessageFactory const&) = delete;
};

#endif
