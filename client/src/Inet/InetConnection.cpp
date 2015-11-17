#include <sys/select.h>

#include "Inet/InetConnection.hpp"

#define PORT "3393"
#define UDP_BUFMAX "1500"

ConnectionState InetConnection::m_state = ConnectionState::DISCONNECTED;

struct addrinfo InetConnection::hints = {
AI_NUMERICHOST | AI_NUMERICSERV,  // addrinfo::ai_flags
		PF_UNSPEC,                      // addrinfo::ai_family
		SOCK_DGRAM,                     // addrinfo::ai_socktype
		IPPROTO_UDP,                    // addrinfo::ai_protocol
		0, 0, nullptr, nullptr             // unused
		};
struct addrinfo *InetConnection::result = nullptr;
struct addrinfo *InetConnection::iter = nullptr;
struct timeval InetConnection::timeout;

int InetConnection::length = 0;
int InetConnection::rval = 0;
int InetConnection::listensocket = -1;
int InetConnection::biggestsocket = 0;
char InetConnection::dgram[1];

// fd_set does not need to be initialized as it need to be emptied ALWAYS before use
fd_set InetConnection::socket_fds;
fd_set InetConnection::socket_fds_temp;
std::string InetConnection::ip = "";
std::string InetConnection::port = "";

std::vector<Message*> InetConnection::messages;

void InetConnection::init(void) {
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, PORT, &hints, &result)) {
		std::cout << "Cannot resolve address. Exiting" << std::endl;
		exit (EXIT_FAILURE);
	} else {
		std::cout << "getaddrinfo success" << std::endl;
		// Go through every returned address and attempt to connect to each
		for (iter = result; iter != NULL; iter = iter->ai_next) {
			/* Can socket be created? */
			if ((listensocket = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol)) < 0) {
				std::cout << "Error socket(): " << strerror(errno) << std::endl;
				exit (EXIT_FAILURE);
				break;
			}
			if (bind(listensocket, iter->ai_addr, iter->ai_addrlen) < 0) {
				close(listensocket); /* Even when bind fails, socket remains, close it */
				std::cout << "Error bind(): " << strerror(errno) << std::endl;
				break;
			}
			break;
		}
	}
	std::cout << "Listening socket" << listensocket << std::endl;
}

/** deconstructor
 * frees all used memory and empties messages vector
 * @params: void
 *

 */
void InetConnection::destroy(void) {
	delete result;
	delete iter;
	// delete messages behind pointers
	for (auto& it : messages) {
		delete it;
	}
	// empty whole vector
	messages.empty();
}
/** send
 * this function sends socket messages
 * @params: void
 * @return: bool success. returns success if there was no socket error
 */
bool InetConnection::send(std::string l_ip, std::string l_port, std::string message) {
	/* Try to send data to server:
	 * sendto(socket, data , data length, flags, destination, struct length)
	 * see 'man sendto'
	 */

	struct addrinfo * server_addrinfo = nullptr;
	if (getaddrinfo(l_ip.c_str(), l_port.c_str(), &hints, &server_addrinfo) < 0){
		std::cout << "Cannot resolve address: " << strerror(errno) << std::endl;
		return false;
	}


	/* send to asdfasdf	 */
	if ((sendto(listensocket, message.c_str(), BUFFER_SIZE, 0, server_addrinfo->ai_addr, server_addrinfo->ai_addrlen)) < 0) {
		std::cout << "Error sentto(): " << strerror(errno) << std::endl;
		return false;
	}else {
		std::cout << "Client: Sent datagram" << std::endl;
	}
	return true;

}
/**
 * connect
 * this function tries to connect to given server crediterials.
 * it makes ConnectionState CONNECTING and then tries to send message to connect
 * @params: string ip, string port. Port and IP of the server we are connecting.
 * @return: bool success. returns success if there was no socket error
 */
bool InetConnection::connect(std::string l_ip, std::string l_port) {
	return false;
}
bool InetConnection::disconnect() {
	if (result == nullptr)
		freeaddrinfo(result);
//close(socketfd);
	return true;
}

void InetConnection::update() {
	memset(&timeout, 0, sizeof(timeout));
	biggestsocket = 0;
	timeout.tv_usec = 1000; // microseconds
	timeout.tv_sec = 2; // seconds
	FD_ZERO(&socket_fds); // Clear the set of file descriptors

// Add listening socket to the set and check if it is the biggest socket number
	FD_SET(listensocket, &socket_fds);
	if (listensocket > biggestsocket) {
		std::cout << "We have a descriptor: " << listensocket << std::endl;
		biggestsocket = listensocket;
	}

	switch (select(biggestsocket + 1, &socket_fds, NULL, NULL, &timeout)) {
	case (-1):
		std::cout << "Error -1" << std::endl;
		break;
	case (0):
		std::cout << "Timeout" << std::endl;
		break;
	default:
		std::cout << "Inet update" << std::endl;
		struct message_header *header = static_cast<struct message_header*>(malloc(sizeof(struct message_header)));
		uint8_t payloadBuffer[BUFFER_SIZE];

		for (int socket_fd = 0; socket_fd <= biggestsocket; socket_fd++) {
			if (FD_ISSET(socket_fd, &socket_fds)) {
				std::cout << "Unpacking" << std::endl;
				Message::UnpackHeader(socket_fd, header, payloadBuffer);
				MessageFactory::getInstance().getMessageByType(header, payloadBuffer);
			}
		}
		break;
	}

	/*
	 switch msg{
	 case JOIN_ACK:
	 messages.push_back(new NickACK());
	 case WAIT:
	 messages.push_back(new WAIT());
	 default:
	 std::cout << "Undefined message error"  << std::endl;
	 }
	 */
//if(m_state == ConnectionState::CONNECTING)
//
	for (auto& it : messages) {
		it->Update();
	}
}
