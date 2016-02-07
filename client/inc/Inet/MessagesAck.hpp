#ifndef SRC_INET_MESSAGESACK_HPP_
#define SRC_INET_MESSAGESACK_HPP_

#include "Inet/Messages.hpp"

class MessagesAck : public Message {
	public:
		inline MessagesAck(MessageHeader header, GAME_MESSAGE_TYPE pGameMessageType) :
				Message(header, MESSAGE_TYPE::ACK) {
			gameMessageType = pGameMessageType;
		};

		inline ~MessagesAck(){};

		inline GAME_MESSAGE_TYPE getGameMessageType() const {
			return gameMessageType;
		};

		static MessagesAck * Unpack(MessageHeader, uint32_t, uint8_t*);

		inline void Update(){};


	protected:
		GAME_MESSAGE_TYPE gameMessageType;
};

class JoinAck: public MessagesAck {
	public:
		inline JoinAck(MessageHeader header, uint8_t statusAck, uint8_t idAck) :
				MessagesAck(header, GAME_MESSAGE_TYPE::JOIN) {
			status = statusAck;
			id = idAck;
		};

		inline ~JoinAck() {;};

		int PackSelf(uint8_t* payload);
		static JoinAck* Unpack(MessageHeader header, uint32_t length, uint8_t* payload);
		const uint8_t& getStatus() const {return status;}
	public:
		uint8_t status;
		uint16_t id;
};

class NickAck: public MessagesAck {
	public:
		inline NickAck(MessageHeader header, uint8_t nickStatus) :
			MessagesAck(header, GAME_MESSAGE_TYPE::NICK) {
			status = nickStatus;
		};

		inline ~NickAck(){;};

		int PackSelf(uint8_t* payload);
		static NickAck* Unpack(MessageHeader header, uint32_t length, uint8_t* payload);
		const uint8_t& getStatus() const {return status;}
	private:
		uint8_t status;
};

class ExitAck: public MessagesAck {
	public:
		inline ExitAck(MessageHeader header) :
			MessagesAck(header, GAME_MESSAGE_TYPE::EXIT) {;};

		inline ~ExitAck(){;};

		int PackSelf(uint8_t* payload);
		static ExitAck* Unpack(MessageHeader header, uint32_t length, uint8_t* payload);
};

class RestartAck: public MessagesAck {
	public:
		inline RestartAck(MessageHeader header) :
			MessagesAck(header, GAME_MESSAGE_TYPE::RESTART) {;};

		inline ~RestartAck(){;};

		int PackSelf(uint8_t* payload);
		static RestartAck* Unpack(MessageHeader header, uint32_t length, uint8_t* payload);
};

class GameEndAck: public MessagesAck {
	public:
		inline GameEndAck(MessageHeader header) :
			MessagesAck(header, GAME_MESSAGE_TYPE::GAME_END) {;};

		inline ~GameEndAck(){;};

		int PackSelf(uint8_t* payload);
		static GameEndAck* Unpack(MessageHeader header, uint32_t length, uint8_t* payload);
};

class PlayerDeadAck: public MessagesAck {
	public:
		inline PlayerDeadAck(MessageHeader header) :
			MessagesAck(header, GAME_MESSAGE_TYPE::PLAYER_DEAD) {;};

		inline ~PlayerDeadAck(){;};

		int PackSelf(uint8_t* payload);
		static PlayerDeadAck* Unpack(MessageHeader header, uint32_t length, uint8_t* payload);
};

class PlayerOutAck: public MessagesAck {
	public:
		inline PlayerOutAck(MessageHeader header) :
			MessagesAck(header, GAME_MESSAGE_TYPE::PLAYER_OUT) {;};

		inline ~PlayerOutAck(){;};

		int PackSelf(uint8_t* payload);
		static PlayerOutAck* Unpack(MessageHeader header, uint32_t length, uint8_t* payload);
};

#endif /* SRC_INET_MESSAGESACK_HPP_ */
