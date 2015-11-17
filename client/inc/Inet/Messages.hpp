#ifndef _MESSAGES_HPP
#define _MESSAGES_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1500

enum MESSAGE_TYPE {
	JOIN = 1, JOIN_ACK = 2, NICK = 3, NICK_ACK = 4
};

struct message_header {
	uint16_t user_id = 0;
	uint32_t game_time;
	uint8_t message_type;
	uint32_t payload_length;
	socklen_t addrlen;
	struct sockaddr_storage sender_addr;
};

class IMessage {
public:
	IMessage() {
	}
	;
	virtual ~IMessage() {
	}
	;
	virtual void Pack() = 0;
	virtual void Update() = 0;
	virtual void UnpackPayload() = 0;
};

class Message: public IMessage {
public:
	inline Message() {
	}
	inline virtual ~Message() {
		;
	}
	void Pack() {
	}
	;
	void Update() {
	}
	;
	void UnpackPayload() {
	}
	;
	static void UnpackHeader(int socket_fd, struct message_header*, uint8_t*);

	virtual void unpack() = 0;
	virtual void pack() = 0;
	virtual void update() = 0;
};

class JoinMessage: public Message {
public:
	JoinMessage();

	~JoinMessage() {
	}

	inline void Pack() {
		;
	}

	inline void Update() {
		;
	}

	inline void UnpackPayload(uint32_t, uint8_t*);

	inline void unpack() {;}
	inline  void pack() {;}
	inline void update() {;}

};

class NickACK {
public:
	inline NickACK() {
		;
	}
	inline ~NickACK() {
		;
	}
	void Pack() {
		;
	}

	inline void Update() {
		;
	}

	inline void UnpackPayload(uint32_t, uint8_t*){
		;
	}

};

class MessageFactory {
public:
	static MessageFactory& getInstance() {
		static MessageFactory instance;
		return instance;
	}
	Message* getMessageByType(struct message_header*, uint8_t*);
private:
	MessageFactory() {
	}
	;
	MessageFactory(MessageFactory const&) = delete;
	void operator=(MessageFactory const&) = delete;
};
#endif
