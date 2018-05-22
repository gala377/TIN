//
// Created by gajus123 on 16.04.18.
//

#include "sockets/TCPSocketBase.h"

namespace Sockets {
    TCPSocketBase::TCPSocketBase(SocketFacade *socket_interface) :
            socket_interface_(socket_interface) {
        socket_ = socket_interface_->socket(AF_INET6, SOCK_STREAM, 0);
        state_ = SocketState::UNCONNECTED;
        if (socket_ == -1)
            throw SocketInitializationException();
        socket_interface_->setFlags(socket_, O_NONBLOCK);
    }

    TCPSocketBase::TCPSocketBase(SocketFacade *socket_interface, int socket, SocketState state) :
            socket_interface_(socket_interface),
            socket_(socket),
            state_(state) {
    }

    TCPSocketBase::~TCPSocketBase() {
        close();
    }

    TCPSocketBase::TCPSocketBase(TCPSocketBase &&other) {
        socket_ = other.socket_;
        state_ = other.state_;
        other.socket_ = -1;
    }

    TCPSocketBase &TCPSocketBase::operator=(TCPSocketBase &&other) {
        close();
        socket_ = other.socket_;
        state_ = other.state_;
        other.socket_ = -1;

        return *this;
    }

    void TCPSocketBase::close() {
        if (socket_ >= 0) {
            int status = socket_interface_->close(socket_);
            if (status == -1) {
                //TODO error handling
                return;
            }
            socket_ = -1;
        }
    }

    bool TCPSocketBase::connect(in6_addr address, uint16_t port) {
        struct sockaddr_in6 server = createAddress(address, port);
        if (socket_interface_->connect(socket_, (struct sockaddr *) &server, sizeof(server)) == -1) {
            std::cout << "Connect error " << strerror(socket_interface_->getErrno());
            switch (socket_interface_->getErrno()) {
                case EISCONN:
                    setConnected();
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
        setConnected();
        return true;
    }

    bool TCPSocketBase::connect(IP address, uint16_t port) {
        return connect(ip(address), port);
    }

    bool TCPSocketBase::connect(DNS address, uint16_t port) {
        return connect(ip(address), port);
    }

    uint16_t TCPSocketBase::port() const {
        return socket_interface_->port(socket_);
    }

    int TCPSocketBase::write(std::string buffer) {
        if (buffer.size() == 0)
            return 0;
        return write(&buffer.front(), buffer.size());
    }

    int TCPSocketBase::write(char *buffer, unsigned int size) {
        if (size == 0)
            return 0;
        int status = socket_interface_->write(socket_, buffer, size);
        if (status == -1) {
            switch (socket_interface_->getErrno()) {
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

    int TCPSocketBase::privateRead(char *buffer, unsigned int size) {
        int status = socket_interface_->read(socket_, buffer, size);
        if (status == -1) {
            std::cout << strerror(socket_interface_->getErrno());
            switch (socket_interface_->getErrno()) {
                case EAGAIN: //non-blocking - no data - return 0 without error
                    status = 0;
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
        } else if (status == 0) { //Other side called close()
            setState(SocketState::UNCONNECTED);
            setError(SocketError::HOST_CLOSED);
            close();
            status = -1;
        }
        return status;
    }

    void TCPSocketBase::read(char *buffer, unsigned int size) {
        buffer_.sgetn(buffer, size);
    }

    void TCPSocketBase::setConnected() {
        setState(SocketState::CONNECTED);
        connected();
    }

    int TCPSocketBase::availableBytes() const {
        return buffer_.size();
    }

    void TCPSocketBase::setState(SocketState state) {
        state_ = state;
    }

    void TCPSocketBase::setError(SocketError error) {
        error_ = error;
    }

    SocketError TCPSocketBase::getError() const {
        return error_;
    }

    SocketState TCPSocketBase::getState() const {
        return state_;
    }

    int TCPSocketBase::getDescriptor() const {
        return socket_;
    }

    void TCPSocketBase::readFromSocket() {
        int bytes = socketAvailableBytes();
        char *buffer = new char[bytes];
        privateRead(buffer, bytes);
        buffer_.sputn(buffer, bytes);
    }

    int TCPSocketBase::socketAvailableBytes() const {
        return socket_interface_->availableBytes(socket_);
    }
}