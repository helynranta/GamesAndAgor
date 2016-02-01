#ifndef SRC_INET_MESSAGESACK_HPP_
#define SRC_INET_MESSAGESACK_HPP_

#include "Inet/Messages.hpp"

class MessagesAck: public Message {
	public:
		inline MessagesAck(MessageHeader header, GAME_MESSAGE_TYPE pGameMessageType) :
				Message(header, MESSAGE_TYPE::GAME_MESSAGE) {
		}
		;

		inline ~MessagesAck(){;};
};

class JoinAck: public MessagesAck {
	public:
		inline JoinAck(MessageHeader header) :
				MessagesAck(header, GAME_MESSAGE_TYPE::JOIN) {
			;
		}
		;

		inline ~JoinAck() {
			;
		}
		;
};

#endif /* SRC_INET_MESSAGESACK_HPP_ */
