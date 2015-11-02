#ifndef _INETCONNECTION_HPP
#define _INETCONNECTION_HPP

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#include "Inet/Messages.hpp"

enum ConnectionState { DISCONNECTED, CONNECTING, CONNECTED, TIMING_OUT, EXITING };

class InetConnection {
friend class Engine;
private:
    /* private data */
    ConnectionState m_state = ConnectionState::DISCONNECTED;
    struct addrinfo hints;
    struct addrinfo *result = nullptr;
    struct addrinfo *iter = nullptr;
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
    void unpack_header();
protected:
    /* protected data */
    inline InetConnection() {;}
    inline ~InetConnection() {;}
public:
    /* public data */
    bool send();
    bool connect(std::string ip, std::string port);
    bool disconnect();
    void update();
    std::vector<Message*> messages;
    void init();
    void destroy();
};
#endif
