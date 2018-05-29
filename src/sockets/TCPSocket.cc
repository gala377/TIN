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
            while (availableBytes() >= 4) {
                const uint32_t *packet_size_address = boost::asio::buffer_cast<const uint32_t *>(
                        *buffer_.data().begin());

                if (availableBytes() >= *packet_size_address) {
                    buffer_.consume(4);
                    messages_.push_back(Message::fromBuffer(buffer_));
                    packetReady();
                }
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
}