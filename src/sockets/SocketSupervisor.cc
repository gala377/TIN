//
// Created by gajus123 on 21.04.18.
//

#include <syscall.h>
#include "../../inc/sockets/SocketSupervisor.h"


SocketSupervisor::SocketSupervisor(SocketFacade* socket_interface) :
    socket_interface_(socket_interface) {
    int descriptors[2];
    pipe(descriptors);
    pipe_input_ = descriptors[1];
    pipe_output_ = descriptors[0];
    fcntl (pipe_output_, F_SETFL, O_ASYNC | O_NONBLOCK);

    running_ = true;
    thread = new std::thread([this](){
        loop();
    });
}

SocketSupervisor::~SocketSupervisor() {
    stop();
    close(pipe_input_);
    close(pipe_output_);
    delete thread;
}

void SocketSupervisor::update() {
    const char character = ' ';
    write(pipe_input_, &character, 1);
}

void SocketSupervisor::add(TCPSocket* socket) {
    sockets_.insert(std::make_pair(socket->getDescriptor(), socket));
    update();
}

void SocketSupervisor::add(TCPServer* server) {
    servers_.insert(std::make_pair(server->getDescriptor(), server));
    update();
}

void SocketSupervisor::remove(TCPSocket* socket) {
    sockets_.erase(socket->getDescriptor());
    update();
}

void SocketSupervisor::remove(TCPServer* server) {
    servers_.erase(server->getDescriptor());
    update();
}

void SocketSupervisor::loop() {
    while(running_) {
        int biggest_descriptor = pipe_output_;
        fd_set set;
        FD_ZERO(&set);
        fd_set write_set;
        FD_ZERO(&write_set);
        std::for_each(sockets_.begin(), sockets_.end(), [&](auto socket) {
            if(socket.second->getState() != SocketState::CONNECTED) {
                if (socket.first > biggest_descriptor)
                    biggest_descriptor = socket.first;
                FD_SET(socket.first, &write_set);
            }
        });
        std::for_each(sockets_.begin(), sockets_.end(), [&](auto socket) {
            if (socket.first > biggest_descriptor)
                biggest_descriptor = socket.first;
            FD_SET(socket.first, &set);
        });
        std::for_each(servers_.begin(), servers_.end(), [&](auto server) {
            if (server.first > biggest_descriptor)
                biggest_descriptor = server.first;
            FD_SET(server.first, &set);
        });
        FD_SET(pipe_output_, &set);
        select(biggest_descriptor + 1, &set, &write_set, NULL, NULL);
        if (FD_ISSET(pipe_output_, &set)) {
            char character;
            while (true) {
                if (read(pipe_output_, &character, 1) == -1) {
                    break;
                }
            }
        }
        std::for_each(sockets_.begin(), sockets_.end(), [&](auto socket) {
            if (FD_ISSET(socket.first, &write_set)) {
                if(socket.second->getState() == SocketState::CONNECTING) {
                    socket.second->setConnected();
                }
            }
        });
        std::for_each(sockets_.begin(), sockets_.end(), [&](auto socket) {
            if (FD_ISSET(socket.first, &set)) {
                socket.second->readFromSocket();
                socket.second->readyRead();
            }
        });
        std::for_each(servers_.begin(), servers_.end(), [&](auto server) {
            if (FD_ISSET(server.first, &set)) {
                server.second->incomingConnection();
            }
        });
    }
}

void SocketSupervisor::stop() {
    if(running_) {
        running_ = false;
        update();
        thread->join();
    }
}