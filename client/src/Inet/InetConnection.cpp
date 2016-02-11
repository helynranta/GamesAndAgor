#include <sys/select.h>

#include "Inet/InetConnection.hpp"
#include "Engine.hpp"

#define PORT "3393"
#define UDP_BUFMAX "1500"

vector<Message*> messages;

InetConnection::InetConnection() {}
InetConnection::~InetConnection() {}
void InetConnection::init(void) {
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
}
void InetConnection::destroy(void) {
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
void InetConnection::sendTCP(const string& msg) {
	string m = (string(Engine::getNick() + ": " + msg));
	if(::send(sockettcp, m.c_str(), sizeof(char*)*m.size(), 0)<0) {
		cerr << strerror(errno) << endl;
	}
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
			disconnect();
			return false;
		}
		struct addrinfo* p = nullptr;
		for(p = res; p != NULL; p = p->ai_next) {
			// create socket
			if ((sockettcp = socket(p->ai_family, p->ai_socktype | SOCK_NONBLOCK, p->ai_protocol)) < 0) {
				cerr << "cannot create socket for tcp connection " << strerror(errno) << endl;
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

	if(socketudp != 0) close(socketudp);
	// create udp socket and bind it
	if ((socketudp = ::socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		cerr << "was not able to create udp socket!" << endl << strerror(errno) << endl;
		disconnect();
		return false;
	}
	if (::bind(socketudp, reinterpret_cast<struct sockaddr*>(&me_addr), sizeof(me_addr)) < 0) {
		cerr << "was not able to bind udp port!" << endl << strerror(errno) << endl;
		disconnect();
		return false;
	}

	if (getaddrinfo(ip.c_str(), portUDP.c_str(), &hints, &res)) {
		std::cout << "Cannot resolve address. Exiting" << std::endl;
		disconnect();
		return false;
	} else {
		//std::cout << "getaddrinfo success" << std::endl;
		// Go through every returned address and attempt to connect to each
		for (iter = res; iter != NULL; iter = iter->ai_next) {
			if ((socketudp = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol)) < 0) {
				std::cout << "Error socket(): " << strerror(errno) << std::endl;
				disconnect();
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


	m_state = ConnectionState::DISCONNECTED;
	tcpsocketstatus = false;

	cout << "disconnect has been successfull" << endl;
	return true;
}
int InetConnection::update() {
	int dataProsessed = false;
	checkTCPConnection();
	dataProsessed = checkUDPConnections();
	calculatePing();
	return dataProsessed;
}
//uint InetConnection::lastChecked = 0;
void InetConnection::calculatePing() {
	// calculate ping
	if(pings.size() > 5) pings.erase(pings.begin());
	// send new ping packages
	static uint lastChecked;
	static uint16_t ping_index;
	static uint8_t pingBuffer[BUFFER_SIZE];
	Ping* ping;
	if(lastChecked + 1000 < SDL_GetTicks() && m_state == ConnectionState::CONNECTED) {
		ping = new Ping(createDummyHeader(id, SDL_GetTicks(), MESSAGE_TYPE::STATISTICS_MESSAGE, sizeof(uint32_t)), ++ping_index);
		int messageLength = ping->PackSelf(pingBuffer);
		send(pingBuffer, messageLength);
		lastChecked = SDL_GetTicks();
		delete ping;
	}
}
int InetConnection::checkTCPConnection() {
// if connecting tcp
	char buffer[BUFFER_SIZE];
	memset(buffer, '\0', BUFFER_SIZE);
	if(m_state == ConnectionState::DISCONNECTED) return 0;

	FD_ZERO(&socket_fds);
	FD_SET(sockettcp, &socket_fds);

	int err;
	socklen_t len = sizeof(err);
	if (getsockopt(sockettcp, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
		cerr << "getsockopt error: " << strerror(errno) << endl;
		connectTCP();
		tcpsocketstatus = false;
	}
	else if (err == 0) {
		memset(&timeout, 0, sizeof(timeout));
		timeout.tv_usec = 250;
		timeout.tv_sec = 0;
		int res;
		if((res = select(sockettcp + 1, &socket_fds, NULL, NULL, &timeout))<0) {
			cerr << strerror(errno) << endl;
			tcpsocketstatus = false;
			return false;
		} else {
			if (FD_ISSET(sockettcp, &socket_fds)) {
				recv(sockettcp, buffer, sizeof(buffer), 0);
				if(strlen(buffer) > 0) {
					tcpsocketstatus = true;
					chatmessage.push_back(string(buffer));
				}
			}
		}
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
	switch (dynamic_cast<MessageAck*>(unpackedMessage)->getGameMessageType()) {
	case GAME_MESSAGE_TYPE::JOIN:
		//std::cout << "============== GOT JOIN ACK ==============" << std::endl;
		messageInbox.push_back(dynamic_cast<JoinAck*>(unpackedMessage));
		break;
	case GAME_MESSAGE_TYPE::NICK:
		//std::cout << "==============  GOT NICK ACK ==============" << std::endl;
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
	#ifdef MESG_TEST
		timeout.tv_usec = 250000; // microseconds
	#endif
	timeout.tv_sec = 0; // seconds
	FD_ZERO(&socket_fds); // Clear the set of file descriptors
	// Add listening socket to the set and check if it is the biggest socket number
	FD_SET(socketudp, &socket_fds);
	Message* unpackedMessage = nullptr;
	switch (select(socketudp + 1, &socket_fds, NULL, NULL, &timeout)) {
		case -1:
			std::cout << strerror(errno) << std::endl;
			disconnect();
			return false;
		case 0:
			return false;
		default:
			struct MessageHeader *header = static_cast<struct MessageHeader*>(malloc(sizeof(struct MessageHeader)));
			uint8_t payloadBuffer[BUFFER_SIZE];
			if (FD_ISSET(socketudp, &socket_fds)) {
				Message::UnpackHeader(socketudp, header, payloadBuffer);

				if (header->payload_length < 1){
					if(getMessageTypeAsString(header->message_type) == "STATISTICS_MESSAGE") {
						pings.push_back(SDL_GetTicks() - header->gameTime);
					}
					return false;
				}

				unpackedMessage = MessageFactory::getInstance().getMessageByType(header, payloadBuffer);
				break;
			}
			return false;
	}

	if(unpackedMessage != nullptr) {
		switch (unpackedMessage->getMessageType()) {
			case MESSAGE_TYPE::GAME_MESSAGE:
				UnpackGameMessageSubType(unpackedMessage);
				return true;
			case MESSAGE_TYPE::PLAYER_CHAT_MESSAGE:
				return false;
			case MESSAGE_TYPE::PLAYER_MOVEMENT:
				return false;
			case MESSAGE_TYPE::ACK:
				UnpackAckMessageSubtype(unpackedMessage);
				return true;
			case MESSAGE_TYPE::STATISTICS_MESSAGE:
				return false;
			default:
				return false;
			}
	}
	return false;
}

vector<Message*> InetConnection::getMessagesOfType(MESSAGE_TYPE type) {
	vector<Message*> lmessages;
	for (unsigned int it = 0; it < messageInbox.size(); it++) {
		if (messageInbox[it]->getMessageType() == type) {
			lmessages.push_back(static_cast<GameMessage*>(messageInbox[it]));
			messageInbox[it] = messageInbox.back();
			messageInbox.pop_back();
		}
	}
	return lmessages;
}
vector<Message*> InetConnection::getMessagesOfType(MESSAGE_TYPE type, GAME_MESSAGE_TYPE subtype) {
	vector<Message*> msgs;
	for (unsigned int it = 0; it < messageInbox.size(); it++) {
		if (messageInbox[it]->getMessageType() == type) {
			MessageAck* tmp = static_cast<MessageAck*>(messageInbox[it]);
			if(tmp->getGameMessageType() == subtype) {
				msgs.push_back(static_cast<MessageAck*>(messageInbox[it]));
				messageInbox[it] = messageInbox.back();
				messageInbox.pop_back();
			}
		}
	}
	return msgs;
}
vector<GameUpdate*> InetConnection::getGameUpdateMessages() {
	vector<GameUpdate*> lmessages;
	for (unsigned int it = 0; it < messageInbox.size(); it++) {
		if (messageInbox[it]->getMessageType() == MESSAGE_TYPE::GAME_MESSAGE) {
			if(static_cast<GameMessage*>(messageInbox[it])->getGameMessageType() == GAME_MESSAGE_TYPE::GAME_UPDATE){
				lmessages.push_back(static_cast<GameUpdate*>(messageInbox[it]));
				messageInbox[it] = messageInbox.back();
				messageInbox.pop_back();
			}
		}
	}
	return lmessages;
}

bool InetConnection::getGameEnding(){
	for (unsigned int it = 0; it < messageInbox.size(); it++) {
		if (messageInbox[it]->getMessageType() == MESSAGE_TYPE::GAME_MESSAGE) {
			if(static_cast<GameMessage*>(messageInbox[it])->getGameMessageType() == GAME_MESSAGE_TYPE::GAME_END){
				messageInbox[it] = messageInbox.back();
				messageInbox.pop_back();
				return true;
			}
		}
	}
	return false;
}

vector<string> InetConnection::getChatMessages() {
	vector<string> ret;
	for(auto& it : chatmessage) {
		ret.push_back(it);
	}
	chatmessage.clear();
	return ret;
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

MessageHeader InetConnection::createDummyHeader(uint16_t id, uint32_t gameTime, uint8_t messageType, uint32_t payloadLenght){
	MessageHeader dummyGameMessageHeader;
	dummyGameMessageHeader.user_id = id;
	dummyGameMessageHeader.message_type = messageType;
	dummyGameMessageHeader.gameTime = gameTime;
	dummyGameMessageHeader.payload_length = payloadLenght;
	return dummyGameMessageHeader;

}
