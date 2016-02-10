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

enum TEST_STATES {
	JOINING, JOINING_ACK, NICKING, NICKING_ACK, GAME_UPDATING
};

void TestMessagesLoop() {
	std::cout << "USING MESSAGE TEST LOOP" << std::endl;
	TEST_STATES testStates = TEST_STATES::JOINING;

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
	testStates = TEST_STATES::JOINING_ACK;

	int loopCounter = 0;
	while (loopCounter < 1000) {
		std::cout << "==================================== LOOP START ====================================" << std::endl;
		connection->update();

		if (testStates != TEST_STATES::GAME_UPDATING) {
			std::vector<MessagesAck*> acks = connection->getAcks();

			if (acks.size() > 0) {
				for (auto& ack : acks) {
					memset(testBuffer, 0, BUFFER_SIZE);
					if (ack->getGameMessageType() == GAME_MESSAGE_TYPE::JOIN && testStates == TEST_STATES::JOINING_ACK) {
						uint16_t id = static_cast<JoinAck*>(ack)->getUserID();
						uint8_t status = static_cast<JoinAck*>(ack)->getStatus();
						dummyGameMessageHeader.user_id = id;
						dummyGameMessageHeader.gameTime = static_cast<JoinAck*>(ack)->getgameTime();
						messageLenght = (new JoinAck(dummyGameMessageHeader, status, id))->PackSelf(testBuffer);
						connection->send(testBuffer, messageLenght);
//					std::cout << "NICK ID: " << unsigned(static_cast<JoinAck*>(ack)->getUserID()) << std::endl;
						testStates = TEST_STATES::NICKING;

						memset(testBuffer, 0, BUFFER_SIZE);
//					std::cout << "Main.cpp " << unsigned(dummyGameMessageHeader.user_id) << std::endl;

						if (testStates == TEST_STATES::NICKING) {
							Nick * nick = new Nick(dummyGameMessageHeader, "Oskar");
							messageLenght = nick->PackSelf(testBuffer);
							connection->send(testBuffer, messageLenght);
							testStates = TEST_STATES::NICKING_ACK;
						}

					} else if (ack->getGameMessageType() == GAME_MESSAGE_TYPE::NICK && testStates == TEST_STATES::NICKING_ACK) {
//					std::cout << "Main.cpp " << unsigned(ack->getMessageHeaderUserID()) << std::endl;
						NickAck * nickAck = static_cast<NickAck*>(ack);
						messageLenght = nickAck->PackSelf(testBuffer);
						connection->send(testBuffer, messageLenght);
						testStates = TEST_STATES::GAME_UPDATING;

					}
				}
				continue;
			}
		}

		if (testStates == TEST_STATES::GAME_UPDATING) {
			std::vector<GameUpdate*> gameUpdates = connection->getGameUpdateMessages();
			if (gameUpdates.size() > 0) {
				for (auto& update : gameUpdates) {
					memset(testBuffer, 0, BUFFER_SIZE);
//					std::cout << "Main.cpp - GameUpdate - Dir_X: " << update->getDirX() << std::endl;
//					std::cout << "Main.cpp - GameUpdate - Dir_Y: " << update->getDirY() << std::endl;
//					std::cout << "Main.cpp - GameUpdate - NumberOfObjects: " << update->getNumberOfObjects() << std::endl;
//					std::cout << "Main.cpp - GameUpdate - NumberOfPlayers: " << unsigned(update->getNumberOfPlayers()) << std::endl;

				}
				continue;
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
