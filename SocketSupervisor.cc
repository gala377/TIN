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
    //TODO
}