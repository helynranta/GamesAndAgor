#define UDP_PORT 8888
#define TCP_PORT 8889
#define BUFFERSIZE 1024

#include "server_functions.h"

void createGameWorld();
void endGame();

int main(int argc, char *argv[]) {

	struct sockaddr_in tcpAddress, udpAddress;
	int udpSocket, tcpSocket, activity;
	fd_set readset;

	// Initialize other variables


	// Create TCP socket
	if ((tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		perror("TCP socket");

	// Create UDP socket
	if ((udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		perror("UDP socket");

	// TCP address struct
	memset((char *) &tcpAddress, 0, sizeof(tcpAddress));
	tcpAddress.sin_family = AF_INET;
	tcpAddress.sin_port = htons(TCP_PORT);
	tcpAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	// UDP address struct
	memset((char *) &udpAddress, 0, sizeof(udpAddress));
	udpAddress.sin_family = AF_INET;
	udpAddress.sin_port = htons(UDP_PORT);
	udpAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	// Bind TCP socket to port
	if (bind(tcpSocket, (struct sockaddr*)&tcpAddress, sizeof(tcpAddress)) == -1)
		perror("TCP bind");

	// Bind UDP socket to port
	if (bind(udpSocket, (struct sockaddr*)&udpAddress, sizeof(udpAddress)) == -1)
		perror("UDP bind");

	// Start listening TCP socket
	if (listen(tcpSocket, 10) > 0)
		perror("Listen error");

	printf("Server up and running...\n");

	// Initialize new game world here
	createGameWorld();

	// Loop for game actions
	while(1) {

		// Clear select set
		FD_ZERO(&readset);

		// Add sockets to select set
		FD_SET(tcpSocket, &readset);
		FD_SET(udpSocket, &readset);

		// Start waiting for activity
		activity = select(0, &readset, NULL, NULL, NULL);

		// Activity on TCP socket
		if (FD_ISSET(tcpSocket, &readset)) {

			printf("TCP activity...");

		// Activity on UDP socket
		} else if (FD_ISSET(udpSocket, &readset)) {

			printf("UDP activity...");

		// Game timer has expired
		} else if (0) {
			endGame();
		}

	}

	return 0;

}

// Create new game world
// Set NPC objects and inform existing clients about new game
void createGameWorld() {
	printf("Starting new game...");
}

// End game and clear expired game data
void endGame() {
	printf("Game over...");
}
