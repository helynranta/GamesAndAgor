#include "core/InetConnection.hpp"

InetConnection::InetConnection() {
    memset(&dgram, 1, 1);
    // c99
    /*
    hints = (struct addrinfo){
            .ai_flags = AI_NUMERICHOST|AI_NUMERICSERV,
            .ai_family = PF_UNSPEC,
            .ai_socktype = SOCK_DGRAM,
            .ai_protocol = IPPROTO_UDP
        };
    */
    // c++
    hints.ai_flags = AI_NUMERICHOST|AI_NUMERICSERV;
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
}
bool InetConnection::send() {
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
    //it->update();
  }
}