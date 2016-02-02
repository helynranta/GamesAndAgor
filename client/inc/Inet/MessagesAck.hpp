#ifndef SRC_INET_MESSAGESACK_HPP_
#define SRC_INET_MESSAGESACK_HPP_

#include "Inet/Messages.hpp"

class MessagesAck : public Message {
	public:
		inline MessagesAck(MessageHeader header, GAME_MESSAGE_TYPE pGameMessageType) :
				Message(header, MESSAGE_TYPE::GAME_MESSAGE) {
		};

		inline ~MessagesAck(){};
};

class JoinAck: public MessagesAck {
	public:
		inline JoinAck(MessageHeader header) :
				MessagesAck(header, GAME_MESSAGE_TYPE::JOIN) {;};

		inline ~JoinAck() {;};

		static JoinAck* PackSelf();
		static JoinAck* Unpack(uint8_t* payload);
};

class NickAck: public MessagesAck {
	public:
		inline NickAck(MessageHeader header) :
			MessagesAck(header, GAME_MESSAGE_TYPE::NICK) {;};

		inline ~NickAck(){;};

		static NickAck* PackSelf();
		static NickAck* Unpack(uint8_t* payload);
};

#endif /* SRC_INET_MESSAGESACK_HPP_ */
