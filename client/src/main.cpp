#include <iostream>
#include <string>
#include <map>

#include "Engine.hpp"
#include "Inet/InetConnection.hpp"

#include "scenes/Game.hpp"
#include "scenes/IPDialog.hpp"
#include "scenes/NickDialog.hpp"

using namespace std;

// Defines if we really want to start the game or just use simple game loop to test messages
//#define MESG_TEST = 0

void TestMessagesLoop() {
	std::cout << "USING MESSAGE TEST LOOP" << std::endl;

	InetConnection * connection = new InetConnection();
	connection->init();
	connection->connectUDP();
	uint8_t testBuffer[BUFFER_SIZE];
	MessageHeader dummyGameMessageHeader;
	dummyGameMessageHeader.user_id = 0;
	dummyGameMessageHeader.gameTime = 123123;
	Join * joinMessage = new Join(dummyGameMessageHeader);
	int messageLenght = joinMessage->PackSelf(testBuffer);
	connection->send(testBuffer, messageLenght);

	int loopCounter = 0;
	while (loopCounter < 3) {
		std::cout << "==================================== LOOP START ====================================" << std::endl;
		connection->update();
		std::vector<MessagesAck*> acks = connection->getAcks();

		if (acks.size() > 0) {
			for (auto& ack : acks) {
				memset(testBuffer, 0, BUFFER_SIZE);
				std::cout << getSubMessageTypeAsString(ack->getGameMessageType()) << std::endl;
				if (ack->getGameMessageType() == GAME_MESSAGE_TYPE::JOIN) {
					uint16_t id = static_cast<JoinAck*>(ack)->id;
					uint8_t status = static_cast<JoinAck*>(ack)->status;
					dummyGameMessageHeader.user_id = id;
					dummyGameMessageHeader.gameTime = static_cast<JoinAck*>(ack)->getgameTime();
					messageLenght = (new JoinAck(dummyGameMessageHeader, status, id))->PackSelf(testBuffer);
					connection->send(testBuffer, messageLenght);

					memset(testBuffer, 0, BUFFER_SIZE);
					Nick * nick = new Nick(dummyGameMessageHeader, "Oskar");
					messageLenght = nick->PackSelf(testBuffer);
					connection->send(testBuffer, messageLenght);

				} else if (ack->getGameMessageType() == GAME_MESSAGE_TYPE::NICK) {
					NickAck * nickAck = static_cast<NickAck*>(ack);
					messageLenght = nickAck->PackSelf(testBuffer);
					connection->send(testBuffer, messageLenght);

				}
			}
		}
		std::cout << "==================================== LOOP END ====================================" << std::endl;
		loopCounter++;
	}
}


int main(void) {

#ifdef MESG_TEST
	TestMessagesLoop();
	return 0;
#endif
	std::cout << "Starting game" << std::endl;
	Engine* engine = new Engine();
	engine->addScenes( { { "Game", new Game() }, { "NickDialog", new NickDialog() }, { "IPDialog", new IPDialog() } });
	engine->run("IPDialog");
	delete engine;

	return 0;
}
