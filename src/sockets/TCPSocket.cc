//
// Created by gajus123 on 26.04.18.
//

#include "sockets/TCPSocket.h"

namespace Sockets {
    TCPSocket::TCPSocket(SocketFacade *socket_interface) :
            TCPSocketBase(socket_interface) {
        createMessageHandler();
    }

    TCPSocket::TCPSocket(SocketFacade *socket_interface, int socket, SocketState state) :
            TCPSocketBase(socket_interface, socket, state) {
        createMessageHandler();
    }

    void TCPSocket::createMessageHandler() {
        readyRead.connect([&]() {
            while (availableBytes() >= 4) {
                const uint32_t *packet_size_address = boost::asio::buffer_cast<const uint32_t *>(
                        *buffer_.data().begin());

                if (availableBytes() >= *packet_size_address) {
                    buffer_.consume(4);
                    messages_.push(Message::fromBuffer(buffer_));
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

    void TCPSocket::writePacket(Message *message) {
        std::string message_data = "0000" + message->toString();
        uint32_t *message_size = reinterpret_cast<uint32_t *>(&message_data[0]);
        *message_size = uint32_t(message_data.size());
        write(message_data);
    }

    Message *TCPSocket::readPacket() {
        Message *result = messages_.front();
        messages_.pop();
        return result;
    }

    int TCPSocket::availablePackets() {
        return messages_.size();
    }
}