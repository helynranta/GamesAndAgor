#pragma once


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <iostream>
#include <vector>

#include "Inet/Messages.hpp"

enum ConnectionState { DISCONNECTED, CONNECTING, CONNECTED, TIMING_OUT, EXITING };

class InetConnection {
private:

    ConnectionState m_state = ConnectionState::DISCONNECTED;

    struct addrinfo hints = {};
    struct addrinfo *result = nullptr, *iter = nullptr;
    int socketfd = -1;
    int length = 0;
    int rval = 0;
    char dgram[1];

    std::string ip = "";
    std::string port = "";
public:
    InetConnection ();
    virtual ~InetConnection ();
    bool send();
    bool connect(std::string ip, std::string port);
    bool disconnect();
    void update();

    std::vector<Message*> messages;
};
