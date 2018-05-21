//
// Created by gajus123 on 26.04.18.
//

#ifndef TIN_TCPSOCKETWITHPACKET_H
#define TIN_TCPSOCKETWITHPACKET_H

#include <queue>
#include <cinttypes>

#include "TCPSocketBase.h"
#include "../messages/Message.h"
#include "../messages/Acknowledge.h"

class Packet;

class TCPSocket : public TCPSocketBase {
public:
    TCPSocket(SocketFacade* socket_interface);
    TCPSocket(SocketFacade* socket_interface, int socket, SocketState state = SocketState::CONNECTED);
    TCPSocket(TCPSocket&) = delete;
    TCPSocket(TCPSocket&& other);
    TCPSocket& operator=(TCPSocket&) = delete;
    TCPSocket& operator=(TCPSocket&& other);

    ~TCPSocket();

    void writePacket(Message* message);

    Message* readPacket();

    int availablePackets();

    boost::signals2::signal<void ()> packetReady;
private:
    std::queue<Message*> messages_;

    void createMessageHandler();
};


#endif //TIN_TCPSOCKETWITHPACKET_H
