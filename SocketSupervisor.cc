//
// Created by gajus123 on 21.04.18.
//

#include "SocketSupervisor.h"


SocketSupervisor::SocketSupervisor() {
    int descriptors[2];
    pipe(descriptors);
    pipe_input_ = descriptors[1];
    pipe_output_ = descriptors[0];
}

SocketSupervisor::~SocketSupervisor() {
    close(pipe_input_);
    close(pipe_output_);
}

void SocketSupervisor::add(TCPSocket* socket) {
    //TODO
}

void SocketSupervisor::add(TCPServer* server) {
    //TODO
}

void SocketSupervisor::remove(TCPSocket* socket) {
    //TODO
}

void SocketSupervisor::remove(TCPServer* server) {
    //TODO
}

void SocketSupervisor::run() {
    while(true) {
        int bigger_descriptor = 0;
        fd_set set;
        FD_ZERO(&set);
        std::for_each(sockets_.begin(), sockets_.end(), [](TCPSocket* socket) {
            if(socket->getDescriptor() >= 0) {
                FD_SET(socket->getDescriptor(), &set);
            }
        });
        std::for_each(servers_.begin(), servers_.end(), [](TCPServer* server) {
            if(server->getDescriptor() >= 0) {
                FD_SET(server->getDescriptor(), &set);
            }
        });
        FD_SET(pipe_output_, &set);
        select(bigger_descriptor+1, &set, NULL, NULL, NULL);
        std::for_each(sockets_.begin(), sockets_.end(), [](TCPSocket* socket) {
            if(socket->getDescriptor() >= 0) {
                if(FD_ISSET(socket->getDescriptor(), &set)) {
                    std::cout << "Incoming data\n";
                    //TODO
                }
            }
        });
        std::for_each(servers_.begin(), servers_.end(), [](TCPServer* server) {
            if(server->getDescriptor() >= 0) {
                if(FD_ISSET(server->getDescriptor(), &set)) {
                    std::cout << "Incoming connection\n";
                    //TODO
                }
            }
        });
    }
}

void SocketSupervisor::stop() {
    //TODO
}