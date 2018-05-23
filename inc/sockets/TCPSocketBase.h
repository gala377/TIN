//
// Created by gajus123 on 16.04.18.
//

#ifndef TIN_TCPSOCKET_H
#define TIN_TCPSOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory.h>
#include <sys/ioctl.h>
#include <boost/signals2.hpp>
#include <sstream>
#include <boost/asio/streambuf.hpp>

#include "SocketHelpers.h"
#include "SocketFacade.h"

namespace Sockets {
    class TCPSocketBase {
    public:
        TCPSocketBase(SocketFacade* socket_interface);
        TCPSocketBase(SocketFacade* socket_interface, int socket, SocketState state = SocketState::CONNECTED);
        TCPSocketBase(SocketFacade* socket_interface, int socket, in6_addr address, uint16_t port, SocketState state = SocketState::CONNECTED);
        TCPSocketBase(TCPSocketBase&) = delete;
        TCPSocketBase(TCPSocketBase&& other);
        TCPSocketBase& operator=(TCPSocketBase&) = delete;
        TCPSocketBase& operator=(TCPSocketBase&& other);

        ~TCPSocketBase();

        void close();

        bool connect(in6_addr address, uint16_t port);
        bool connect(IP address, uint16_t port);
        bool connect(DNS address, uint16_t port);
        bool connect();

        uint16_t port() const;
        in6_addr address() const;

        int write(char* buffer, unsigned int size);
        int write(std::string buffer);

        void read(char* buffer, unsigned int size);

        int availableBytes() const;

        SocketError getError() const;
        SocketState getState() const;

        int getDescriptor() const;

        boost::signals2::signal<void ()> readyRead;
        boost::signals2::signal<void ()> connected;
        boost::signals2::signal<void ()> disconnected;

        void setConnected();
        void readFromSocket();
    protected:
        boost::asio::streambuf buffer_;
    private:
        SocketFacade* socket_interface_;

        int socket_;
        SocketState state_;
        SocketError error_;

        in6_addr address_;
        uint16_t port_;

        void setState(SocketState state);
        void setError(SocketError error);

        int privateRead(char* buffer, unsigned int size);
        int socketAvailableBytes() const;
    };
}


#endif //TIN_TCPSOCKET_H
