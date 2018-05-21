//
// Created by gajus123 on 17.04.18.
//

#ifndef TIN_TCPSERVER_H
#define TIN_TCPSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory.h>
#include <sys/ioctl.h>
#include <memory>
#include <boost/signals2.hpp>

#include "TCPSocket.h"

namespace Sockets {
    class TCPServer {
    public:
        TCPServer(SocketFacade *socket_interface);
        TCPServer(SocketFacade *socket_interface, int socket);
        TCPServer(TCPServer &) = delete;
        TCPServer(TCPServer &&other);
        TCPServer &operator=(TCPServer &) = delete;
        TCPServer &operator=(TCPServer &&other);
        ~TCPServer();

        void close();

        bool listen(in6_addr address, uint16_t server_port);
        bool listen(uint16_t server_port);
        bool listen(IP address, uint16_t server_port);
        bool listen(DNS address, uint16_t server_port);

        bool waitForConnection(int ms);

        std::shared_ptr<TCPSocket> accept();
        uint16_t port() const;

        int getDescriptor() const;

        boost::signals2::signal<void()> incomingConnection;
    private:
        SocketFacade *socket_interface_;

        int socket_;
        bool closed_;
        SocketError error_;

        void setError(SocketError error);
    };
}


#endif //TIN_TCPSERVER_H
