#include <sys/select.h>

#include "Inet/InetConnection.hpp"
#include "Engine.hpp"
#include "Inet/Messages.hpp"
#include "Inet/MessagesAck.hpp"

#define PORT "3393"
#define UDP_BUFMAX "1500"

vector<Message*> messages;

InetConnection::InetConnection() {
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
}

void InetConnection::init(void) {
}
void InetConnection::destroy(void) {
	close(socketudp);
	// delete messages behind pointers
	for (auto& it : messages) {
		delete it;
	}
	// empty whole vector
	messages.clear();
}

bool InetConnection::sendChatMessage(const string& message) {
	// these are all going threw tcp
	if ((::send(sockettcp, message.c_str(), sizeof(message.c_str()), 0)) < 0) {
		cerr << "TCP message send failed!" << endl << strerror(errno) << endl;
		return false;
	}
	return true;
}

bool InetConnection::send(uint8_t * message, int size) {
	/* Try to send data to server:
	 * sendto(socket, data , data length, flags, destination, struct length)
	 * see 'man sendto'
	 */

	 struct addrinfo * server_addrinfo = nullptr;
	 if (getaddrinfo(ip.c_str(), portUDP.c_str(), &hints, &server_addrinfo) < 0) {
	 	std::cout << "Cannot resolve address: " << strerror(errno) << std::endl;
	 	return false;
	 }

	/* send to asdfasdf	 */
	 if ((sendto(socketudp, message, size, 0, server_addrinfo->ai_addr, server_addrinfo->ai_addrlen)) < 0) {
	 	std::cout << "Error sentto(): " << strerror(errno) << std::endl;
	 	return false;
	 } else {
//		std::cout << "Client: Sent datagram" << std::endl;
	 }
	 return true;
}	

void InetConnection::sendUDP(GAME_MESSAGE_TYPE type, const string& message) {
// only adds outgoing message to list. put actual send to update

//Message* msg = new GameMessage();
//m_outgoing.insert({int(SDL_GetTicks()), msg});
}
void InetConnection::sendTCP(const string& message) {
	//::sendto(sockettcp, message.c_str(), sizeof(message.c_str()), 0);
}
// http://stackoverflow.com/questions/17769964/linux-sockets-non-blocking-connect
bool InetConnection::connectTCP() {

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (sockettcp != 0) close(sockettcp);
	iter = nullptr;
	if (!tcpsocketstatus) {
		// get server address information
		if (getaddrinfo(ip.c_str(), portTCP.c_str(), &hints, &res) != 0) {
			cerr << "getaddrinfo error " << strerror(errno) << endl;
			strerrno = strerror(errno);
			return false;
		}
		struct addrinfo* p = nullptr;
		for(p = res; p != NULL; p = p->ai_next) {
			// create socket
			if ((sockettcp = socket(p->ai_family, p->ai_socktype | SOCK_NONBLOCK, p->ai_protocol)) < 0) {
				cerr << "cannot create socket for tcp connection " << strerror(errno) << endl;
				strerrno = strerror(errno);
				continue;
			} 
			break;
		}
		// bind to port
		/*
		if (::bind(sockettcp, reinterpret_cast<struct sockaddr*>(&me_addr), sizeof(struct sockaddr)) < 0) {
			cerr << "cannot bind tcp socket to random port" << endl << strerror(errno) << endl;
		}
		*/
		// create unblocking connect process
		int sock_res = ::connect(sockettcp, res->ai_addr, res->ai_addrlen);
		if (sock_res < 0) {
			if (errno == EINPROGRESS) {
				cout << "Trying to connect to host" << endl;
				return true;
			} else if (errno == ECONNREFUSED) m_state = ConnectionState::REFUSED;
			else m_state = ConnectionState::DISCONNECTED;
			cout << strerror(errno) << endl;
			tcpsocketstatus = false;
			return false;
		}
		if (sock_res == 0) {
			tcpsocketstatus = true; 
			cout << "TCP connection established" << endl;
			return true;
		}
	}
	return false;
}
bool InetConnection::connectUDP() {
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	close(socketudp);
	// create udp socket and bind it
	if ((socketudp = ::socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		cerr << "was not able to create udp socket!" << endl << strerror(errno) << endl;
		return false;
	}
	if (::bind(socketudp, reinterpret_cast<struct sockaddr*>(&me_addr), sizeof(me_addr)) < 0) {
		cerr << "was not able to bind udp port!" << endl << strerror(errno) << endl;
		return false;
	}

	if (getaddrinfo(ip.c_str(), portUDP.c_str(), &hints, &res)) {
		std::cout << "Cannot resolve address. Exiting" << std::endl;
		return false;
	} else {
		//std::cout << "getaddrinfo success" << std::endl;
		// Go through every returned address and attempt to connect to each
		for (iter = res; iter != NULL; iter = iter->ai_next) {
			if ((socketudp = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol)) < 0) {
				std::cout << "Error socket(): " << strerror(errno) << std::endl;
				return false;
			}
			/*
			if (bind(socketudp, iter->ai_addr, iter->ai_addrlen) < 0) {
				close(socketudp);
				std::cout << "Error bind(): " << strerror(errno) << std::endl;
				return false;
			}
			*/
			break;
		}
	}
	return true;
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
	checkTCPConnection();
	checkUDPConnections();
	return;
}
int InetConnection::checkTCPConnection() {
// if connecting tcp
	if(m_state == ConnectionState::DISCONNECTED) return 0;

	FD_ZERO(&socket_fds);
	FD_SET(sockettcp, &socket_fds);

	int err;
	socklen_t len = sizeof(err);
	if (getsockopt(sockettcp, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
		cerr << "getsockopt error: " << strerror(errno) << endl;
		cout << strerror(err) << endl;
		m_state = ConnectionState::REFUSED;
		tcpsocketstatus = false;
	}
	else if (err == 0) {
		memset(&timeout, 0, sizeof(timeout));
		timeout.tv_usec = 100;
		timeout.tv_sec = 0;
		switch(select(sockettcp + 1, &socket_fds, NULL, NULL, &timeout)) {
			case -1:
				cerr << strerror(errno) << endl;
			case 0: break;
			default:
				if (FD_ISSET(sockettcp, &socket_fds)) {
					char buffer[BUFFER_SIZE];
					recv(sockettcp, buffer, sizeof(buffer), 0);
					if(strlen(buffer) > 0) {
						tcpsocketstatus = true; 
						cout << buffer << endl; 
					}
				}
				break;
		}	
	} else {
		cerr << strerror(errno) << endl;
		tcpsocketstatus = false;
		return false;
	}
	
	return true;
}

void InetConnection::UnpackGameMessageSubType(Message* unpackedMessage) {
	switch (dynamic_cast<GameMessage*>(unpackedMessage)->getGameMessageType()) {
	case GAME_MESSAGE_TYPE::JOIN:
		//std::cout << "============== GOT JOIN ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<Join*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::NICK:
		//std::cout << "============== GOT NICK ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<Nick*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::GAME_UPDATE:
		//std::cout << "============== GOT GAME_UPDATE ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<GameUpdate*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::EXIT:
		//std::cout << "============== GOT EXIT ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<Exit*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::RESTART:
		//std::cout << "============== GOT RESTART ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<Restart*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::GAME_END:
		//std::cout << "============== GOT GAME_END ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<GameEnd*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::POINTS:
		//std::cout << "============== GOT POINTS ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<Points*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::PLAYER_DEAD:
		//std::cout << "============== GOT PLAYER_DEAD ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<Points*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::PLAYER_OUT:
		//std::cout << "============== GOT PLAYER_OUT ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<Points*>(unpackedMessage));
		break;
	}
}

void InetConnection::UnpackAckMessageSubtype(Message* unpackedMessage) {
	switch (dynamic_cast<MessagesAck*>(unpackedMessage)->getGameMessageType()) {
	case GAME_MESSAGE_TYPE::JOIN:
		//						std::cout << "============== GOT JOIN ACK ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<JoinAck*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::NICK:
		//						std::cout << "==============  GOT NICK ACK ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<NickAck*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::GAME_UPDATE:
		//std::cout << "============== GOT GAME_UPDATE ACK ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<GameUpdate*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::EXIT:
		//std::cout << "============== GOT EXIT ACK ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<ExitAck*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::RESTART:
		//std::cout << "============== GOT RESTART ACK ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<RestartAck*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::GAME_END:
		//std::cout << "============== GOT GAME_END ACK ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<GameEndAck*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::POINTS:
		//std::cout << "============== GOT POINTS ACK ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<Points*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::PLAYER_DEAD:
		//std::cout << "============== GOT PLAYER_DEAD ACK ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<PlayerDeadAck*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::PLAYER_OUT:
		//std::cout << "============== GOT PLAYER_OUT ACK ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<PlayerOutAck*>(unpackedMessage));
		break;
	default:
		break;
	}
}

int InetConnection::checkUDPConnections() {
	memset(&timeout, 0, sizeof(timeout));
	timeout.tv_usec = 100; // microseconds
	timeout.tv_sec = 0; // seconds
	FD_ZERO(&socket_fds); // Clear the set of file descriptors
	// Add listening socket to the set and check if it is the biggest socket number
	FD_SET(socketudp, &socket_fds);
	Message* unpackedMessage = nullptr;
	switch (select(socketudp + 1, &socket_fds, NULL, NULL, &timeout)) {
		case -1:
			std::cout << strerror(errno) << std::endl;
			break;
		case 0:
			break;
		default:
			struct MessageHeader *header = static_cast<struct MessageHeader*>(malloc(sizeof(struct MessageHeader)));
			uint8_t payloadBuffer[BUFFER_SIZE];
			if (FD_ISSET(socketudp, &socket_fds)) {
				Message::UnpackHeader(socketudp, header, payloadBuffer);
				unpackedMessage = MessageFactory::getInstance().getMessageByType(header, payloadBuffer);
			}
			break;
	}
	if(unpackedMessage != nullptr) {
		switch (unpackedMessage->getMessageType()) {
			case MESSAGE_TYPE::GAME_MESSAGE:
				UnpackGameMessageSubType(unpackedMessage);
				break;
			case MESSAGE_TYPE::PLAYER_CHAT_MESSAGE: return false;
			case MESSAGE_TYPE::PLAYER_MOVEMENT: return false;
			case MESSAGE_TYPE::ACK:
				UnpackAckMessageSubtype(unpackedMessage);
				break;
			case MESSAGE_TYPE::STATISTICS_MESSAGE: return false;
			default: return false;
			}
	}
	return true;
}
vector<MessagesAck*> InetConnection::getAcks() {
	vector<MessagesAck*> lmessages;
	for (unsigned int it = 0; it < messageInbox.size(); it++) {
		if (messageInbox[it]->getMessageType() == MESSAGE_TYPE::ACK) {
			lmessages.push_back(static_cast<MessagesAck*>(messageInbox[it]));
			messageInbox[it] = messageInbox.back();
			messageInbox.pop_back();
		}
	}

	return lmessages;
}
MessagesAck* InetConnection::getAck(GAME_MESSAGE_TYPE type) {
	vector<MessagesAck*> msgs;
	for (unsigned int it = 0; it < messageInbox.size(); it++) {
		if (messageInbox[it]->getMessageType() == MESSAGE_TYPE::ACK) {
			MessagesAck* tmp = static_cast<MessagesAck*>(messageInbox[it]);
			if(tmp->getGameMessageType() == type) {
				msgs.push_back(static_cast<MessagesAck*>(messageInbox[it]));
				messageInbox[it] = messageInbox.back();
				messageInbox.pop_back();
			}
		}
	}
	if(msgs.size() == 0) return nullptr;
	return msgs.back();
}

vector<ChatMessage*> InetConnection::getChatMessages() {
	vector<ChatMessage*> lmessages;
	for (unsigned int it = 0; it < messageInbox.size(); it++) {
		if (messageInbox[it]->getMessageType() == MESSAGE_TYPE::PLAYER_CHAT_MESSAGE) {
			lmessages.push_back(static_cast<ChatMessage*>(messageInbox[it]));
			messageInbox[it] = messageInbox.back();
			messageInbox.pop_back();
		}
	}
	return lmessages;
}

vector<PlayerDead*> InetConnection::getDeadPayers() {
	vector<PlayerDead*> lmessages;
	for (unsigned int it = 0; it < messageInbox.size(); it++) {
		if (messageInbox[it]->getMessageType() == MESSAGE_TYPE::GAME_MESSAGE) {
			if (dynamic_cast<GameMessage*>(messageInbox[it])->getGameMessageType() == GAME_MESSAGE_TYPE::PLAYER_DEAD) {
				lmessages.push_back(static_cast<PlayerDead*>(messageInbox[it]));
				messageInbox[it] = messageInbox.back();
				messageInbox.pop_back();
			}
		}
	}
	return lmessages;
}

