//
// Created by gajus123 on 16.04.18.
//

#ifndef TIN_COMMUNICATIONMODULE_H
#define TIN_COMMUNICATIONMODULE_H

#include <memory>

#include "sockets/TCPServer.h"
#include "sockets/TCPSocket.h"
#include "sockets/SocketSupervisor.h"
#include "sockets/SocketUnix.h"

class CommunicationModule {
public:
    CommunicationModule(CommunicationModule& other) = delete;
    CommunicationModule(CommunicationModule&& other);
    CommunicationModule& operator=(CommunicationModule& other) = delete;
    CommunicationModule& operator=(CommunicationModule&& other);
    ~CommunicationModule();

    static CommunicationModule createServer(uint16_t port);
    static CommunicationModule createClient(uint16_t port, Sockets::IP address, uint16_t server_port);

    boost::signals2::signal<void()> incommingMessage;
private:
    CommunicationModule(uint16_t port);

    void prepareSocket();

    Sockets::SocketUnix socket_facade_;
    Sockets::TCPServer server_;
    std::shared_ptr<Sockets::TCPSocket> socket_;
    Sockets::SocketSupervisor supervisor_;

    enum class State {
        UNCONNECTED, CONNECTED, DISCONNECTED
    } state_;
};

#endif //TIN_COMMUNICATIONMODULE_H