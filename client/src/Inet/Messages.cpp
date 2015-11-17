#include <iostream>
#include <cstring>

#include "Inet/Messages.hpp"

Message * MessageFactory::getMessageByType(struct message_header * header, uint8_t * payload) {
	switch (header->message_type) {
	case MESSAGE_TYPE::JOIN || MESSAGE_TYPE::JOIN_ACK:
		JoinMessage* message;
		message = new JoinMessage();
		message->UnpackPayload(header->payload_length, payload);
		return message;
	default:
		return new JoinMessage();
	}
}


void Message::UnpackHeader(int socket_fd, struct message_header *header, uint8_t* payloadBuffer) {
	uint8_t byteBuffer[BUFFER_SIZE];
	int udpBufP = 0;
	memset(byteBuffer, 0, BUFFER_SIZE);
	memset(header, 0, sizeof(struct message_header));
	header->addrlen = sizeof(header->sender_addr);

	int read_amount = recvfrom(socket_fd, byteBuffer, BUFFER_SIZE, 0,
			reinterpret_cast<struct sockaddr*>(&header->sender_addr),
			&header->addrlen);
	if (read_amount > 0) {

		uint16_t uint16_tmp;
		memcpy(&uint16_tmp, &byteBuffer[udpBufP], sizeof(uint16_t));
		header->user_id = ntohs(uint16_tmp);
		std::cout << "USER_ID: " << header->user_id << std::endl;
		udpBufP += static_cast<uint16_t>(sizeof(uint16_t));
		std::cout << "location pointer: " << udpBufP << std::endl;

		uint32_t uint32_tmp;
		memcpy(&uint32_tmp, &byteBuffer[udpBufP], sizeof(uint32_t));
		header->game_time = ntohl(uint32_tmp);
		std::cout << "GAME TIME: " << header->game_time << std::endl;
		udpBufP += static_cast<uint16_t>(sizeof(uint32_t));
		std::cout << "location pointer: " << udpBufP << std::endl;

		memcpy(&header->message_type, &byteBuffer[udpBufP], sizeof(uint8_t));
		std::cout << "Message type: " << unsigned(header->message_type)
				<< std::endl;
		udpBufP += static_cast<uint16_t>(sizeof(uint8_t));
		std::cout << "location pointer: " << udpBufP << std::endl;

		memcpy(&uint32_tmp, &byteBuffer[udpBufP], sizeof(uint32_t));
		header->payload_length = ntohl(uint32_tmp);
		std::cout << "Payload lenght: " << header->payload_length << std::endl;
		udpBufP += static_cast<uint16_t>(sizeof(uint32_t));
		std::cout << "location pointer: " << udpBufP << std::endl;

		// TODO Will I miss the last meaningful byte?
		memcpy(payloadBuffer, &byteBuffer[udpBufP], header->payload_length);
	}else{
		std::cout << "Nothing read from recvfrom()" << std::endl;
	}

	char s[INET_ADDRSTRLEN];
	std::cout << "Got message from "
			<< inet_ntop(header->sender_addr.ss_family,
					reinterpret_cast<const void*>(&reinterpret_cast<struct sockaddr_in*>(reinterpret_cast<struct sockaddr*>(&header->sender_addr))->sin_addr),
					s, sizeof s) << std::endl;
}


//======= JoinMessage ========//
JoinMessage::JoinMessage() {}
void JoinMessage::UnpackPayload(uint32_t length, uint8_t * payload){
	char * nick = static_cast<char*>(malloc(length * sizeof (char) + 1));
	memcpy(nick, payload, length * sizeof (char));
	nick[3] = '\0';
	std::cout << "Nick: " << nick << std::endl;
}
