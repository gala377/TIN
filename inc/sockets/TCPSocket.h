//
// Created by gajus123 on 26.04.18.
//

#ifndef TIN_TCPSOCKETWITHPACKET_H
#define TIN_TCPSOCKETWITHPACKET_H

#include <deque>
#include <cinttypes>

#include "TCPSocketBase.h"
#include "../messages/Message.h"
#include "../messages/Acknowledge.h"

class Packet;

namespace Sockets {
    class TCPSocket : public TCPSocketBase {
    public:
        TCPSocket(SocketFacade *socket_interface);
        TCPSocket(SocketFacade *socket_interface, int socket, in6_addr address, uint16_t port, SocketState state = SocketState::CONNECTED);
        TCPSocket(TCPSocket &) = delete;
        TCPSocket(TCPSocket &&other);
        TCPSocket &operator=(TCPSocket &) = delete;
        TCPSocket &operator=(TCPSocket &&other);
        ~TCPSocket();

        bool waitForMessage(int secs);

        void writeMessage(Message& message);

        std::shared_ptr<Message> readMessage();
        std::shared_ptr<Message> peekBack();
        void popBack();

        int availableMessages();

        boost::signals2::signal<void()> packetReady;
    private:
        std::deque<std::shared_ptr<Message>> messages_;
        void createMessageHandler();
    };
}


#endif //TIN_TCPSOCKETWITHPACKET_H
