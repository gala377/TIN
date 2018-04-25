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
    SocketSupervisor();
    ~SocketSupervisor();

    void add(TCPSocket* socket);
    void add(TCPServer* server);

    void remove(TCPSocket* socket);
    void remove(TCPServer* server);


    static void sigioHandle(int signum);
private:
    void update();
    void loop();

    void stop();

    void checkSockets();

    int pipe_input_;
    int pipe_output_;

    pid_t process_pid_;

    std::thread* thread;
    bool running_;

    std::unordered_map<int, TCPSocket*> sockets_;
    std::unordered_map<int, TCPServer*> servers_;

    static std::unordered_map<pid_t , SocketSupervisor*> supervisors_;
};


#endif //TIN_SOCKETSUPERVISOR_H
