//
// Created by gajus123 on 26.04.18.
//

#include "sockets/TCPSocket.h"

namespace Sockets {
    TCPSocket::TCPSocket(SocketFacade *socket_interface) :
            TCPSocketBase(socket_interface) {
        createMessageHandler();
    }

    TCPSocket::TCPSocket(SocketFacade *socket_interface, int socket, in6_addr address, uint16_t port, SocketState state) :
            TCPSocketBase(socket_interface, socket, address, port, state) {
        createMessageHandler();
    }

    void TCPSocket::createMessageHandler() {
        readyRead.connect([&]() {
            while(true) {
                if(privateReadMessage())
                    incommingMessage();
                else
                    break;
            }
        });
    }

    TCPSocket::TCPSocket(TCPSocket &&other) :
            TCPSocketBase(std::move(other)) {
    }

    TCPSocket &TCPSocket::operator=(TCPSocket &&other) {
        TCPSocketBase::operator=(std::move(other));
        return *this;
    }

    TCPSocket::~TCPSocket() {
    }

    void TCPSocket::writeMessage(Message& message) {
        std::string message_data = "0000" + message.toString();
        uint32_t *message_size = reinterpret_cast<uint32_t *>(&message_data[0]);
        *message_size = uint32_t(message_data.size());
        write(message_data);
    }

    std::shared_ptr<Message> TCPSocket::readMessage() {
        std::shared_ptr<Message> result = messages_.front();
        messages_.pop_front();
        return result;
    }

    std::shared_ptr<Message> TCPSocket::peekBack() {
        return messages_.back();
    }

    void TCPSocket::popBack() {
        messages_.pop_back();
    }


    int TCPSocket::availableMessages() {
        return messages_.size();
    }

    bool TCPSocket::waitForMessage(int secs) {
        FDSet fd_set;
        fd_set.addRead(getDescriptor());
        struct timeval time;
        time.tv_sec = secs;
        time.tv_usec = 0;
        while(true) {
            std::cout << getDescriptor() << " " << time.tv_sec << " " << time.tv_usec << "\n";
            socket_interface_->select(fd_set.getBiggestDescriptor() + 1, fd_set.getRead(), NULL, NULL, &time);
            if (time.tv_sec != 0 || time.tv_usec != 0) {
                if(fd_set.isSetRead(getDescriptor())) {
                    if(!readFromSocket())
                        return false;
                    else {
                        if(privateReadMessage())
                            return true;
                    }
                }
            } else {
                return false;
            }
        }
    }

    uint32_t TCPSocket::getPendingMessageSize() const {
        if(availableBytes() < 4)
            return 4;

        const uint32_t* packet_size_address = boost::asio::buffer_cast<const uint32_t*>(*buffer_.data().begin());
        return *packet_size_address;
    }

    bool TCPSocket::privateReadMessage() {
        uint32_t available = availableBytes();
        if(available >= 4 && available >= getPendingMessageSize()) {
            buffer_.consume(4);
            messages_.push_back(Message::fromBuffer(buffer_));
            return true;
        }
        return false;
    }
}