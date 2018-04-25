//
// Created by gajus123 on 17.04.18.
//

#include "../../inc/sockets/TCPServer.h"

TCPServer::TCPServer(SocketFacade* socket_interface) :
    socket_interface_(socket_interface) {
    socket_ = socket_interface_->socket(AF_INET6, SOCK_STREAM, 0);
    closed_ = false;
    if(socket_ == -1)
        throw SocketInitializationException();
}

TCPServer::TCPServer(SocketFacade* socket_interface, int socket) :
    socket_interface_(socket_interface),
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
        if(socket_interface_->close(socket_) == -1) {
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
    if(socket_interface_->bind(socket_, (struct sockaddr*) &server, sizeof(server)) == -1) {
        std::cout << strerror(socket_interface_->getErrno()) << "\n";
        switch(socket_interface_->getErrno()) {
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

    if(socket_interface_->listen(socket_, 1) == -1) {
        switch(socket_interface_->getErrno()) {
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
    int status = socket_interface_->accept(socket_, (struct sockaddr*) nullptr, (socklen_t*) nullptr);
    if(status == -1) {
        std::cout << strerror(socket_interface_->getErrno()) << "\n";
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
    return std::make_shared<TCPSocket>(socket_interface_, status);
}

uint16_t TCPServer::port() const {
    socket_interface_->port(socket_);
}

void TCPServer::setError(SocketError error) {
    error_ = error;
}

bool TCPServer::waitForConnection(int ms) {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(socket_, &set);
    struct timeval timeout;
    timeout.tv_sec = ms / 1000;
    timeout.tv_usec = (ms % 1000) * 1000;
    int status = socket_interface_->select(socket_+1, &set, NULL, NULL, &timeout);
    if(status == -1) {
        std::cout << "Select error: " << strerror(errno) << "\n";
    }
    else if(status == 0) {
        return false;
    }
    else {
        if(FD_ISSET(socket_, &set)) {
            return true;
        }
    }
}

int TCPServer::getDescriptor() const {
    return socket_;
}