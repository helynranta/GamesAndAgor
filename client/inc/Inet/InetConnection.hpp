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
    void unpack_header();

    map<int, Message*> m_inbox;
    map<int, Message*> m_outgoing;
    bool tcpsocketstatus = false;
protected:
    /* protected data */
    InetConnection();
    ~InetConnection() {;}
public:
    string strerrno = "";
    bool sendChatMessage(const string& message);
    void sendUDP(GAME_MESSAGE_TYPE type, const string& message);
    bool connectTCP(const string& ip, const string& port);
    bool disconnect();
    void update();
    std::vector<Message*> messages;
    void init();
    void destroy();
    inline const ConnectionState& getState() const { return m_state; }
    vector<ChatMessage*> getChatMessages();
    vector<PlayerDead*> getDeadPayers();
    inline const bool& getTCPStatus() const { return tcpsocketstatus; }

};
#endif
