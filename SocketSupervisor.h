//
// Created by gajus123 on 21.04.18.
//

#ifndef TIN_SOCKETSUPERVISOR_H
#define TIN_SOCKETSUPERVISOR_H

#include "TCPServer.h"
#include "TCPSocket.h"

class SocketSupervisor {
public:
    void add(TCPSocket* socket);
    void add(TCPServer* server);
    void remove(TCPSocket* socket);
    void remove(TCPServer* server);
};


#endif //TIN_SOCKETSUPERVISOR_H
