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

#include "Inet/Messages.hpp"

using namespace std;

enum ConnectionState { DISCONNECTED, CONNECTING, CONNECTED, REFUSED, TIMING_OUT, EXITING };

class InetConnection {
friend class Engine;
private:
    /* private data */
    ConnectionState m_state = ConnectionState::DISCONNECTED;
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *iter;
    
    struct sockaddr_in server_addr; // lassi uses this
    struct hostent* server;
    int length = 0;
    int rval = 0;
    char dgram[1];
    std::string ip = "";
    std::string port = "";
    fd_set socket_fds;
    fd_set socket_fds_temp;
    struct timeval timeout;
    
    int listensocket = -1;
    int biggestsocket = -1;
    
    int sockettcp;
    void unpack_header();
    std::vector<Message*> m_messages;
protected:
    /* protected data */
    InetConnection();
    ~InetConnection() {;}
public:
    bool send(std::string l_ip, std::string l_port, std::string message);
    bool connect(const std::string& ip, const std::string& port);
    bool disconnect();
    void update();
    std::vector<Message*> messages;
    void init();
    void destroy();

    inline const ConnectionState& getState() const { return m_state; }

    std::vector<ChatMessage*> getChatMessages();
    std::vector<PlayerDead*> getDeadPayers();

};
#endif
