#include <sys/select.h>

#include "Inet/InetConnection.hpp"
#include "Inet/Messages.hpp"
#include "Inet/MessagesAck.hpp"

#define PORT "3393"
#define UDP_BUFMAX "1500"

InetConnection::InetConnection() {
	//result = static_cast<addrinfo*>(malloc(sizeof(addrinfo)));
	/*hints = {
	 AI_NUMERICHOST | AI_NUMERICSERV,  // addrinfo::ai_flags
	 PF_UNSPEC,// addrinfo::ai_family
	 SOCK_STREAM,// addrinfo::ai_socktype
	 IPPROTO_UDP,// addrinfo::ai_protocol
	 0, 0, nullptr, nullptr// unused
	 };
	 */
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	iter = nullptr;
}

std::vector<Message*> messages;

void InetConnection::init() {

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	std::string l_port = "8889";
	if (getaddrinfo(NULL, l_port.c_str(), &hints, &res)) {
		std::cout << "Cannot resolve address. Exiting" << std::endl;
		exit (EXIT_FAILURE);
	} else {
		std::cout << "getaddrinfo success" << std::endl;
// Go through every returned address and attempt to connect to each
		for (iter = res; iter != NULL; iter = iter->ai_next) {
			if ((listensocket = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol)) < 0) {
				std::cout << "Error socket(): " << strerror(errno) << std::endl;
				exit (EXIT_FAILURE);
				break;
			}
			if (bind(listensocket, iter->ai_addr, iter->ai_addrlen) < 0) {
				close(listensocket);
				std::cout << "Error bind(): " << strerror(errno) << std::endl;
				break;
			}
			break;
		}
	}
//	std::cout << "Listening socket" << listensocket << std::endl;

}

void InetConnection::destroy(void) {
	// delete messages behind pointers
	for (auto& it : messages) {
		delete it;
	}
	// empty whole vector
	messages.clear();
}
bool InetConnection::send(uint8_t * message, int size) {
	/* Try to send data to server:
	 * sendto(socket, data , data length, flags, destination, struct length)
	 * see 'man sendto'
	 */

	struct addrinfo * server_addrinfo = nullptr;
	string l_ip = "10.0.2.15";
	string l_port = "8888";
	if (getaddrinfo(l_ip.c_str(), l_port.c_str(), &hints, &server_addrinfo) < 0) {
		std::cout << "Cannot resolve address: " << strerror(errno) << std::endl;
		return false;
	}

	/* send to asdfasdf	 */
	if ((sendto(listensocket, message, size, 0, server_addrinfo->ai_addr, server_addrinfo->ai_addrlen)) < 0) {
		std::cout << "Error sentto(): " << strerror(errno) << std::endl;
		return false;
	} else {
//		std::cout << "Client: Sent datagram" << std::endl;
	}
	return true;

}
// http://stackoverflow.com/questions/17769964/linux-sockets-non-blocking-connect
/**
 * connect
 * this function tries to connect to given server crediterials.
 * it makes ConnectionState CONNECTING and then tries to send message to connect
 * @params: string ip, string port. Port and IP of the server we are connecting.
 * @return: bool success. returns success if there was no socket error
 */
bool InetConnection::connectTCP(const std::string& l_ip, const std::string& l_port) {
	if (sockettcp != 0)
		close(sockettcp);
	if (m_state != ConnectionState::CONNECTING) {
		// get server address information
		ip = l_ip;
		port = l_port;
		if (getaddrinfo(ip.c_str(), port.c_str(), &hints, &res) != 0) {
			cerr << "getaddrinfo error " << strerror(errno) << endl;
			strerrno = strerror(errno);
			return false;
		}
		// create socket
		if ((sockettcp = socket(res->ai_family, res->ai_socktype | SOCK_NONBLOCK, res->ai_protocol)) < 0) {
			cerr << "cannot create socket for tcp connection " << strerror(errno) << endl;
			strerrno = strerror(errno);
			return false;
		}
		int sock_res = ::connect(sockettcp, res->ai_addr, res->ai_addrlen);
		if (sock_res < 0) {
			if (errno == EINPROGRESS) {
				m_state = ConnectionState::CONNECTING;
				cout << "Trying to connect to host" << endl;
				return true;
			} else if (errno == ECONNREFUSED)
				m_state = ConnectionState::REFUSED;
			else
				m_state = ConnectionState::DISCONNECTED;
			cout << strerror(errno) << endl;
			return false;
		}
		if (sock_res == 0) {
			m_state = ConnectionState::CONNECTED;
			cout << "We are now connected to host" << endl;
			return true;
		}
	}
	return false;;
}
bool InetConnection::disconnect() {
	if (res == nullptr)
		freeaddrinfo(res);
	close(sockettcp);
	sockettcp = 0;
	m_state = ConnectionState::DISCONNECTED;
	return true;
}

void InetConnection::update() {
	/*
	// if connecting tcp
//	FD_ZERO(&socket_fds);
//	FD_SET(sockettcp, &socket_fds);
//	if (m_state == ConnectionState::CONNECTING) {
//		int err;
//		socklen_t len = sizeof(err);
//		if (getsockopt(sockettcp, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
//			cerr << "getsockopt error: " << strerror(errno) << endl;
//			strerrno = strerror(errno);
//			cout << strerror(err) << endl;
//			m_state = ConnectionState::REFUSED;
//		} else if (err == EINPROGRESS)
//			m_state = ConnectionState::CONNECTING;
//		else if (err == 0) {
//			timeout.tv_usec = 100;
//			timeout.tv_sec = 0;
//			int ret = 0;
//			if ((ret = select(sockettcp + 1, &socket_fds, NULL, NULL, &timeout)) < 0) {
//				cout << ret << endl;
//			}
//			if (FD_ISSET(sockettcp, &socket_fds)) {
//				char buffer[BUFFER_SIZE];
//				recv(sockettcp, buffer, sizeof(buffer), 0);
//				//cout << buffer << endl;
//				if (strlen(buffer) > 0)
//					m_state = ConnectionState::CONNECTED;
//			}
//		} else {
//			strerrno = strerror(err);
//			return;
//		}
//
//	}
 */

	memset(&timeout, 0, sizeof(timeout));
	biggestsocket = 0;
	timeout.tv_usec = 500000; // microseconds
	timeout.tv_sec = 0; // seconds
	FD_ZERO(&socket_fds); // Clear the set of file descriptors

	// Add listening socket to the set and check if it is the biggest socket number
	FD_SET(listensocket, &socket_fds);
	if (listensocket > biggestsocket) {
		// std::cout << "We have a descriptor: " << listensocket << std::endl;
		biggestsocket = listensocket;
	}
	Message * unpackedMessage;
	switch (select(biggestsocket + 1, &socket_fds, NULL, NULL, &timeout)) {
	case (-1):
		std::cout << "Error -1" << std::endl;
		break;
	case (0):
		std::cout << "Timeout" << std::endl;
		break;
	default:
//		std::cout << "Inet update" << std::endl;
		struct MessageHeader *header = static_cast<struct MessageHeader*>(malloc(sizeof(struct MessageHeader)));

		uint8_t payloadBuffer[BUFFER_SIZE];

		for (int socket_fd = 0; socket_fd <= biggestsocket; socket_fd++) {
			if (FD_ISSET(socket_fd, &socket_fds)) {
				Message::UnpackHeader(socket_fd, header, payloadBuffer);
				unpackedMessage = MessageFactory::getInstance().getMessageByType(header, payloadBuffer);
			}
		}
		break;
	}

	switch (unpackedMessage->getMessageType()) {
	case MESSAGE_TYPE::GAME_MESSAGE:
		switch (dynamic_cast<GameMessage*>(unpackedMessage)->getGameMessageType()) {
		case GAME_MESSAGE_TYPE::JOIN:
			std::cout << "============== GOT JOIN ==============" << std::endl;
			m_messages.push_back(dynamic_cast<Join*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::NICK:
			std::cout << "============== GOT NICK ==============" << std::endl;
			m_messages.push_back(dynamic_cast<Nick*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::GAME_UPDATE:
			std::cout << "============== GOT GAME_UPDATE ==============" << std::endl;
			m_messages.push_back(dynamic_cast<GameUpdate*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::EXIT:
			std::cout << "============== GOT EXIT ==============" << std::endl;
			m_messages.push_back(dynamic_cast<Exit*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::RESTART:
			std::cout << "============== GOT RESTART ==============" << std::endl;
			m_messages.push_back(dynamic_cast<Restart*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::GAME_END:
			std::cout << "============== GOT GAME_END ==============" << std::endl;
			m_messages.push_back(dynamic_cast<GameEnd*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::POINTS:
			std::cout << "============== GOT POINTS ==============" << std::endl;
			m_messages.push_back(dynamic_cast<Points*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::PLAYER_DEAD:
			std::cout << "============== GOT PLAYER_DEAD ==============" << std::endl;
			m_messages.push_back(dynamic_cast<Points*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::PLAYER_OUT:
			std::cout << "============== GOT PLAYER_OUT ==============" << std::endl;
			m_messages.push_back(dynamic_cast<Points*>(unpackedMessage));
			break;
		}
		break;
	case MESSAGE_TYPE::PLAYER_CHAT_MESSAGE:
		break;
	case MESSAGE_TYPE::PLAYER_MOVEMENT:
		break;
	case MESSAGE_TYPE::ACK:
		switch (dynamic_cast<MessagesAck*>(unpackedMessage)->getGameMessageType()) {
		case GAME_MESSAGE_TYPE::JOIN:
			std::cout << "============== GOT JOIN ACK ==============" << std::endl;
			m_messages.push_back(dynamic_cast<JoinAck*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::NICK:
			std::cout << "==============  GOT NICK ACK ==============" << std::endl;
			m_messages.push_back(dynamic_cast<NickAck*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::GAME_UPDATE:
			std::cout << "============== GOT GAME_UPDATE ACK ==============" << std::endl;
			m_messages.push_back(dynamic_cast<GameUpdate*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::EXIT:
			std::cout << "============== GOT EXIT ACK ==============" << std::endl;
			m_messages.push_back(dynamic_cast<ExitAck*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::RESTART:
			std::cout << "============== GOT RESTART ACK ==============" << std::endl;
			m_messages.push_back(dynamic_cast<RestartAck*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::GAME_END:
			std::cout << "============== GOT GAME_END ACK ==============" << std::endl;
			m_messages.push_back(dynamic_cast<GameEndAck*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::POINTS:
			std::cout << "============== GOT POINTS ACK ==============" << std::endl;
			m_messages.push_back(dynamic_cast<Points*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::PLAYER_DEAD:
			std::cout << "============== GOT PLAYER_DEAD ACK ==============" << std::endl;
			m_messages.push_back(dynamic_cast<PlayerDeadAck*>(unpackedMessage));
			break;
		case GAME_MESSAGE_TYPE::PLAYER_OUT:
			std::cout << "============== GOT PLAYER_OUT ACK ==============" << std::endl;
			m_messages.push_back(dynamic_cast<PlayerOutAck*>(unpackedMessage));
			break;
		}
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

//	for (auto& it : messages) {
//		it->Update();
//	}
	return;
}

std::vector<MessagesAck*> InetConnection::getAcks() {
//	 std::cout << "GETTING ACK MESSAGES"  << std::endl;

	std::vector<MessagesAck*> lmessages;
	for (unsigned int it = 0; it < m_messages.size(); it++) {
		if (m_messages[it]->getMessageType() == MESSAGE_TYPE::ACK) {
			lmessages.push_back(static_cast<MessagesAck*>(m_messages[it]));
			m_messages[it] = m_messages.back();
			m_messages.pop_back();
		}
	}
	return lmessages;
}

vector<ChatMessage*> InetConnection::getChatMessages() {
	vector<ChatMessage*> lmessages;
	for (unsigned int it = 0; it < m_messages.size(); it++) {
		if (m_messages[it]->getMessageType() == MESSAGE_TYPE::PLAYER_CHAT_MESSAGE) {
			lmessages.push_back(static_cast<ChatMessage*>(m_messages[it]));
			m_messages[it] = m_messages.back();
			m_messages.pop_back();
		}
	}
	return lmessages;
}

vector<PlayerDead*> InetConnection::getDeadPayers() {
	vector<PlayerDead*> lmessages;
	for (unsigned int it = 0; it < m_messages.size(); it++) {
		if (m_messages[it]->getMessageType() == MESSAGE_TYPE::GAME_MESSAGE) {
			if (dynamic_cast<GameMessage*>(m_messages[it])->getGameMessageType() == GAME_MESSAGE_TYPE::PLAYER_DEAD) {
				lmessages.push_back(static_cast<PlayerDead*>(m_messages[it]));
				m_messages[it] = m_messages.back();
				m_messages.pop_back();
			}
		}
	}
	return lmessages;
}

