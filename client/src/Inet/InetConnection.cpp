#include <sys/select.h>

#include "Inet/InetConnection.hpp"
#include "Engine.hpp"

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
	// fill local addr_in
	memset(&me_addr, 0, sizeof(struct sockaddr_in));
	me_addr.sin_family = PF_UNSPEC;
	me_addr.sin_port = htons(0);
	// create udp socket and bind it
	if((socketudp = ::socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		cerr << "was not able to create udp socket!" << endl
		<< strerror(errno) << endl;
	}
	if(::bind(socketudp, reinterpret_cast<struct sockaddr*>(&me_addr), sizeof(me_addr)) < 0) {
        cerr << "was not able to bind udp port!" << endl
		<< strerror(errno) << endl;
    }
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
	if((::send(sockettcp, message.c_str(), sizeof(message.c_str()), 0))<0) {
		cerr << "TCP message send failed!" << endl << strerror(errno) << endl;
		return false;
	}
	return true;
}
void InetConnection::sendUDP(GAME_MESSAGE_TYPE type, const string& message) {
	// only adds outgoing message to list. put actual send to update

	//Message* msg = new GameMessage();
	//m_outgoing.insert({int(SDL_GetTicks()), msg});
}
// http://stackoverflow.com/questions/17769964/linux-sockets-non-blocking-connect
bool InetConnection::connectTCP() {
	if(sockettcp != 0) close(sockettcp);
	iter = nullptr;
	if(tcpsocketstatus) {
		// get server address information
		if(getaddrinfo(ip.c_str(), port.c_str(),&hints, &res) != 0) {
			cerr << "getaddrinfo error "<< strerror(errno) << endl;
			strerrno = strerror(errno);
			return false;
		}
		// create socket
		if((sockettcp = socket(res->ai_family, res->ai_socktype | SOCK_NONBLOCK, res->ai_protocol))< 0) {
			cerr << "cannot create socket for tcp connection " << strerror(errno) << endl;
			strerrno = strerror(errno);
			return false;
		}
		// bind to port
		if(::bind(sockettcp, reinterpret_cast<struct sockaddr*>(&me_addr), sizeof(struct sockaddr))<0) {
			cerr << "cannot bind tcp socket to random port" << endl << strerror(errno) << endl;
		}
		// create unblocking connect process
		int sock_res = ::connect(sockettcp, res->ai_addr, res->ai_addrlen);
		if(sock_res < 0) {
			if(errno == EINPROGRESS) {
				tcpsocketstatus = false;
				cout << "Trying to connect to host" << endl;
				return true;
			} else if (errno == ECONNREFUSED) m_state = ConnectionState::REFUSED;
			else m_state = ConnectionState::DISCONNECTED;
			cout << strerror(errno) << endl;
			return false;
		}
		if (sock_res == 0) {
			tcpsocketstatus = true;
			cout << "We are now connected to host" << endl;
			return true;
		}
	}
	return false;;
}
bool InetConnection::disconnect() {
	if (res == nullptr) freeaddrinfo(res);
	close(sockettcp);
	sockettcp = 0;
	m_state = ConnectionState::DISCONNECTED;
	return true;
}
void InetConnection::update() {
	// if connecting tcp
	FD_ZERO(&socket_fds);
	FD_SET(sockettcp, &socket_fds);
	if(!tcpsocketstatus && m_state == ConnectionState::CONNECTED) {
		int err;
		socklen_t len = sizeof(err);
		if(getsockopt(sockettcp, SOL_SOCKET, SO_ERROR, &err, &len)<0) {
			cerr << "getsockopt error: " << strerror(errno) << endl;
			strerrno = strerror(errno);
			cout << strerror(err) << endl;
			m_state = ConnectionState::REFUSED;
		} else if(err == EINPROGRESS)
			tcpsocketstatus = false;
		else if(err == 0) {
			timeout.tv_usec=100;
			timeout.tv_sec = 0;
			int ret = 0;
			if((ret = select(sockettcp+1, &socket_fds, NULL, NULL, &timeout))<0) {
				cout << ret << endl;
			}
			if(FD_ISSET(sockettcp, &socket_fds)) {
				char buffer[1024];
				recv(sockettcp, buffer, sizeof(buffer), 0);
				//cout << buffer << endl;
				if(strlen(buffer)>0) tcpsocketstatus = true;
			}
		} else
			strerrno = strerror(err);
		return;
	}
/*
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
Message * unpackedMessage;
switch (select(biggestsocket + 1, &socket_fds, NULL, NULL, &timeout)) {
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

for (int socket_fd = 0; socket_fd <= biggestsocket; socket_fd++) {
if (FD_ISSET(socket_fd, &socket_fds)) {
std::cout << "Unpacking" << std::endl;
Message::UnpackHeader(socket_fd, header, payloadBuffer);
unpackedMessage = MessageFactory::getInstance().getMessageByType(header, payloadBuffer);
}
}
break;
}

switch (unpackedMessage->getMessageType()) {
case MESSAGE_TYPE::GAME_MESSAGE:
//		switch (unpackedMessage->getMessageType()) {
//		case GAME_MESSAGE_TYPE::JOIN:
//			message = dynamic_cast<Join*>(unpackedMessage);
//		case GAME_MESSAGE_TYPE::NICK:
//			return dynamic_cast<Nick*>(unpackedMessage);
//		case GAME_MESSAGE_TYPE::EXIT:
//			return dynamic_cast<Exit*>(unpackedMessage);
//		case GAME_MESSAGE_TYPE::RESTART:
//			return dynamic_cast<Restart*>(unpackedMessage);
//		case GAME_MESSAGE_TYPE::GAME_END:
//			return dynamic_cast<GameEnd*>(unpackedMessage);
//		case GAME_MESSAGE_TYPE::GAME_UPDATE:
//			return dynamic_cast<GameUpdate*>(unpackedMessage);
//		case GAME_MESSAGE_TYPE::POINTS:
//			return dynamic_cast<Points*>(unpackedMessage);
//		case GAME_MESSAGE_TYPE::PLAYER_DEAD:
//			return dynamic_cast<PlayerDead*>(unpackedMessage);
//		case GAME_MESSAGE_TYPE::PLAYER_OUT:
//			return dynamic_cast<PlayerOut*>(unpackedMessage);
//		}
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
*/
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

for (auto& it : messages) {
	it->Update();
}
}
vector<ChatMessage*> InetConnection::getChatMessages() {
	vector<ChatMessage*> lmessages;
	for (unsigned int it = 0; it < m_inbox.size(); it++) {
		if (m_inbox[it]->getMessageType() == MESSAGE_TYPE::PLAYER_CHAT_MESSAGE) {
			//messages.push_back(dynamic_cast<ChatMessage*>(message));
			//message = m_messages.end();
			//m_messages.pop_back();
		}
	}
	return lmessages;
}

vector<PlayerDead*> InetConnection::getDeadPayers() {
	vector<PlayerDead*> lmessages;
	for (unsigned int it = 0; it < m_inbox.size(); it++) {
		if (m_inbox[it]->getMessageType() == MESSAGE_TYPE::GAME_MESSAGE) {

			if (dynamic_cast<GameMessage*>(m_inbox[it])->getGameMessageType() == GAME_MESSAGE_TYPE::PLAYER_DEAD) {
				messages.push_back(dynamic_cast<PlayerDead*>(m_inbox[it]));
				//delete m_messages[it];
				//m_messages[it] = m_messages.back();
				//m_messages.pop_back();
			}
		}
	}
	return lmessages;
}
