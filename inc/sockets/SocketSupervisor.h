//
// \author Jakub Gajownik
// \date 21.04.2018
//

#ifndef TIN_SOCKETSUPERVISOR_H
#define TIN_SOCKETSUPERVISOR_H

#include <algorithm>
#include <thread>
#include <unordered_map>
#include <csignal>

#include "TCPServer.h"
#include "TCPSocketBase.h"
#include "FDSet.h"

namespace Sockets {
    /*!
     * \class SocketSupervisor
     * \brief Separate thread socket selecting
     *
     * Run in separate thread and check whether sockets have active read/write sending appropriate signals
     */
    class SocketSupervisor {
    public:
        SocketSupervisor(SocketFacade *socket_interface);
        ~SocketSupervisor();

        void add(TCPSocketBase *socket);
        void add(TCPServer *server);

        void remove(TCPSocketBase *socket);
        void remove(TCPServer *server);
    private:
        void update();
        void loop();
        void stop();

        SocketFacade *socket_interface_;

        int pipe_input_;
        int pipe_output_;

        std::thread *thread;
        bool running_;

        std::unordered_map<int, TCPSocketBase *> sockets_;
        std::unordered_map<int, TCPServer *> servers_;
    };
}


#endif //TIN_SOCKETSUPERVISOR_H
