#include "Inet/InetConnection.hpp"

ConnectionState   InetConnection::m_state = ConnectionState::DISCONNECTED;
struct addrinfo   InetConnection::hints = {};
struct addrinfo   *InetConnection::result = nullptr;
struct addrinfo   *InetConnection::iter = nullptr;
int               InetConnection::socketfd = -1;
int               InetConnection::length = 0;
int               InetConnection::rval = 0;
char              InetConnection::dgram[1];
std::string       InetConnection::ip = "";
std::string       InetConnection::port = "";

std::vector<Message*> InetConnection::messages;

void InetConnection::init(void) {
    // this is temp. replace 1 with size of message
    memset(&dgram, 1, 1);
    // fill hints with rightful flags
    hints.ai_flags = AI_NUMERICHOST|AI_NUMERICSERV;
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
}
/** deconstructor
* frees all used memory and empties messages vector
* @params: void
*

*/
void InetConnection::destroy(void)  {
    // delete messages behind pointers
    for ( auto& it : messages) {
        delete it;
    }
    // empty whole vector
    messages.empty();
}
/** send
* this function sends socket messages
* @params: void
* @return: bool success. returns success if there was no socket error
*/
bool InetConnection::send(void) {
    bool success = false;
    if(getaddrinfo(ip.c_str(), port.c_str(), &hints, &result) < 0) {
        std::cout << "Cannot resolve address" << std::endl;
    }
    else {
        std::cout << "getaddrinof sucess" << std::endl;
        // Go through every returned address and attempt to connect to each
        for (iter = result; iter != NULL; iter = iter->ai_next) {
            /* Can socket be created? */
            if ((socketfd = socket(iter->ai_family, iter->ai_socktype,iter->ai_protocol)) < 0) {
                std::cout << "Error socket()" << std::endl;
            break;
        }

        /* Try to send data to server:
        * sendto(socket, data , data length, flags, destination, struct length)
        * see 'man sendto'
        */
        if((length = sendto(socketfd,&dgram,1,0,iter->ai_addr,iter->ai_addrlen)) < 0) {
            std::cout << "error sendto()" << std::endl;
            break;
        }
        else {
            std::cout << "Client: Sent datagram legnth = " << length << std::endl;
            success = true;
            }
        }
    }
    return success;
}
/**
* connect
* this function tries to connect to given server crediterials.
* it makes ConnectionState CONNECTING and then tries to send message to connect
* @params: string ip, string port. Port and IP of the server we are connecting.
* @return: bool success. returns success if there was no socket error
*/
bool InetConnection::connect(std::string ip, std::string port) {
    m_state = ConnectionState::CONNECTING;
    /*
    send to asdfasdf
    */
    return true;
}
bool InetConnection::disconnect() {
  if (result == nullptr)
    freeaddrinfo(result);
  //close(socketfd);
  return true;
}
void InetConnection::update() {

  // SELECT
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
  //if(m_state == ConnectionState::CONNECTING)
    //
  for ( auto& it : messages) {
    it->update();
  }
}
