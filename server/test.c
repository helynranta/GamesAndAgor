/*#include "server_functions.h"

int main(int argc, char const *argv[]){
	int i;
	char bf[BUFFERSIZE];

	Near *Tester;
	Game newGame;
	if (!(Tester = calloc(1,sizeof(Near))))
		perror("calloc");
	//memset(Tester,0,sizeof(Near));

	Player *TestList = NULL;
	for (i = 0; i < 10; i++) {
		newPlayer(&TestList);
	}

	clearListPlayer(&TestList);

	Tester->pParticle = TestList;
	free(Tester);
	return 0;
}*/
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
* Author:
*   Jussi Laakkonen
*   1234567
*   jussi.laakkonen@lut.fi

TAA ON HYVÄ POHJA LÄHTEE LIIKKEELLE
*/

#include "server_unpackers.h"
#include "server_functions.h"
#define SIZE 256



int server(char* port) {
  int socketfd = -1, activity;
	fd_set readset;
	struct timeval tvSelect, tvUpdate1, tvUpdate2;
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

  socklen_t addrlen = 0;
  unsigned int optval = 0;
  int dgramlen = 0;

  printf("| - - - - - S E R V E R - - - - - |\n");

  if (atoi(port) >= 1024 && atoi(port) <= 65000) {
    // Get my (AI_PASSIVE) addresses that are suitable for bind
    if(getaddrinfo(NULL,port,&hints,&result)) {
      perror("cannot get addresses for server");
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

    freeaddrinfo(result);

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

		while (1) {

			// Refresh select() set
			FD_ZERO(&readset);
			FD_SET(socketfd, &readset);

			tvSelect.tv_sec = 0;
			tvSelect.tv_usec = 1000000;

			// Start waiting for socket activity
			activity = select(socketfd+1, &readset, NULL, NULL, &tvSelect);

			// UPD activity
			if (FD_ISSET(socketfd, &readset)) {
					/* Try to receive something (expecting a char - length = 1 byte).. */
					dgramlen = recvfrom(socketfd,&recvbuffer,SIZE,0,client_address,&addrlen);
					struct Packet packet;
					packet = unpackPacket(recvbuffer, client_address);

					switch (packet.msgType) {

						case GAME_MESSAGE:
							printf("Game message packet received!\n");
							break;

						case ACK:
							printf("Ack packet received!\n");
							break;

						case PLAYER_MOVEMENT:
							printf("Player movement packet received!\n");
							break;

						case STATISTICS_MESSAGE:
							printf("Player movement packet received!\n");
							break;

						default:
							printf("Invalid packet received!\n");

					}

					printf("PacketID: %d\n", packet.ID);
					printf("msgType: %d\n", packet.msgType);

					/* TASK: Get the sender address and port and print it*/
					memset(&hostbuffer,0,NI_MAXHOST);
					memset(&portbuffer,0,NI_MAXSERV);

					printf("Server: Got %d bytes from %s:%s\n",dgramlen,hostbuffer,portbuffer);
			}

			// TCP activity
			else if (0) {

			}

			if ((time2 - time1) >= 5000) {
				printf("Game update!\n");
				gettimeofday(&tvUpdate1, NULL);
				time1 = tvUpdate1.tv_sec * 1000 + tvUpdate1.tv_usec / 1000;
			}
			gettimeofday(&tvUpdate2, NULL);
			time2 = tvUpdate2.tv_sec * 1000 + tvUpdate2.tv_usec / 1000;


			// Run game update if enough time passed since last update


	}

/* Question:
* Since we know the address and port client is using, it should be easy to send something
* back. Think about the operation and reverse it without initializing new address.
* What can be changed in sendto()-function parameters if message is going to be sent to
* someone else?
*/

    close(socketfd); /* REMEMBER ME! */
  }
  else {
    printf("Server: Invalid port. Choose something between 1024 - 65000\n");
    return -1;
  }

  return 0;
}

int client(char* port, char *serverip)
{
  int socketfd = -1, length = 0, rval = 0;
  char dgram[SIZE];

  struct addrinfo hints = { .ai_flags = AI_NUMERICHOST|AI_NUMERICSERV,
                            .ai_family = PF_UNSPEC,
                            .ai_socktype = SOCK_DGRAM,
                            .ai_protocol = IPPROTO_UDP};

  struct addrinfo *result = NULL, *iter = NULL;

  memset(&dgram,1,SIZE);

  printf("| - - - - - C L I E N T - - - - - |\n");

  /* Why only IP address works ? */
  if(getaddrinfo(serverip,port,&hints,&result) < 0) perror("Cannot resolve address");
  else {
    // Go through every returned address and attempt to connect to each
    for (iter = result; iter != NULL; iter = iter->ai_next) {

      /* Can socket be created? */
      if ((socketfd = socket(iter->ai_family, iter->ai_socktype,iter->ai_protocol)) < 0) {
        perror("socket()");
        rval = -1;
        break;
      }

      /* Try to send data to server:
      * sendto(socket, data , data length, flags, destination, struct length)
      * see 'man sendto'
      */
			/* Dummy Join Test */

      int index = 0;
      uint16_t uid = 0;
      *(uint16_t*)&dgram[index] = htons(uid);
      index += sizeof(uint16_t);
      uint32_t gametime = 23;
      uint8_t msgtype = GAME_MESSAGE;
      *(uint32_t*)&dgram[index] = htonl(gametime);
      index += sizeof(uint32_t);
      *(uint8_t*)&dgram[index] = msgtype;
			index += sizeof(uint8_t);
			*(uint32_t*)&dgram[index] = htonl(1);
			index += sizeof(uint32_t);
      uint8_t subt = JOIN;
			*(uint8_t*)&dgram[index] = subt;


      if((length = sendto(socketfd,&dgram,SIZE,0,iter->ai_addr,iter->ai_addrlen)) < 0) {
        perror("sendto()");
        rval = -1;
        break;
      }
      else printf("Client: Sent datagram length = %d\n", length);

    }
  }

  freeaddrinfo(result);

  close(socketfd); /* REMEMBER ME! */

  return rval;
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
    if (client(argv[1],argv[2]) == 0) printf("Client: exited with success\n");
    else
    {
      printf("Errors with client\n");
      return -1;
    }
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
