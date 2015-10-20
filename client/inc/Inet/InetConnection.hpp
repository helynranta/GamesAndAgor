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
private:
    static ConnectionState m_state;
    static struct addrinfo hints;
    static struct addrinfo *result;
    static struct addrinfo *iter;
    static int length;
    static int rval;
    static char dgram[1];
    static std::string ip;
    static std::string port;
    static fd_set socket_fds;
    static fd_set socket_fds_temp;
    static struct timeval timeout;
    static int listensocket;
    static int biggestsocket;
    static void unpack_header();
public:
    static bool send();
    static bool connect(std::string ip, std::string port);
    static bool disconnect();
    static void update();
    static std::vector<Message*> messages;
    static void init();
    static void destroy();
};
#endif
