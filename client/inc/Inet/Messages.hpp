#ifndef _MESSAGES_HPP
#define _MESSAGES_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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
	virtual ~IMessage();
	virtual void unpack() = 0;
	virtual void pack() = 0;
	virtual void update() = 0;
};

class Message: public IMessage {
public:
	inline Message() {
	}
	inline virtual ~Message() {
		;
	}
	virtual void unpack() = 0;
	inline static void unpack_header(int socket_fd,
			struct message_header *header) {
		uint8_t byteBuffer[1500];
		int location = 0;
		memset(byteBuffer, 0, 1500);
		memset(header, 0, sizeof(struct message_header));
		header->addrlen = sizeof(header->sender_addr);

		int read_amount = recvfrom(socket_fd, byteBuffer, 1500, 0,
				reinterpret_cast<struct sockaddr*>(&header->sender_addr),
				&header->addrlen);
		if (read_amount > 0) {

			uint16_t uint16_tmp;
			memcpy(&uint16_tmp, &byteBuffer[location], sizeof(uint16_t));
			header->user_id = ntohs(uint16_tmp);
			std::cout << "USER_ID: " << header->user_id << std::endl;
			location += static_cast<uint16_t>(sizeof(uint16_t));
			std::cout << "location pointer: " << location << std::endl;

			uint32_t uint32_tmp;
			memcpy(&uint32_tmp, &byteBuffer[location], sizeof(uint32_t));
			header->game_time = ntohl(uint32_tmp);
			std::cout << "GAME TIME: " << header->game_time << std::endl;
			location += static_cast<uint16_t>(sizeof(uint32_t));
			std::cout << "location pointer: " << location << std::endl;

			uint8_t uint8_tmp;
			memcpy(&header->message_type, &byteBuffer[location], sizeof(uint8_t));
			std::cout << "Message type: " << unsigned(header->message_type) << std::endl;
			location += static_cast<uint16_t>(sizeof(uint8_t));
			std::cout << "location pointer: " << location << std::endl;

			memcpy(&uint32_tmp, &byteBuffer[location], sizeof(uint32_t));
			header->payload_length = ntohl(uint32_tmp);
			std::cout << "Payload lenght: " << header->payload_length << std::endl;
			location += static_cast<uint16_t>(sizeof(uint32_t));
			std::cout << "location pointer: " << location << std::endl;
		}
		//recvfrom(socket_fd, &header->game_time, sizeof(uint32_t), 0, NULL, NULL);
		//std::cout << read_amount << std::endl;
		//recvfrom(socket_fd, &header->message_type, sizeof(uint8_t), 0, NULL, NULL);
		//std::cout << "BOOM?" << std::endl;
		//recvfrom(socket_fd, &header->payload_length, sizeof(uint32_t), 0, NULL, NULL);
		//std::cout << "BOOM?" << std::endl;

		char s[INET_ADDRSTRLEN];
		std::cout << "Got message from "
				<< inet_ntop(header->sender_addr.ss_family,
						reinterpret_cast<const void*>(&reinterpret_cast<struct sockaddr_in*>(reinterpret_cast<struct sockaddr*>(&header->sender_addr))->sin_addr),
						s, sizeof s) << std::endl;

		std::cout << "BOOM?" << std::endl;
	}
	virtual void pack() = 0;
	virtual void update() = 0;
};

class JoinMessage: public Message {
public:
	inline JoinMessage() {
		;
	}

};

class NickACK: public Message {
public:
	inline NickACK(/* byttejä tänn */) {
		;
	}
	inline ~NickACK() {
		;
	}
	inline void unpack() override {
		;
	}
	inline void pack() override {
		;
	}
	inline void update() override {
		;
	}
};
#endif
