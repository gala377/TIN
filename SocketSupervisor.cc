//
// Created by gajus123 on 21.04.18.
//

#include <syscall.h>
#include "SocketSupervisor.h"


SocketSupervisor::SocketSupervisor() {
    int descriptors[2];
    pipe(descriptors);
    pipe_input_ = descriptors[1];
    pipe_output_ = descriptors[0];
    fcntl (pipe_output_, F_SETFL, O_ASYNC | O_NONBLOCK);

    running_ = true;
    thread = new std::thread([this](){
        signal(SIGIO, SocketSupervisor::sigioHandle);
        process_pid_ = syscall(SYS_gettid);
        supervisors_.insert(std::make_pair(process_pid_, this));
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
    ioctl(socket->getDescriptor(), SIOCSPGRP, &process_pid_);
    int on = 1;
    ioctl(socket->getDescriptor(), FIOASYNC, &on);
}

void SocketSupervisor::add(TCPServer* server) {
    servers_.insert(std::make_pair(server->getDescriptor(), server));
    ioctl(server->getDescriptor(), SIOCSPGRP, &process_pid_);
    int on = 1;
    ioctl(server->getDescriptor(), FIOASYNC, &on);
}

void SocketSupervisor::remove(TCPSocket* socket) {
    sockets_.erase(socket->getDescriptor());
    //update();
    //TODO remove signal
}

void SocketSupervisor::remove(TCPServer* server) {
    servers_.erase(server->getDescriptor());
    //update();
    //TODO remove signal
}

void SocketSupervisor::loop() {
    while(running_) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(pipe_output_, &set);
        select(pipe_output_ + 1, &set, NULL, NULL, NULL);
        if (FD_ISSET(pipe_output_, &set)) {
            char character;
            while (true) {
                if (read(pipe_output_, &character, 1) == -1) {
                    break;
                }
            }
        }
    }
}

void SocketSupervisor::stop() {
    if(running_) {
        running_ = false;
        update();
        thread->join();
    }
}

void SocketSupervisor::checkSockets() {
    int biggest_descriptor = 0;
    fd_set set;
    FD_ZERO(&set);
    std::for_each(sockets_.begin(), sockets_.end(), [&](auto socket) {
        if(socket.first >= 0) {
            if(socket.first > biggest_descriptor)
                biggest_descriptor = socket.first;
            FD_SET(socket.first, &set);
        }
    });
    std::for_each(servers_.begin(), servers_.end(), [&](auto server) {
        if(server.first >= 0) {
            if(server.first > biggest_descriptor)
                biggest_descriptor = server.first;
            FD_SET(server.first, &set);
        }
    });
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    select(biggest_descriptor+1, &set, NULL, NULL, &timeout);
//TODO error handling
    std::for_each(sockets_.begin(), sockets_.end(), [&](auto socket) {
        if(socket.first >= 0) {
            if(FD_ISSET(socket.first, &set)) {
                socket.second->readyRead();
//TODO
            }
        }
    });
    std::for_each(servers_.begin(), servers_.end(), [&](auto server) {
        if(server.first >= 0) {
            if(FD_ISSET(server.first, &set)) {
                server.second->incomingConnection();
//TODO
            }
        }
    });
}

void SocketSupervisor::sigioHandle(int signum) {
    pid_t temp = syscall(SYS_gettid);
    SocketSupervisor::supervisors_[temp]->checkSockets();
}

std::unordered_map<pid_t , SocketSupervisor*> SocketSupervisor::supervisors_;