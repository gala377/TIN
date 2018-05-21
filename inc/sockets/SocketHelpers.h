//
// Created by gajus123 on 18.04.18.
//

#ifndef TIN_TCPSOCKETBASE_H
#define TIN_TCPSOCKETBASE_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string>
#include <iostream>

struct IP {
    std::string address;
};

struct DNS {
    std::string address;
};

enum class SocketError {
    UNSUPPORTED_OPERATION, ADDRESS_IN_USE, ACCESS_ERROR, ADDRESS_NOT_AVAILABLE, NETWORK_ERROR,
    SOCKET_RESOURCE, TEMPORARY_ERROR, CONNECTION_REFUSED, UNFINISHED_OPERATION, HOST_CLOSED,
};

enum class SocketState {
    UNCONNECTED, BOUND, LISTEN, CONNECTING, CONNECTED, CLOSING
};

in6_addr ip(const IP& address);
in6_addr ip(const DNS& address);
struct sockaddr_in6 createAddress(in6_addr address, uint16_t port);

struct SocketInitializationException : public std::exception {
    const char * what () const throw () {
        return "Cannot initilize BSD socket";
    }
};


#endif //TIN_TCPSOCKETBASE_H
