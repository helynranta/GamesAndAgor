#include <iostream>
#include <string>
#include <map>

#include "Engine.hpp"
#include "Inet/InetConnection.hpp"

#include "scenes/Game.hpp"
#include "scenes/IPDialog.hpp"
#include "scenes/NickDialog.hpp"

using namespace std;
/*
void TestMessagesLoop() {
	std::cout << "USING MESSAGE TEST LOOP" << std::endl;

	InetConnection * connection = new InetConnection();
	connection->init();
	uint8_t testBuffer[BUFFER_SIZE];
	MessageHeader dummyGameMessageHeader;
	dummyGameMessageHeader.user_id = 0;
	dummyGameMessageHeader.gameTime = 12;
	Join * joinMessage = new Join(dummyGameMessageHeader);
	int messageLenght = joinMessage->PackSelf(testBuffer);
	connection->send(testBuffer, messageLenght);

	int loocounter = 0;
	while (true) {
		std::cout << "LOOP START" << std::endl;
		connection->update();
		std::vector<MessagesAck*> acks = connection->getAcks();

		if (acks.size() > 0) {
			for (auto& ack : acks) {
				memset(testBuffer, 0, BUFFER_SIZE);
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
		std::cout << "LOOP END" << std::endl;
		loocounter++;
	}
}
*/
int main(void) {
	Engine* engine = new Engine();
	engine->addScenes( { { "Game", new Game() }, { "NickDialog", new NickDialog() }, { "IPDialog", new IPDialog() } });
	engine->run("IPDialog");
	delete engine;

	return 0;
}
