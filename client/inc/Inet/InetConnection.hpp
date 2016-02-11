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
#include "MessageAck.hpp"

// Defines if we really want to start the game or just use simple game loop to test messages
#define MESG_TEST = 0


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

    string ip = "127.0.0.1";
    string portUDP = "8888";
    string portTCP = "8889";
    fd_set socket_fds;
    fd_set socket_fds_temp;
    struct timeval timeout;

    int sockettcp = 0;
    int socketudp = 0;
    int biggestsocket = -1;

    void unpack_header();
	void UnpackGameMessageSubType(Message* unpackedMessage);
	void UnpackAckMessageSubtype(Message* unpackedMessage);

    vector<Message*> messageInbox;
    vector<Message*> m_outgoing;
    bool tcpsocketstatus = false;
    vector<string> chatmessage;
    uint16_t id = -1;
protected:
    /* protected data */

public:
    InetConnection();
    virtual ~InetConnection();
    bool sendChatMessage(const string& message);
    bool connectTCP();
    bool connectUDP();
    void sendUDP(GAME_MESSAGE_TYPE type, const string& message);
    void sendTCP(const string& message);
    bool send(uint8_t *, int);
    bool disconnect();
    int  update();
    int checkUDPConnections();
	int checkTCPConnection();

    vector<Message*> messages;
    void init();

    void destroy();
    inline const ConnectionState& getState() const { return m_state; }
    inline void setState(ConnectionState state) { m_state = state; }

    inline const bool& getTCPStatus() const { return tcpsocketstatus; }
    inline void setIP(const string& i) { ip = i; }

    vector<Message*> getMessagesOfType(MESSAGE_TYPE);
    vector<Message*> getMessagesOfType(MESSAGE_TYPE, GAME_MESSAGE_TYPE);

    vector<GameUpdate*> getGameUpdateMessages();
    vector<string> getChatMessages();
    vector<PlayerDead*> getDeadPayers();
    bool getGameEnding();
    inline void setID(uint16_t i) { id = i; }
    inline const uint16_t& getID() const { return id; }

    MessageHeader createDummyHeader(uint16_t id, uint32_t gameTime, uint8_t messageType, uint32_t payloadLenght);
};
#endif
