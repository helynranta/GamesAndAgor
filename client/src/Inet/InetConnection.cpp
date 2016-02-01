#include <sys/select.h>

#include "Inet/InetConnection.hpp"

#define PORT "3393"
#define UDP_BUFMAX "1500"

InetConnection::InetConnection() {
	result = static_cast<addrinfo*>(malloc(sizeof(addrinfo)));
	hints = {
		AI_NUMERICHOST | AI_NUMERICSERV,  // addrinfo::ai_flags
		PF_UNSPEC,// addrinfo::ai_family
		SOCK_DGRAM,// addrinfo::ai_socktype
		IPPROTO_UDP,// addrinfo::ai_protocol
		0, 0, nullptr, nullptr// unused
	};

	iter = nullptr;

}

std::vector<Message*> messages;

void InetConnection::init(std::string l_ip, std::string l_port) {
	m_state = ConnectionState::CONNECTING;
	serverIP = l_ip;
	serverPort = l_port;
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
			if ((udpListensocket = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol)) < 0) {
				std::cout << "Error socket(): " << strerror(errno) << std::endl;
				exit (EXIT_FAILURE);
				break;
			}
			if (bind(udpListensocket, iter->ai_addr, iter->ai_addrlen) < 0) {
				close(udpListensocket); /* Even when bind fails, socket remains, close it */
				std::cout << "Error bind(): " << strerror(errno) << std::endl;
				break;
			}
			break;
		}
	}
	m_state = ConnectionState::CONNECTED;
	std::cout << "Listening socket" << udpListensocket << std::endl;
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
//bool InetConnection::send(std::string l_ip, std::string l_port, std::string message) {
bool InetConnection::send(uint8_t * message, int size) {
	/* Try to send data to server:
	 * sendto(socket, data , data length, flags, destination, struct length)
	 * see 'man sendto'
	 */

	struct addrinfo * server_addrinfo = nullptr;
	if (getaddrinfo(serverIP.c_str(), serverPort.c_str(), &hints, &server_addrinfo) < 0) {
		std::cout << "Cannot resolve address: " << strerror(errno) << std::endl;
		return false;
	}

	/* send to server	 */
	if ((sendto(udpListensocket, message, size, 0, server_addrinfo->ai_addr, server_addrinfo->ai_addrlen)) < 0) {
		std::cout << "Error sentto(): " << strerror(errno) << std::endl;
		return false;
	} else {
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
bool InetConnection::startConnection(std::string l_ip, std::string l_port) {
	// CREATE TCP CONNECTION
	std::cout << "Connecting TCP to server at " << l_ip << ":" << l_port << std::endl;
	tcp_socket_fd = createTCPSocket(l_ip, l_port);
	if (tcp_socket_fd == -1) {
		std::cout << "Error creating tcp stream" << std::endl;
		exit (EXIT_FAILURE);
	}

	return true;
}

int InetConnection::createTCPSocket(std::string serverip, std::string port) {
	int tcp_socket_fd = -1, yes = 1;

	struct addrinfo hints = {
	AI_NUMERICHOST | AI_NUMERICSERV,  // addrinfo::ai_flags
			AF_UNSPEC,  // addrinfo::ai_family
			SOCK_STREAM,  // addrinfo::ai_socktype
			0, 0, nullptr, nullptr  // unused
			};

	struct addrinfo *result = NULL, *iter = NULL, *client_tcp_addrinfo = NULL;

	if (getaddrinfo(serverip.c_str(), port.c_str(), &hints, &result)) {
		std::cout << "Cannot resolve address. Exiting" << std::endl;
		exit (EXIT_FAILURE);
	} else {
		for (iter = result; iter != NULL; iter = iter->ai_next) {

			if ((tcp_socket_fd = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol)) < 0) {
				perror("socket()");
				break;
			}

			if (setsockopt(tcp_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
				perror("sockopts()");
				break;
			}

			client_tcp_addrinfo = iter;
			break;
		}
	}
	if (connect(tcp_socket_fd, client_tcp_addrinfo->ai_addr, client_tcp_addrinfo->ai_addrlen) < 0) {
		//if (connect(tcp_socket_fd, (const sockaddr*) &client_tcp_addrinfo->ai_addr, sizeof(client_tcp_addrinfo)) < 0) {
		perror("connect()");
		return -1;
	}

	freeaddrinfo(result);
	return tcp_socket_fd;
}

bool InetConnection::disconnect() {
	if (result == nullptr)
		freeaddrinfo(result);
	//close(socketfd);
	return true;
}

void InetConnection::update() {
	memset(&timeout, 0, sizeof(timeout));
	biggestSocket = 0;
	timeout.tv_usec = 3000; // microseconds
	timeout.tv_sec = 0; // seconds
	FD_ZERO(&socket_fds); // Clear the set of file descriptors

	// Add listening socket to the set and check if it is the biggest socket number
	FD_SET(udpListensocket, &socket_fds);
	if (udpListensocket > biggestSocket) {
//		std::cout << "We have a descriptor: " << udpListensocket << std::endl;
		biggestSocket = udpListensocket;
	}

	Message * unpackedMessage;
	switch (select(biggestSocket + 1, &socket_fds, NULL, NULL, &timeout)) {
	case (-1):
		std::cout << "Error -1" << std::endl;
		break;
	case (0):
		std::cout << "Timeout" << std::endl;
		break;
	default:
		std::cout << "Inet update" << std::endl;
		struct MessageHeader *header = static_cast<struct MessageHeader*>(malloc(sizeof(struct MessageHeader)));

		uint8_t payloadBuffer[BUFFER_SIZE];

		for (int socket_fd = 0; socket_fd <= biggestSocket; socket_fd++) {
			if (FD_ISSET(socket_fd, &socket_fds)) {
//				std::cout << "Unpacking" << std::endl;
				Message::UnpackHeader(socket_fd, header, payloadBuffer);
				unpackedMessage = MessageFactory::getInstance().getMessageByType(header, payloadBuffer);
			}
		}
		break;
	}

	int payloadLength = 0;
	uint8_t senderPayload[BUFFER_SIZE];
	memset(senderPayload, 0, sizeof(uint8_t[1500]));
	switch (unpackedMessage->getMessageType()) {
	case MESSAGE_TYPE::GAME_MESSAGE:
		switch (static_cast<const GameMessage*>(unpackedMessage)->getGameMessageType()) {
		case GAME_MESSAGE_TYPE::JOIN:
			messages.push_back(dynamic_cast<Join*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::NICK:
			payloadLength = dynamic_cast<Nick*>(unpackedMessage)->Ack(senderPayload);
			break;
		case GAME_MESSAGE_TYPE::EXIT:
			payloadLength = dynamic_cast<Exit*>(unpackedMessage)->Ack(senderPayload);
			break;
		case GAME_MESSAGE_TYPE::RESTART:
			payloadLength = dynamic_cast<Restart*>(unpackedMessage)->Ack(senderPayload);
			break;
		case GAME_MESSAGE_TYPE::GAME_END:
			payloadLength = dynamic_cast<GameEnd*>(unpackedMessage)->Ack(senderPayload);
			break;
		case GAME_MESSAGE_TYPE::GAME_UPDATE:
			payloadLength = dynamic_cast<GameUpdate*>(unpackedMessage)->Ack(senderPayload);
			break;
		case GAME_MESSAGE_TYPE::POINTS:
			payloadLength = dynamic_cast<Points*>(unpackedMessage)->Ack(senderPayload);
			break;
		case GAME_MESSAGE_TYPE::PLAYER_DEAD:
			payloadLength = dynamic_cast<PlayerDead*>(unpackedMessage)->Ack(senderPayload);
			break;
		case GAME_MESSAGE_TYPE::PLAYER_OUT:
			payloadLength = dynamic_cast<PlayerOut*>(unpackedMessage)->Ack(senderPayload);
			break;
		}
		break;
	case MESSAGE_TYPE::PLAYER_CHAT_MESSAGE:
		break;
	case MESSAGE_TYPE::PLAYER_MOVEMENT:
		break;
	case MESSAGE_TYPE::ACK:
		break;
	case MESSAGE_TYPE::STATISTICS_MESSAGE:
		break;
	default:
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
std::vector<ChatMessage*> InetConnection::getChatMessages() {
	std::vector<ChatMessage*> messages;
	for (uint32_t i = 0; i < m_messages.size(); i++) {
		if (m_messages[i]->getMessageType() == MESSAGE_TYPE::PLAYER_CHAT_MESSAGE) {
			messages.push_back(static_cast<ChatMessage*>(m_messages[i]));
			m_messages[i] = messages.back();
			m_messages.pop_back();
		}
	}
	return messages;
}

std::vector<Join*> InetConnection::getJoinMessages() {
	std::vector<Join*> messages;
	for (uint32_t i = 0; i < m_messages.size(); i++) {
		if (m_messages[i]->getMessageType() == MESSAGE_TYPE::GAME_MESSAGE) {
			if (dynamic_cast<GameMessage*>(m_messages[i])->getGameMessageType() == GAME_MESSAGE_TYPE::JOIN) {
				messages.push_back(dynamic_cast<Join*>(m_messages[i]));
				m_messages[i] = messages.back();
				m_messages.pop_back();
			}
		}
	}
	return messages;
}

std::vector<Join*> InetConnection::getJoinAckMessages() {
	std::vector<Join*> messages;
	for (uint32_t i = 0; i < m_messages.size(); i++) {
		if (m_messages[i]->getMessageType() == MESSAGE_TYPE::ACK) {
			if (dynamic_cast<GameMessage*>(m_messages[i])->getGameMessageType() == GAME_MESSAGE_TYPE::JOIN) {
				messages.push_back(dynamic_cast<Join*>(m_messages[i]));
				m_messages[i] = messages.back();
				m_messages.pop_back();
			}
		}
	}
	return messages;
}

std::vector<PlayerDead*> InetConnection::getDeadPayers() {
	std::vector<PlayerDead*> messages;
	for (auto& message : m_messages) {
		if (message->getMessageType() == MESSAGE_TYPE::GAME_MESSAGE) {

			if (dynamic_cast<GameMessage*>(message)->getGameMessageType() == GAME_MESSAGE_TYPE::PLAYER_DEAD) {
				messages.push_back(dynamic_cast<PlayerDead*>(message));
				//delete m_messages[it];
				//m_messages[it] = m_messages.back();
				//m_messages.pop_back();
			}
		}
	}
	return messages;
}

