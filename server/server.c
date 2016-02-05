/*
* CT30A5002 Games and Networking
* udpexample, UDP server and client example
*
* Contains simple UDP server that waits for a char from some client and
* quits after it has been received.
*
* Contains simple UDP client that connects to given IP address and sends
* characters then quits.
*


TAA ON HYVÄ POHJA LÄHTEE LIIKKEELLE
*/

#include "server_unpackers.h"
#include "server_functions.h"
#define SIZE 256
#define TCPPORT "8889"

int sendMsg(struct sockaddr *to, struct Ack *ack, struct Packet packet){
/* Wait what? !? */
return 0;
}




int server(char* port) {
	struct Game game;
  int socketfd = -1, activity, fdmax, listener = -2, newfd, nbytes;
	fd_set readset, master;
	struct timeval tvSelect, tvUpdate1, tvUpdate2, tv;
	long time1, time2;

  struct addrinfo hints = { .ai_flags = AI_PASSIVE,	/* Get addresses suitable for bind */
                            .ai_family = PF_UNSPEC,
                            .ai_socktype = SOCK_DGRAM,	/* Datagram socket - UDP */
                            .ai_protocol = IPPROTO_UDP};/* UDP protocol */

  struct addrinfo *result = NULL, *iter = NULL;
  struct sockaddr_storage client_addr;

  char hostbuffer[NI_MAXHOST] = { 0 };
  char portbuffer[NI_MAXSERV] = { 0 };
  char recvbuffer[SIZE] = { 0 };
	char sendbuffer[SIZE] = {0};
	tv.tv_usec = 1000000;
	tv.tv_sec = 0;

  socklen_t addrlen = 0;
  unsigned int optval = 0;
  int dgramlen = 0;

	FD_ZERO(&master);
	FD_ZERO(&readset);

  printf("| - - - - - S E R V E R - - - - - |\n");

  if (atoi(port) >= 1024 && atoi(port) <= 65000) {
    // Get my (AI_PASSIVE) addresses that are suitable for bind
    if(getaddrinfo(NULL,port,&hints,&result)) {
      perror("cannot get addresses for server UDP");
      return -1;
    }

    // Go through all addresses returned with specified hints and use the first one
    for(iter = result; iter != NULL; iter = iter->ai_next) {
      if ((socketfd = socket(iter->ai_family,iter->ai_socktype,iter->ai_protocol)) < 0) {
        perror("socket()");
        return -1;
      }

      //Try to bind to this address
      if (bind(socketfd,iter->ai_addr, iter->ai_addrlen) < 0 ) {
        close(socketfd); /* Even when bind fails, socket remains, close it */
        perror("bind()");
        return -1;
      }

      break;
    }

		/* TCP socket initialization */
		struct addrinfo hintstcp = { .ai_flags = AI_PASSIVE,	/* Get addresses suitable for bind */
	                            .ai_family = AF_UNSPEC,
	                            .ai_socktype = SOCK_STREAM};/*  protocol */
		/****************************/
		result = NULL;
		iter = NULL;

		if(getaddrinfo(NULL,"8889",&hintstcp,&result)) {
      perror("cannot get addresses for server TCP");
      return -1;
    }
		for(iter = result; iter != NULL; iter = iter->ai_next) {
      if ((listener = socket(iter->ai_family,iter->ai_socktype,iter->ai_protocol)) < 0) {
        perror("socket()");
        return -1;
      }

      //Try to bind to this address
      if (bind(listener,iter->ai_addr, iter->ai_addrlen) < 0 ) {
        close(socketfd); /* Even when bind fails, socket remains, close it */
        perror("bind()");
        return -1;
      }

      break;
    }
		/*****************************/

    freeaddrinfo(result);

		if(listen(listener, 10) == -1){
			perror("Listen");
			exit(3);
		}
		else {
			printf("TCP listener OK\n");
		}

    /* Try to get the maximum length for read buffer */
    socklen_t optlen = sizeof(optval);
    if(getsockopt(socketfd,SOL_SOCKET,SO_RCVBUF,&optval,&optlen) == -1) perror("Cannot get read buffer size getsockopt()");
    else printf("Server: Read buffer in bytes: %u\n",optval);

    memset(&recvbuffer,0,SIZE);
    printf("Server: Waiting for datagram..\n");

    addrlen = sizeof(client_addr);
    struct sockaddr* client_address = (struct sockaddr*) &client_addr;

		gettimeofday(&tvUpdate1, NULL);
		gettimeofday(&tvUpdate2, NULL);

		time1 = tvUpdate1.tv_sec * 1000 + tvUpdate1.tv_usec / 1000;
		time2 = tvUpdate2.tv_sec * 1000 + tvUpdate2.tv_usec / 1000;

		tvSelect.tv_sec = 0;
		tvSelect.tv_usec = 1000;

		// Add socket to the master set
		FD_SET(socketfd, &master);
		FD_SET(listener, &master);
		fdmax = listener+1;

		/* Create a game */
		Game game;
		game.sPlayers=NULL;
		game.sObjects=NULL;
		game.sAcks=NULL;
		int tavut = -2;
		while (1) {

			// Refresh select() set
			//FD_ZERO(&readset);
			//FD_SET(socketfd, &readset);
			readset = master; // /* Copy master fd_set, so that won't change*/

			/* rest timeout values */
			tvSelect.tv_sec = 0;
			tvSelect.tv_usec = 1000;


			// Start waiting for socket activity
			if((activity = select(fdmax+1, &readset, NULL, NULL, &tvSelect)) == -1){
				perror("select");
				exit(4);
			}

			/* Run through existing connections, see if any data to read */
			for(int i = 0; i <= fdmax; i++){
				if(FD_ISSET(i, &readset)) {
					if(i == listener){
						/* New TCP connection */
						addrlen = sizeof(client_addr);
						newfd = accept(listener, (struct sockaddr*) &client_addr, &addrlen);
						if(newfd == -1){
							perror("Accept");
						}
						else {
							/* Add to master set */
							send(newfd, "server: connection established", 256, 0);
							FD_SET(newfd, &master);
							if(newfd > fdmax)
								fdmax = newfd;

								/* Add tcp connection to player */
						}
					}
						else if(i == socketfd) {
							/* UDP msg */
							/* Try to receive something (expecting a char - length = 1 byte).. */
							dgramlen = recvfrom(socketfd,&recvbuffer,SIZE,0,client_address,&addrlen);
							struct Packet packet;
							packet = unpackPacket(recvbuffer, client_address, socketfd, addrlen);

							switch (packet.msgType) {

								// Game message packet
								case GAME_MESSAGE:
									printf("Game message packet received!\n");

									switch (packet.subType) {

										case JOIN:
											printf("Player joins game!\n");

											msgPacker(sendbuffer, &game, packet.ID, ACK, JOIN, 0,1);
											tavut = sendto(socketfd, sendbuffer, SIZE, 0, &packet.senderAddr, addrlen);
											printf("Lähetettiin clientille JOIN ACK: %d\n", tavut);
											break;

										case NICK:
											printf("Player inserted nick!\n");
											printf("Nick: %s\n", packet.nick);
											int nickStatus = -1;
											nickStatus = checkNick(packet.nick, game.sPlayers);
											printf("Nick status: %d\n", nickStatus);
											msgPacker(sendbuffer, &game, packet.ID, ACK, NICK, 0, nickStatus);
											printf("ACKID: %d\n", game.sAcks->packetID);
											sendto(socketfd, sendbuffer, SIZE, 0, &packet.senderAddr, addrlen);

											/* If nick OK add new player */
											if(nickStatus == 1){
												newPlayer(&game.sPlayers, packet, game.nPlayers);
												game.nPlayers++;
												/* add the tcp connection for this new player */
												/* Actually no, let's not do it so we can have some fun */
											}
											break;

										case EXIT:
											printf("Player exits the game!\n");

											// Set player as OUT
											Player *p = getPlayer(packet.ID, game.sPlayers);
											p->state = OUT;

											// Send ACK to player
											msgPacker(sendbuffer, &game, packet.ID, ACK, EXIT, packet.ID, 0);
											sendto(socketfd, sendbuffer, SIZE, 0, &packet.senderAddr, addrlen);

											// Inform other clients
											Player *pPla = game.sPlayers;
											while (pPla != NULL) {
												// Don't send to exiting player
												if (pPla->ID != packet.ID) {
													msgPacker(sendbuffer, &game, pPla->ID, GAME_MESSAGE, PLAYER_OUT, packet.ID, 0);
													sendto(socketfd, sendbuffer, SIZE, 0, &pPla->address, addrlen);
													pPla = pPla->pNext;
												}
											}

											break;
									}

									break;

								// Ack packet
								case ACK:
									printf("Ack packet received!\n");
									/* Handle ack */
									/* remove ack from server's own ack list */
									removeAck(&game.sAcks, packet.ackID);

									break;

								// Player movement packet
								case PLAYER_MOVEMENT:
									printf("Player movement packet received!\n");
									/* Do game functions:
									calculate nearby objects etc. */
									/* update player's position */
									Player *p = getPlayer(packet.ID, game.sPlayers);

									/* x and y position */
									p->location[0] = packet.posX;
									p->location[1] = packet.posY;

									/* dirX and dirY */
									p->direction[0] = packet.dirX;
									p->direction[1] = packet.dirY;
									break;

								// Statisic packet
								case STATISTICS_MESSAGE:
									printf("Client ping packet returned!\n");
									break;

								default:
									printf("Invalid packet received!\n");

							}

							printf("PacketID: %d\n", packet.ID);
							printf("msgType: %d\n", packet.msgType);
							printf("subtype: %d\n", packet.subType);
							/*************/
						}
						else{
							/* TCP CHAT MSG */
							if((nbytes = recv(i, recvbuffer, sizeof(recvbuffer), 0)) <= 0){
								/* Connection closed or error */
								if(nbytes == 0){
									/* connection closed */
								}
								else{
									perror("recv");
								}
								close(i);
								FD_CLR(i, &master);

							}
							else {
								/* TCP chat */
								for(int j = 0; j <= fdmax; j++){
									if(FD_ISSET(j, &master)) {
										/* skip listener */
										if(j != listener) {
											if(sendAllTCP(j, recvbuffer, &nbytes) == -1) {
												perror("sendAllTCP failure");
											}
										}
									}
								}
							}
						}
					}
				}
				/* Do game functions */
				ComputeNearParticles(game.sPlayers, game.sObjects);

				/* If player eaten inform the player and others? */


				/* If eough time has passed send game update */
				if ((time2 - time1) >= 500) {
					//printf("Game update!\n");
					gettimeofday(&tvUpdate1, NULL);
					time1 = tvUpdate1.tv_sec * 1000 + tvUpdate1.tv_usec / 1000;

					/* Send game update to everyone */
					sendGameUpdate(&game, sendbuffer, socketfd, addrlen);
					}
					gettimeofday(&tvUpdate2, NULL);
					time2 = tvUpdate2.tv_sec * 1000 + tvUpdate2.tv_usec / 1000;
				}

			}

			// UPD activity





/* Question:
* Since we know the address and port client is using, it should be easy to send something
* back. Think about the operation and reverse it without initializing new address.
* What can be changed in sendto()-function parameters if message is going to be sent to
* someone else?
*/



  else {
    printf("Server: Invalid port. Choose something between 1024 - 65000\n");
    return -1;
  }

  return 0;
}


int main(int argc, char *argv[])
{
  /* server */
  if (argc == 2)
  {
    if (server(argv[1]) == 0) printf("Server: exited with success\n");
    else
    {
      printf("Errors with server\n");
      return -1;
    }
  }
  /* client */
  else if (argc == 3)
  {

  }
  /* error */
  else
  {
    printf("Invalid amount of arguments.\nUsage:\n\
      server: %s <portnumber>\n\
      client: %s <portnumber> <server ip>\n",argv[0],argv[0]);
    return -1;
  }
  return 0;
}
