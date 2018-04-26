//
// Created by gajus123 on 26.04.18.
//

#include "../../inc/sockets/TCPSocketWithPacket.h"

TCPSocketWithPacket::TCPSocketWithPacket(SocketFacade* socket_interface) :
    TCPSocket(socket_interface) {
}

TCPSocketWithPacket::TCPSocketWithPacket(SocketFacade* socket_interface, int socket, SocketState state) :
    TCPSocket(socket_interface, socket, state) {

}

TCPSocketWithPacket::TCPSocketWithPacket(TCPSocketWithPacket&& other) :
    TCPSocket(std::move(other)) {
}

TCPSocketWithPacket& TCPSocketWithPacket::operator=(TCPSocketWithPacket&& other) {
    TCPSocket::operator=(std::move(other));
    return *this;
}

TCPSocketWithPacket::~TCPSocketWithPacket() {
}