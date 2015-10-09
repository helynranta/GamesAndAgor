#pragma once

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

enum ConnectionState { DISCONNECTED, CONNECTING, CONNECTED, TIMING_OUT };

class InetConnection {
private:

    ConnectionState m_state = ConnectionState::DISCONNECTED;

    struct addrinfo hints = {};
    struct addrinfo *results = nullptr, *iter = nullptr;
    int socketfd = -1;
    int length = 0;
    int rval = 0;
    char dgram[1];
public:
    InetConnection ();
    inline virtual ~InetConnection () {}
    bool send();
    bool connect();
};
