#include <iostream>
#include <string>

#include "Engine.hpp"

#include "scenes.hpp"
#include "Inet/InetConnection.hpp"

using namespace std;

//int main(void) {
//    Engine* engine = new Engine();
//    import_scenes();
//    engine->addScenes(scenes);
//    engine->run("Game");
//    delete engine;
//    return 1;
//}

enum GAME_STATES {
	JOIN_STATE, JOINING_STATE, NICK_STATE, NICK_ACCEPTED_STATE
};

int main(void) {
	GAME_STATES asd = GAME_STATES::JOIN_STATE;
	InetConnection * connection = new InetConnection();
	connection->init("192.168.0.159", "8888");
	//connection->startConnection("127.0.0.1", "3321");
	uint8_t testBuffer[1500];

	// connection->send(reinterpret_cast<char*>(&testBuffer));

	while (1) {
		connection->update();
		if (asd == GAME_STATES::JOIN_STATE) {
			MessageHeader dummyGameMessageHeader;
			dummyGameMessageHeader.user_id = 101;
			dummyGameMessageHeader.gameTime = 2300;
			Join joinMessage(dummyGameMessageHeader);
			int messageLength = joinMessage.PackSelf(testBuffer);
			connection->send(testBuffer, messageLength);
			asd = JOINING_STATE;

		} else if (asd == GAME_STATES::JOINING_STATE) {
			for (auto& message : connection->getJoinMessages()) {
				if(message->getMessageType() == MESSAGE_TYPE::ACK && message->getGameMessageType()){
					MessageHeader dummyGameMessageHeader;
					dummyGameMessageHeader.user_id = message->getUserID();
					dummyGameMessageHeader.gameTime = message->getgameTime();
					Nick nick(dummyGameMessageHeader, "Oskar");
					int messageLength = nick.PackSelf(testBuffer);
					connection->send(testBuffer, messageLength);

				}
			}

		}
	}
	return 1;
}

