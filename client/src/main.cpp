#include <iostream>
#include <string>

#include "Engine.hpp"

#include "scenes.hpp"

using namespace std;

//int main(void) {
//    Engine* engine = new Engine();
//    import_scenes();
//    engine->addScenes(scenes);
//    engine->run("Game");
//    delete engine;
//    return 1;
//}

#include "Inet/InetConnection.hpp"

int main(void) {
	InetConnection * connection = new InetConnection();
	connection->init("192.168.0.159", "8888");
	//connection->startConnection("127.0.0.1", "3321");
	uint8_t testBuffer[1500];
	MessageHeader dummyGameMessageHeader;
	dummyGameMessageHeader.user_id = 16;
	dummyGameMessageHeader.game_time = 23;
	Join * joinMessage = new Join(dummyGameMessageHeader);
	int qew = joinMessage->PackSelf(testBuffer);

	// connection->send(reinterpret_cast<char*>(&testBuffer));

	connection->send(testBuffer, qew);
//	while (1) {
//		connection->update();
//	}
	return 1;
}

