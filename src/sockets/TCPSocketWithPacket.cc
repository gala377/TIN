//
// Created by gajus123 on 26.04.18.
//

#include "sockets/TCPSocketWithPacket.h"

TCPSocketWithPacket::TCPSocketWithPacket(SocketFacade* socket_interface) :
    TCPSocket(socket_interface) {
    createMessageHandler();
}

TCPSocketWithPacket::TCPSocketWithPacket(SocketFacade* socket_interface, int socket, SocketState state) :
    TCPSocket(socket_interface, socket, state) {
    createMessageHandler();
}

void TCPSocketWithPacket::createMessageHandler() {
    readyRead.connect([&]() {
        while(availableBytes() >= 4) {
            const uint32_t* packet_size_address = boost::asio::buffer_cast<const uint32_t*>(*buffer_.data().begin());

            if(availableBytes() >= *packet_size_address) {
                buffer_.consume(4);
                messages_.push(Message::fromBuffer(buffer_));
                packetReady();
            }
        }
    });
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

void TCPSocketWithPacket::writePacket(Message* message) {
    std::string message_data = "0000" + message->toString();
    uint32_t *message_size = reinterpret_cast<uint32_t*>(&message_data[0]);
    *message_size = uint32_t(message_data.size());
    write(message_data);
}

Message* TCPSocketWithPacket::readPacket() {
    Message* result = messages_.front();
    messages_.pop();
    return result;
}

int TCPSocketWithPacket::availablePackets() {
    return messages_.size();
}