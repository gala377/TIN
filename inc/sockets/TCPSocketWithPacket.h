//
// Created by gajus123 on 26.04.18.
//

#ifndef TIN_TCPSOCKETWITHPACKET_H
#define TIN_TCPSOCKETWITHPACKET_H

#include <queue>
#include <cinttypes>

#include "TCPSocket.h"
#include "../messages/Message.h"
#include "../messages/Acknowledge.h"

class Packet;

class TCPSocketWithPacket : public TCPSocket {
public:
    TCPSocketWithPacket(SocketFacade* socket_interface);
    TCPSocketWithPacket(SocketFacade* socket_interface, int socket, SocketState state = SocketState::CONNECTED);
    TCPSocketWithPacket(TCPSocketWithPacket&) = delete;
    TCPSocketWithPacket(TCPSocketWithPacket&& other);
    TCPSocketWithPacket& operator=(TCPSocketWithPacket&) = delete;
    TCPSocketWithPacket& operator=(TCPSocketWithPacket&& other);

    ~TCPSocketWithPacket();

    void writePacket(Message* message);

    Message* readPacket();

    int availablePackets();

    boost::signals2::signal<void ()> packetReady;
private:
    std::queue<Message*> messages_;
};


#endif //TIN_TCPSOCKETWITHPACKET_H
