#include <iostream>
#include <string>
#include <map>

#include "Engine.hpp"
#include "Inet/InetConnection.hpp"

#include "scenes/Game.hpp"
#include "scenes/IPDialog.hpp"
#include "scenes/NickDialog.hpp"

using namespace std;



enum TEST_STATES {
	JOINING, JOINING_ACK, NICKING, NICKING_ACK, GAME_RUNNING, GAME_ENDING
};

void TestMessagesLoop() {
	std::cout << "USING MESSAGE TEST LOOP" << std::endl;
	TEST_STATES testStates = TEST_STATES::JOINING;

	InetConnection * connection = new InetConnection();
	connection->init();
	connection->connectUDP();
	uint8_t testBuffer[BUFFER_SIZE];
	MessageHeader dummyGameMessageHeader = connection->createDummyHeader(0, 123123, MESSAGE_TYPE::GAME_MESSAGE, 0);
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

		if (testStates != TEST_STATES::GAME_RUNNING) {
			std::vector<MessagesAck*> acks = connection->getAcks();

			if (acks.size() > 0) {
				for (auto& ack : acks) {
					memset(testBuffer, 0, BUFFER_SIZE);
					if (ack->getGameMessageType() == GAME_MESSAGE_TYPE::JOIN && testStates == TEST_STATES::JOINING_ACK) {
						JoinAck* joinAck = static_cast<JoinAck*>(ack);
						MessageHeader headerForNick = connection->createDummyHeader(joinAck->getUserID(), joinAck->getgameTime(),
								joinAck->getMessageType(), joinAck->getPayloadSize());

//						messageLenght = (new JoinAck(headerForJoinAckAndNick, joinAck->getStatus(), joinAck->getUserID()))->PackSelf(testBuffer);
//						connection->send(testBuffer, messageLenght);
						testStates = TEST_STATES::NICKING;

						memset(testBuffer, 0, BUFFER_SIZE);

						if (testStates == TEST_STATES::NICKING) {
							Nick * nick = new Nick(headerForNick, "Oskar");
							messageLenght = nick->PackSelf(testBuffer);
							connection->send(testBuffer, messageLenght);
							testStates = TEST_STATES::NICKING_ACK;
						}

					} else if (ack->getGameMessageType() == GAME_MESSAGE_TYPE::NICK && testStates == TEST_STATES::NICKING_ACK) {
						std::cout << "Main.cpp " << unsigned(ack->getMessageHeaderUserID()) << std::endl;
						std::cout << "Main.cpp " << unsigned(ack->getMessageType()) << std::endl;
						std::cout << "Main.cpp " << unsigned(ack->getGameMessageType()) << std::endl;
						NickAck * nickAck = static_cast<NickAck*>(ack);
						messageLenght = nickAck->PackSelf(testBuffer);
						connection->send(testBuffer, messageLenght);
						testStates = TEST_STATES::GAME_RUNNING;

					}
				}
				continue;
			}
		}

		if (testStates == TEST_STATES::GAME_RUNNING) {

			if (connection->getGameEnding()) {
				testStates = TEST_STATES::GAME_ENDING;
				continue;
			}

			std::vector<GameUpdate*> gameUpdates = connection->getGameUpdateMessages();
			if (gameUpdates.size() > 0) {
				for (auto& update : gameUpdates) {
					memset(testBuffer, 0, BUFFER_SIZE);
					std::cout << "Main.cpp - GameUpdate - Pos_X: " << update->getPosX() << std::endl;
					std::cout << "Main.cpp - GameUpdate - Pox_Y: " << update->getPosY() << std::endl;
					std::cout << "Main.cpp - GameUpdate - Dir_X: " << update->getDirX() << std::endl;
					std::cout << "Main.cpp - GameUpdate - Dir_Y: " << update->getDirY() << std::endl;
					std::cout << "Main.cpp - GameUpdate - NumberOfObjects: " << update->getNumberOfObjects() << std::endl;
					std::cout << "Main.cpp - GameUpdate - NumberOfPlayers: " << unsigned(update->getNumberOfPlayers()) << std::endl;

				}
				continue;
			}
		}

		if (testStates == TEST_STATES::GAME_ENDING) {

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
