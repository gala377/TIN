//
// Created by gajus123 on 21.04.18.
//

#include <syscall.h>
#include "sockets/SocketSupervisor.h"

namespace Sockets {
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

    void SocketSupervisor::add(TCPSocketBase* socket) {
        sockets_.insert(std::make_pair(socket->getDescriptor(), socket));
        update();
    }

    void SocketSupervisor::add(TCPServer* server) {
        servers_.insert(std::make_pair(server->getDescriptor(), server));
        update();
    }

    void SocketSupervisor::remove(TCPSocketBase* socket) {
        for(auto it = sockets_.begin(); it != sockets_.end(); ++it) {
            if(it->second == socket) {
                sockets_.erase(it);
                break;
            }
        }
        update();
    }

    void SocketSupervisor::remove(TCPServer* server) {
        servers_.erase(server->getDescriptor());
        update();
    }

    void SocketSupervisor::loop() {
        while(running_) {
            FDSet fd_set;

            std::for_each(sockets_.begin(), sockets_.end(), [&](auto socket) {
                if(socket.second->getState() == SocketState::CONNECTING) {
                    fd_set.addWrite(socket.first);
                }
            });
            std::for_each(sockets_.begin(), sockets_.end(), [&](auto socket) {
                if(socket.second->getState() == SocketState::CONNECTED) {
                    fd_set.addRead(socket.first);
                }
            });
            std::for_each(servers_.begin(), servers_.end(), [&](auto server) {
                fd_set.addRead(server.first);
            });
            fd_set.addRead(pipe_output_);
            select(fd_set.getBiggestDescriptor() + 1, fd_set.getRead(), fd_set.getWrite(), fd_set.getExcept(), NULL);
            if(fd_set.isSetRead(pipe_output_)) {
                char character;
                while (true) {
                    if (read(pipe_output_, &character, 1) == -1) {
                        break;
                    }
                }
            }
            std::for_each(sockets_.begin(), sockets_.end(), [&](auto socket) {
                if(fd_set.isSetWrite(socket.first)) {
                    if(socket.second->getState() == SocketState::CONNECTING) {
                        socket.second->connect();
                    }
                }
            });
            for(auto it = sockets_.begin(); it != sockets_.end();) {
                if(fd_set.isSetRead(it->first)) {
                    if(!(it->second->readFromSocket())) {
                        it = sockets_.erase(it);
                        continue;
                    }
                    else {
                        it->second->readyRead();
                    }
                }
                ++it;
            }
            std::for_each(servers_.begin(), servers_.end(), [&](auto server) {
                if(fd_set.isSetRead(server.first)) {
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
}