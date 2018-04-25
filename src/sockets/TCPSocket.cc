//
// Created by gajus123 on 16.04.18.
//

#include "../../inc/sockets/TCPSocket.h"

TCPSocket::TCPSocket(SocketFacade* socket_interface) :
    socket_interface_(socket_interface) {
    socket_ = socket_interface_->socket(AF_INET6, SOCK_STREAM, 0);
    state_ = SocketState::UNCONNECTED;
    if(socket_ == -1)
        throw SocketInitializationException();
    //fcntl(socket_, F_SETFL, fcntl(socket_, F_GETFL, 0) | O_NONBLOCK);
    socket_interface_->setFlags(socket_, O_NONBLOCK);
}

TCPSocket::TCPSocket(SocketFacade* socket_interface, int socket, SocketState state ) :
    socket_interface_(socket_interface),
    socket_(socket),
    state_(state) {
}

TCPSocket::~TCPSocket() {
    close();
}

TCPSocket::TCPSocket(TCPSocket&& other) {
    socket_ = other.socket_;
    state_ = other.state_;
    other.socket_ = -1;
}

TCPSocket& TCPSocket::operator=(TCPSocket&& other) {
    close();
    socket_ = other.socket_;
    state_ = other.state_;
    other.socket_ = -1;

    return *this;
}

void TCPSocket::close() {
    if(socket_ >= 0) {
        int status = socket_interface_->close(socket_);
        if (status == -1) {
            std::cout << strerror(socket_interface_->getErrno()) << "\n";
            return;
        }
        socket_ = -1;
    }
}

bool TCPSocket::connect(in6_addr address, uint16_t port) {
    struct sockaddr_in6 server = createAddress(address, port);
    if(socket_interface_->connect(socket_, (struct sockaddr*) &server, sizeof(server)) == -1) {
        std::cout << strerror(socket_interface_->getErrno()) << "\n";
        switch(socket_interface_->getErrno()) {
            case EISCONN:
                setState(SocketState::CONNECTED);
                break;
            case ECONNREFUSED:
            case EINVAL:
                setError(SocketError::CONNECTION_REFUSED);
                setState(SocketState::UNCONNECTED);
                break;
            case ETIMEDOUT:
                setError(SocketError::NETWORK_ERROR);
                break;
            case EHOSTUNREACH:
                setError(SocketError::NETWORK_ERROR);
                setState(SocketState::UNCONNECTED);
                break;
            case ENETUNREACH:
                setError(SocketError::NETWORK_ERROR);
                setState(SocketState::UNCONNECTED);
                break;
            case EADDRINUSE:
                setError(SocketError::ADDRESS_IN_USE);
                break;
            case EINPROGRESS:
            case EALREADY:
                setError(SocketError::UNFINISHED_OPERATION);
                setState(SocketState::CONNECTING);
                break;
            case EAGAIN:
                setError(SocketError::UNFINISHED_OPERATION);
                break;
            case EACCES:
            case EPERM:
                setError(SocketError::ACCESS_ERROR);
                setState(SocketState::UNCONNECTED);
                break;
            case EAFNOSUPPORT:
            case EBADF:
            case EFAULT:
            case ENOTSOCK:
                setState(SocketState::UNCONNECTED);
                break;
        }
        return false;
    }
    setState(SocketState::CONNECTED);
    return true;
}

bool TCPSocket::connect(IP address, uint16_t port) {
    return connect(ip(address), port);
}

bool TCPSocket::connect(DNS address, uint16_t port) {
    return connect(ip(address), port);
}

uint16_t TCPSocket::port() const {
    struct sockaddr_in6 temp = address();
    return ntohs(temp.sin6_port);
}

int TCPSocket::write(std::string buffer) {
    if(buffer.size() == 0)
        return 0;
    return write(&buffer.front(), buffer.size());
}

int TCPSocket::write(char* buffer, unsigned int size) {
    if(size == 0)
        return 0;
    int status = socket_interface_->write(socket_, buffer, size);
    if(status == -1) {
        switch(socket_interface_->getErrno()) {
            case EPIPE:
            case ECONNRESET:
                setError(SocketError::HOST_CLOSED);
                close();
                break;
            case EAGAIN:
                status = 0;
                break;
        }
    }
    return status;
}

int TCPSocket::read(char* buffer, unsigned int size) {
    int status = socket_interface_->read(socket_, buffer, size);
    if(status == -1) {
        switch (socket_interface_->getErrno()) {
            case EAGAIN: //non-blocking - no data - zwróć 0 bez żadnych błędów
                status = 0;
                std::cout << "No data\n";
                break;
            case ECONNRESET: //connection closed - close() this side and return -1
                setError(SocketError::HOST_CLOSED);
                close();
                break;
            default:
                close();
                setError(SocketError::NETWORK_ERROR);
                setState(SocketState::UNCONNECTED);
                break;
        }
    }
    else if(status == 0) { //Other side called close()
        setState(SocketState::UNCONNECTED);
        setError(SocketError::HOST_CLOSED);
        close();
        status = -1;
    }
    return status;
}

int TCPSocket::availableBytes() const {
    return socket_interface_->availableBytes(socket_);
}

struct sockaddr_in6 TCPSocket::address() const {
    struct sockaddr_in6 address;
    socklen_t length = sizeof(address);
    if(getsockname(socket_, (struct sockaddr*) &address, &length) == -1) {
//TODO error handling
    }
    return address;
}

void TCPSocket::setState(SocketState state) {
    state_ = state;
}

void TCPSocket::setError(SocketError error) {
    error_ = error;
}

SocketError TCPSocket::getError() const {
    return error_;
}

SocketState TCPSocket::getState() const {
    return state_;
}

int TCPSocket::getDescriptor() const {
    return socket_;
}