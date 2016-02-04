#ifndef _INETCONNECTION_HPP
#define _INETCONNECTION_HPP

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "Inet/Messages.hpp"
#include "Inet/MessagesAck.hpp"


using namespace std;

enum ConnectionState { DISCONNECTED, CONNECTING, CONNECTED, REFUSED, TIMED_OUT, EXITING };

class InetConnection {
friend class Engine;
private:
    /* private data */
    ConnectionState m_state = ConnectionState::DISCONNECTED;
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *iter;

    struct sockaddr_in server_addr; // lassi uses this
    struct sockaddr_in me_addr;

    struct hostent* server;
    int length = 0;
    int rval = 0;
    char dgram[1];
    std::string ip = "";
    std::string port = "";
    fd_set socket_fds;
    fd_set socket_fds_temp;
    struct timeval timeout;

    int sockettcp = 0;
    int socketudp = 0;
    int biggestsocket = -1;

    void unpack_header();

    vector<Message*> messageInbox;
    vector<Message*> m_outgoing;
    bool tcpsocketstatus = false;
protected:
    /* protected data */
    ~InetConnection() {;}
public:
    InetConnection();

    string strerrno = "";
    bool sendChatMessage(const string& message);
    bool connectTCP();
    void sendUDP(GAME_MESSAGE_TYPE type, const string& message);
    bool send(uint8_t *, int);
    bool disconnect();
    void update();
    int checkUDPConnections();
	int checkTCPConnection();

    std::vector<Message*> messages;
    void init();

    void destroy();
    inline const ConnectionState& getState() const { return m_state; }
    inline const bool& getTCPStatus() const { return tcpsocketstatus; }
    inline void setIP(const string& i) { ip = i; }
    std::vector<MessagesAck*> getAcks();
    std::vector<ChatMessage*> getChatMessages();
    std::vector<PlayerDead*> getDeadPayers();


};
#endif
