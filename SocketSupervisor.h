//
// Created by gajus123 on 21.04.18.
//

#ifndef TIN_SOCKETSUPERVISOR_H
#define TIN_SOCKETSUPERVISOR_H

#include <unordered_map>
#include "TCPServer.h"
#include "TCPSocket.h"

class SocketSupervisor {
public:
    SocketSupervisor();
    ~SocketSupervisor();
    void add(TCPSocket* socket);
    void add(TCPServer* server);
    void remove(TCPSocket* socket);
    void remove(TCPServer* server);
private:
    int pipe_input_;
    int pipe_output_;
    std::unordered_map<int, TCPSocket*> sockets_;
    std::unordered_map<int, TCPServer*> servers_;
};


#endif //TIN_SOCKETSUPERVISOR_H
