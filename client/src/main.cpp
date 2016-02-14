#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <random>


#include "Engine.hpp"
#include "Inet/InetConnection.hpp"

#include "scenes/Game.hpp"
#include "scenes/IPDialog.hpp"
#include "scenes/NickDialog.hpp"

using namespace std;

enum TEST_STATES {
	JOINING, JOINING_ACK, NICKING, NICKING_ACK, GAME_RUNNING, GAME_ENDING, EXITING_GAME, EXITING_GAME_ACK
};

TEST_STATES testStates = TEST_STATES::JOINING;

void printGameStateAsString(){
	string stateAsString;
	switch (testStates) {
		case TEST_STATES::JOINING:
			stateAsString = "JOINING";
			break;
		case TEST_STATES::JOINING_ACK:
			stateAsString = "JOINING_ACK";
			break;
		case TEST_STATES::NICKING:
			stateAsString = "NICKING";
			break;
		case TEST_STATES::NICKING_ACK:
			stateAsString = "NICKING_ACK";
			break;
		case TEST_STATES::GAME_RUNNING:
			stateAsString = "GAME_RUNNING";
			break;
		case TEST_STATES::GAME_ENDING:
			stateAsString = "GAME_ENDING";
			break;
		case TEST_STATES::EXITING_GAME:
			stateAsString = "EXITING_GAME";
			break;
		case TEST_STATES::EXITING_GAME_ACK:
			stateAsString = "EXITING_GAME_ACK";
			break;
		default:
			stateAsString = "CANNOT DEFINE";
			break;
	}
	std::cout << "Entering state: " << stateAsString << std::endl;
}

int uliuli (){
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,10000); // distribution in range [1, 6]

    return dist6(rng);
}

void TestMessagesLoop(string ip_addr) {
	cout << "USING MESSAGE TEST LOOP" << endl;
	testStates = TEST_STATES::JOINING;
	printGameStateAsString();

	InetConnection * connection = new InetConnection();
	connection->init();
	
	connection->setIP(ip_addr);
	connection->connectUDP();
	uint8_t testBuffer[BUFFER_SIZE];
	MessageHeader dummyGameMessageHeader = connection->createDummyHeader(0, 123123, MESSAGE_TYPE::GAME_MESSAGE, 0);
	dummyGameMessageHeader.user_id = 0;
	dummyGameMessageHeader.gameTime = 123123;
	Join * joinMessage = new Join(dummyGameMessageHeader);
	int messageLenght = joinMessage->PackSelf(testBuffer);
	connection->send(testBuffer, messageLenght);
	testStates = TEST_STATES::JOINING_ACK;
	printGameStateAsString();

	int loopCounter = 0;
	while (loopCounter < 1000) {
		std::cout << "==================================== " << loopCounter << ": LOOP START "<< "====================================" << std::endl;
		connection->update();
		vector<Message*> messages;
		if (testStates == TEST_STATES::JOINING ||
				testStates == TEST_STATES::JOINING_ACK ||
				testStates == TEST_STATES::NICKING ||
				testStates == TEST_STATES::NICKING_ACK
				) {



			messages = connection->getMessagesOfType(MESSAGE_TYPE::ACK);
			if (messages.size() > 0) {
				for (auto& a : messages) {
					MessageAck* ack = static_cast<MessageAck*>(a);
					memset(testBuffer, 0, BUFFER_SIZE);
					if (ack->getGameMessageType() == GAME_MESSAGE_TYPE::JOIN && testStates == TEST_STATES::JOINING_ACK) {
						JoinAck* joinAck = static_cast<JoinAck*>(ack);
						MessageHeader headerForNick = connection->createDummyHeader(joinAck->getUserID(), joinAck->getGameTime(),
								joinAck->getMessageType(), joinAck->getPayloadSize());
						connection->setID(joinAck->getUserID());
						testStates = TEST_STATES::NICKING;
						printGameStateAsString();

						memset(testBuffer, 0, BUFFER_SIZE);

						if (testStates == TEST_STATES::NICKING) {
							cout << uliuli() << endl;
							Nick * nick = new Nick(headerForNick, "Oskar" + to_string(uliuli()));
							messageLenght = nick->PackSelf(testBuffer);
							connection->send(testBuffer, messageLenght);
							testStates = TEST_STATES::NICKING_ACK;
							printGameStateAsString();
						}

					} else if (ack->getGameMessageType() == GAME_MESSAGE_TYPE::NICK && testStates == TEST_STATES::NICKING_ACK) {
//						std::cout << "Main.cpp " << unsigned(ack->getMessageHeaderUserID()) << std::endl;
//						std::cout << "Main.cpp " << unsigned(ack->getMessageType()) << std::endl;
//						std::cout << "Main.cpp " << unsigned(ack->getGameMessageType()) << std::endl;
						NickAck * nickAck = static_cast<NickAck*>(ack);
						messageLenght = nickAck->PackSelf(testBuffer);
						connection->send(testBuffer, messageLenght);
						testStates = TEST_STATES::GAME_RUNNING;
						printGameStateAsString();
					}
				}
				continue;
			}
		}

		if (testStates == TEST_STATES::GAME_RUNNING) {

			Move move = Move(connection->createDummyHeader(connection->getID(), 0, MESSAGE_TYPE::PLAYER_MOVEMENT, 0),
					0,
					140 + loopCounter,
					120 + loopCounter,
					1,
					2);

			memset(testBuffer, 0, BUFFER_SIZE);
			int moveLenght = move.PackSelf(testBuffer);

			connection->send(testBuffer, moveLenght);

			if (connection->getGameEnding()) {
				testStates = TEST_STATES::GAME_ENDING;
				printGameStateAsString();
				continue;
			}

			if(loopCounter == 200){
				testStates = TEST_STATES::EXITING_GAME;
				printGameStateAsString();
				Exit exitMessage = Exit(connection->createDummyHeader(connection->getID(), 0, MESSAGE_TYPE::GAME_MESSAGE, 0));
				memset(testBuffer, 0, BUFFER_SIZE);
				int messageSize = exitMessage.PackSelf(testBuffer);
				connection ->send(testBuffer, messageSize);
				testStates = TEST_STATES::EXITING_GAME_ACK;
				printGameStateAsString();
			}

			messages = connection->getMessagesOfType(MESSAGE_TYPE::GAME_MESSAGE, GAME_MESSAGE_TYPE::PLAYER_DEAD);
		if (messages.size() > 0) {
			PlayerDead* playerDeadMessage = static_cast<PlayerDead*>(messages.front());
			memset(testBuffer, 0, BUFFER_SIZE);
			std::cout << "Main.cpp - PlayerDead - ID: " << playerDeadMessage->getPlayerID() << std::endl;
		}



			messages = connection->getMessagesOfType(MESSAGE_TYPE::GAME_MESSAGE, GAME_MESSAGE_TYPE::GAME_UPDATE);
			if (messages.size() > 0) {
				for (auto& update : messages) {
					GameUpdate* gamemessage = static_cast<GameUpdate*>(update);
					memset(testBuffer, 0, BUFFER_SIZE);
//					std::cout << "Main.cpp - GameUpdate - Pos_X: " << gamemessage->getPosX() << std::endl;
//					std::cout << "Main.cpp - GameUpdate - Pox_Y: " << gamemessage->getPosY() << std::endl;
//					std::cout << "Main.cpp - GameUpdate - Dir_X: " << gamemessage->getDirX() << std::endl;
//					std::cout << "Main.cpp - GameUpdate - Dir_Y: " << gamemessage->getDirY() << std::endl;
//					std::cout << "Main.cpp - GameUpdate - NumberOfObjects: " << gamemessage->getNumberOfObjects() << std::endl;
//					std::cout << "Main.cpp - GameUpdate - NumberOfPlayers: " << unsigned(gamemessage->getNumberOfPlayers()) << std::endl;
					vector<GamePlayer*> gamePlayers = gamemessage->getGamePlayers();
					for (auto& gamePlayer : gamePlayers) {
//						std::cout << "Main.cpp - GameUpdate - Players(" << ") posX:" << gamePlayer->getPosX() << " posY: " << gamePlayer->getPosY() << " size:" << gamePlayer->getSize()  << std::endl;

					}

					vector<GameObject*> gameObjects = gamemessage->getGameObjects();
					for (auto& gameObject : gameObjects) {
//						std::cout << "Main.cpp - GameUpdate - Object(" << gameObject->getObjectID() << ") posX:" << gameObject->getLocX() << " posY: " << gameObject->getLocY() << std::endl;
					}
				}
			}
		}
		messages = connection->getMessagesOfType(MESSAGE_TYPE::GAME_MESSAGE, GAME_MESSAGE_TYPE::POINTS);
		if (messages.size() > 0) {
			for (auto& update : messages) {
				Points* pointsMessage = static_cast<Points*>(update);
//				std::cout << "Main.cpp - GameUpdate - Points: ID:" << pointsMessage->getPlayerIDs().front() <<
//						" nick: " << pointsMessage->getPlayerNicks().front() <<
//						" points:" << pointsMessage->getPlayerPoints().front() << std::endl;
			}
		}

		if (testStates == TEST_STATES::GAME_ENDING) {
			std::cout << "Main.cpp - GAME_ENDING: Game ended by server" << std::endl;
			exit(0);
		}

		if(testStates == TEST_STATES::EXITING_GAME_ACK){
			messages = connection->getMessagesOfType(MESSAGE_TYPE::GAME_MESSAGE, GAME_MESSAGE_TYPE::PLAYER_OUT);
			if(messages.size() > 0){
				std::cout << "Main.cpp - EXITING: Game ended by user" << std::endl;
				PlayerOut* playerOut = static_cast<PlayerOut*>(messages.front());
				PlayerOutAck playerOutAck = PlayerOutAck(connection->createDummyHeader(playerOut->getMessageHeaderUserID(), 0, MESSAGE_TYPE::ACK, 0), playerOut->getMessageHeaderUserID());
//				std::cout << "Main.cpp - EXITING- PlayerOutAck: " << playerOutAck.getAckMessagePackerID() << std::endl;
//				std::cout << "Main.cpp - EXITING- PlayerOutAck: " << playerOutAck.getGameTime() << std::endl;
//				std::cout << "Main.cpp - EXITING- PlayerOutAck: " << playerOutAck.getMessageHeaderUserID() << std::endl;
//				std::cout << "Main.cpp - EXITING- PlayerOutAck: " << getSubMessageTypeAsString(playerOutAck.getGameMessageType()) << std::endl;
//				std::cout << "Main.cpp - EXITING- PlayerOutAck: " << getMessageTypeAsString(playerOutAck.getMessageType())<< std::endl;
				// Send three messages so most likely at least one will get there
				memset(testBuffer, 0, BUFFER_SIZE);
				int messageSize = playerOutAck.PackSelf(testBuffer);
				connection->send(testBuffer, messageSize);
				connection->send(testBuffer, messageSize);
				connection->send(testBuffer, messageSize);
				exit(0);
			}
		}


		std::cout << "==================================== LOOP END ====================================" << std::endl;
		loopCounter++;
	}
}

int main(int argc, char* argv[]) {
	string ip_addr = "127.0.0.1";
	if(argc != 1){
		ip_addr = argv[1];
	}
	#ifdef MESG_TEST
		TestMessagesLoop(ip_addr);
		return 0;
	#endif

	std::cout << "Starting game" << std::endl;
	Engine* engine = new Engine();
	engine->addScenes( { { "Game", new Game() }, { "NickDialog", new NickDialog() }, { "IPDialog", new IPDialog() } });
	engine->run("IPDialog");
	delete engine;

	return 0;
}
