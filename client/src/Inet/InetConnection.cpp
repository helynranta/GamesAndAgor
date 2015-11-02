#include <sys/select.h>

#include "Inet/InetConnection.hpp"

#define PORT "3393"
#define UDP_BUFMAX "1500"

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
			if ((listensocket = socket(iter->ai_family, iter->ai_socktype,
					iter->ai_protocol)) < 0) {
				std::cout << "Error socket()" << std::endl;
				exit (EXIT_FAILURE);
				break;
			}
			if (bind(listensocket, iter->ai_addr, iter->ai_addrlen) < 0) {
				close(listensocket); /* Even when bind fails, socket remains, close it */
				perror("bind()");
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
bool InetConnection::send(void) {
	bool success = false;

	/* Try to send data to server:
	 * sendto(socket, data , data length, flags, destination, struct length)
	 * see 'man sendto'
	 */
	if ((length = sendto(listensocket, &dgram, 1, 0, iter->ai_addr,
			iter->ai_addrlen)) < 0) {
		std::cout << "error sendto()" << std::endl;
	} else {
		std::cout << "Client: Sent datagram legnth = " << length << std::endl;
		success = true;
	}
	return success;
}
/**
 * connect
 * this function tries to connect to given server crediterials.
 * it makes ConnectionState CONNECTING and then tries to send message to connect
 * @params: string ip, string port. Port and IP of the server we are connecting.
 * @return: bool success. returns success if there was no socket error
 */
bool InetConnection::connect(std::string l_ip, std::string l_port) {
	m_state = ConnectionState::CONNECTING;
	/*
	 send to asdfasdf
	 */
	return true;
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
		struct message_header *header =
				static_cast<struct message_header*>(malloc(
						sizeof(struct message_header)));

		for (int socket_fd = 0; socket_fd <= biggestsocket; socket_fd++) {
			if (FD_ISSET(socket_fd, &socket_fds)) {
				std::cout << "Unpacking" << std::endl;
				//Message::unpack_header(socket_fd, header);
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
		it->update();
	}
}
