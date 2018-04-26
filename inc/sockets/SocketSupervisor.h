//
// Created by gajus123 on 21.04.18.
//

#ifndef TIN_SOCKETSUPERVISOR_H
#define TIN_SOCKETSUPERVISOR_H

#include <algorithm>
#include <thread>
#include <unordered_map>
#include <csignal>
#include "TCPServer.h"
#include "TCPSocket.h"

class SocketSupervisor {
public:
    SocketSupervisor(SocketFacade* socket_interface);
    ~SocketSupervisor();

    void add(TCPSocket* socket);
    void add(TCPServer* server);

    void remove(TCPSocket* socket);
    void remove(TCPServer* server);
private:
    void update();
    void loop();

    void stop();

    SocketFacade* socket_interface_;

    int pipe_input_;
    int pipe_output_;

    std::thread* thread;
    bool running_;

    std::unordered_map<int, TCPSocket*> sockets_;
    std::unordered_map<int, TCPServer*> servers_;
};


#endif //TIN_SOCKETSUPERVISOR_H
