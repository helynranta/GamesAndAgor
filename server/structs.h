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
	void *pParticle;
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
	int state;  // | alive | eaten | exited |

	// For stroring players and objects nearby
	Near *nearPlayers;
	Near *nearObjects;

	// For storing player's IP address //
	struct sockaddr_storage addressStorage;

	/* NOTE: this pointer must point to addressStorage. This can't be initialized here,
	 * it must be typecasted in a new player function:
	 * address = (struct sockaddr*)&addressStorage; */
	struct sockaddr* address;

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
	Player *sPlayers;
	Object *sObjects;
	Ack *sAcks; // List of messages that haven't been acknowledged
	int packetID; // Increasing integer value to serialize ack packets
} Game;

// Used to give arguments to and receive data from packet handling functions
typedef struct unknown {
	int msgType;
	int playerID;
	int packetID;
	Player *pPlayers;
	Object *pObjects;
	int ackType;
	int ackStatus; // if any
	int newPlayerID; // in case of Join

} unknown;


/**************************************
DATA STRUCTURE FOR PROTOCOL'S MESSAGES
**************************************/


/* The data that will be sent over the network */

typedef struct Header {
	uint16_t ID;
	uint32_t gameTime;
	uint8_t msgType;
	uint32_t payloadLength;
	void *payload;
} Header;

/* Message types */
enum msg {GAME, ACK, MOVEMENT, CHAT, STAT};

/* Game message types  */
enum gameMsg {JOIN, NICK, WAIT, EXIT, INIT, START, STOP, GAME_END, GAME_UPDATE};

/* GAME_UPDATE MSGS */
/*enum gameUpdate { };*/
