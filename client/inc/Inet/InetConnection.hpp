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

#include "Inet/Messages.hpp"

enum ConnectionState { DISCONNECTED, CONNECTING, CONNECTED, TIMING_OUT, EXITING };

class InetConnection {
friend class Engine;
private:
    /* private data */
    ConnectionState m_state = ConnectionState::DISCONNECTED;
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *iter;
    int length = 0;
    int rval = 0;
    char dgram[1];
    std::string serverIP = "";
    std::string serverPort = "";
    fd_set socket_fds;
    fd_set socket_fds_temp;
    struct timeval timeout;
    int tcp_socket_fd = -1;
    int udpListensocket = -1;
    int biggestSocket = -1;
    void unpack_header();
    std::vector<Message*> m_messages;
protected:

public:
    /* protected data */
    InetConnection();
    ~InetConnection() {;}
    int createTCPSocket(std::string serverip, std::string port);
    bool send(uint8_t *, int);
    bool startConnection(std::string ip, std::string port);
    bool disconnect();
    void update();
    std::vector<Message*> messages;
    void init(std::string l_ip, std::string l_port);
    void destroy();
    std::vector<ChatMessage*> getChatMessages();
    std::vector<PlayerDead*> getDeadPayers();
    std::vector<Join*> getJoinMessages();
    std::vector<Join*> getJoinAckMessages();
};
#endif
