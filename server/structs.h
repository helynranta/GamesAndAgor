#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdarg.h>


#define BUFFERSIZE 1500

/*
char buffer[1000];
memset($buffer,0,1000);

int pID = 1;
int gametime = 12;
int msgtype = 6;
int plLength = 0;
int position = 0;

*(uint16_t*)&buffer[position] = htons(pID);
position += sizeof(uint16_t);

*(uint32_t*)&buffer[position] = htonl(gametime);
position += sizeof(uint32_t);

*(uint8_t*)&buffer[position] = msgtype;
position += sizeof(uint8_t);

*(uint32_t*)&buffer[position] = htonl(plLength);
position += sizeof(uint32_t);
*/

/*
NAMING INFO:
Static objects are referred to as 'objects' and 'particle' can be either a
'player' or a 'object'
*/

// List for particles that are near
typedef struct Near {
	void *pParticle;  // typecasted to either *Player or *Object
	struct Near *pNext;
} Near;

// Struct for Players
typedef struct Player {
	int ID;
	char nick[12];
	int lastPacket; // player's game time (game time of last movement packet)
	int location[2];  // | X ¦ Y |
	int direction[2];  // | X ¦ Y |
	int scale;
	int points;
	int state;  // | alive | eaten | exited |
	int ping;

	// For storing players and objects nearby
	Near *nearPlayers;
	Near *nearObjects;

	// For storing player's IP address //
	struct sockaddr_storage addressStorage;
	struct sockaddr* address;
	/* NOTE: this pointer must point to addressStorage. It can't be done here,
	 * it must be typecasted in a new player function:
	 * address = (struct sockaddr*)&addressStorage; */

	struct Player *pNext;
} Player;

// Struct for Objects
typedef struct Object {
	int ID;
	int location[2];
	struct Object *pNext;
} Object;

typedef struct Ack{
	int packetID;
	int gameTimeSent;
	char msg[BUFFERSIZE];
	struct Ack *pNext;
	struct Ack *pPrev;
} Ack;

typedef struct Game {
	int gameTime;
	int nPlayers;
	Player *sPlayers;
	Object *sObjects;
	Ack *sAcks; // List of messages that haven't been acknowledged
	int packetID; // Increasing integer value to serialize ack packets
	int pingID; // Increasing integer value to serialize ping packets
} Game;

/* Message types */

enum MESSAGE_TYPE {
    GAME_MESSAGE = 0,
    ACK = 1,
    PLAYER_MOVEMENT = 2,
    PLAYER_CHAT_MESSAGE = 3,
    STATISTICS_MESSAGE = 4

};

/* Message subtypes*/
enum GAME_MESSAGE_TYPE {
    JOIN = 0,
    NICK = 1,
    EXIT = 2,
    RESTART = 3,
    GAME_END = 4,
    GAME_UPDATE = 5,
    POINTS = 6,
    PLAYER_DEAD = 7,
    PLAYER_OUT = 8,
	PING = 9
};

/* GAME_UPDATE MSGS */
// enum gameUpdate { };
