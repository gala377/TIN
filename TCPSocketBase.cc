//
// Created by gajus123 on 18.04.18.
//

#include "TCPSocketBase.h"

in6_addr ip(const IP &address) {
    in6_addr result;
    inet_pton(AF_INET6, address.address.c_str(), &result);
    return result;
}

in6_addr ip(const DNS &address) {
    //TODO
    return in6addr_any;
}

struct sockaddr_in6 createAddress(in6_addr address, uint16_t port) {
    struct sockaddr_in6 result;
    result.sin6_family = AF_INET6;
    result.sin6_addr = address;
    result.sin6_port = htons(port);
    return result;
}