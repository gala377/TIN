//
// Created by gajus123 on 17.04.18.
//

#include <c++/5/bits/c++config.h>
#include "TCPServer.h"

TCPServer::TCPServer() {
    socket_ = socket(AF_INET6, SOCK_STREAM, 0);
    closed_ = false;
    if(socket_ == -1)
        throw SocketInitializationException();
}

TCPServer::TCPServer(int socket) :
    socket_(socket),
    closed_(false) {
}

TCPServer::~TCPServer() {
    close();
}

TCPServer::TCPServer(TCPServer&& other) {
    socket_ = other.socket_;
    closed_ = other.closed_;
    other.socket_ = 0;
    other.closed_ = true;
}

TCPServer& TCPServer::operator=(TCPServer&& other) {
    close();
    socket_ = other.socket_;
    closed_ = other.closed_;
    other.socket_ = 0;
    other.closed_ = true;

    return *this;
}

void TCPServer::close() {
    if(!closed_) {
        if(::close(socket_) == -1) {
//TODO error handling
            std::cout << strerror(errno) << "\n";
            return;
        }

        closed_ = true;
        socket_ = 0;
    }
}

bool TCPServer::listen(in6_addr address, uint16_t server_port) {
    struct sockaddr_in6 server = createAddress(address, server_port);
    if(bind(socket_, (struct sockaddr*) &server, sizeof(server)) == -1) {
        std::cout << strerror(errno) << "\n";
        switch(errno) {
            case EADDRINUSE:
                setError(SocketError::ADDRESS_IN_USE);
                break;
            case EACCES:
                setError(SocketError::ACCESS_ERROR);
                break;
            case EINVAL:
                setError(SocketError::UNSUPPORTED_OPERATION);
                break;
            case EADDRNOTAVAIL:
                setError(SocketError::ADDRESS_NOT_AVAILABLE);
                break;
        }
        return false;
    }

    if(::listen(socket_, 1) == -1) {
        switch(errno) {
            case EADDRINUSE:
                setError(SocketError::ADDRESS_IN_USE);
                break;
        }
        return false;
    }
    return true;
}

bool TCPServer::listen(uint16_t server_port) {
    return listen(in6addr_any, server_port);
}

bool TCPServer::listen(IP address, uint16_t server_port) {
    return listen(ip(address), server_port);
}

bool TCPServer::listen(DNS address, uint16_t server_port) {
    return listen(ip(address), server_port);
}

std::shared_ptr<TCPSocket> TCPServer::accept() {
    int status = ::accept(socket_, (struct sockaddr*) nullptr, (socklen_t*) nullptr);
    if(status == -1) {
        std::cout << strerror(errno) << "\n";
        switch(errno) {
            case EBADF:
            case EOPNOTSUPP:
                setError(SocketError::UNSUPPORTED_OPERATION);
                break;
            case ECONNABORTED:
                setError(SocketError::NETWORK_ERROR);
                break;
            case EFAULT:
            case ENOTSOCK:
                setError(SocketError::SOCKET_RESOURCE);
                break;
            case EPROTONOSUPPORT:
            case EAFNOSUPPORT:
            case EINVAL:
                setError(SocketError::UNSUPPORTED_OPERATION);
                break;
            case ENFILE:
            case EMFILE:
            case ENOBUFS:
            case ENOMEM:
                setError(SocketError::SOCKET_RESOURCE);
                break;
            case EACCES:
            case EPERM:
                setError(SocketError::ACCESS_ERROR);
                break;
            case EAGAIN:
                setError(SocketError::TEMPORARY_ERROR);
                break;
        }
        return nullptr;
    }
    return std::make_shared<TCPSocket>(status);
}

uint16_t TCPServer::port() const {
    struct sockaddr_in6 address;
    socklen_t length = sizeof(address);
    if(getsockname(socket_, (struct sockaddr*) &address, &length) == -1)
        std::cout << "Error\n";
//TODO error handling
    return ntohs(address.sin6_port);
}

void TCPServer::setError(SocketError error) {
    error_ = error;
}