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

#include "TCPSocketBase.h"

class TCPSocket {
public:
    TCPSocket();
    TCPSocket(int socket, SocketState state = SocketState::CONNECTED);
    TCPSocket(TCPSocket&) = delete;
    TCPSocket(TCPSocket&& other);
    TCPSocket& operator=(TCPSocket&) = delete;
    TCPSocket& operator=(TCPSocket&& other);

    ~TCPSocket();

    void close();

    bool connect(in6_addr address, uint16_t port);
    bool connect(IP address, uint16_t port);
    bool connect(DNS address, uint16_t port);

    struct sockaddr_in6 address() const;
    uint16_t port() const;

    int write(char* buffer, unsigned int size);
    int write(std::string buffer);

    int read(char* buffer, unsigned int size);

    int availableBytes() const;

    SocketError getError() const;
    SocketState getState() const;
private:
    int socket_;
    SocketState state_;
    SocketError error_;
    bool closed_;

    void setState(SocketState state);
    void setError(SocketError error);
};


#endif //TIN_TCPSOCKET_H
