//
// \author Jakub Gajownik
// \date 17.04.2018
//

#ifndef TIN_TCPSERVER_H
#define TIN_TCPSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory.h>
#include <sys/ioctl.h>
#include <memory>
#include <boost/signals2.hpp>

#include "TCPSocket.h"

namespace Sockets {
    /*!
     * \class TCPServer
     * \brief Provides basic operation for servers
     */
    class TCPServer {
    public:
        /*!
         * @param socket_interface Pointer to interface appriopriate to current platform
         */
        TCPServer(SocketFacade *socket_interface);
        /*!
         * @param socket_interface Pointer to interface appriopriate to current platform
         * @param socket Socket file descriptor
         */
        TCPServer(SocketFacade *socket_interface, int socket);
        TCPServer(TCPServer &) = delete;
        TCPServer(TCPServer &&other);
        TCPServer &operator=(TCPServer &) = delete;
        TCPServer &operator=(TCPServer &&other);
        ~TCPServer();

        /*!
         * Closes socket and connection is established
         */
        void close();

        /*!
         * @param address Address to listen on
         * @param server_port Port to listen on
         * @return
         */
        bool listen(in6_addr address, uint16_t server_port);
        /*!
         * @param server_port Port to listen on
         * @return
         */
        bool listen(uint16_t server_port);
        /*!
         * @param address IP address to listen on
         * @param server_port Port to listen on
         * @return
         */
        bool listen(IP address, uint16_t server_port);
        /*!
         * @param address DNS address to listen on
         * @param server_port Port to listen on
         * @return
         */
        bool listen(DNS address, uint16_t server_port);

        /*!
         * Blocking method whick waits until incomming connection, up to secs seconds
         * @param secs Number of seconds to wait
         * @return Result of waiting, true if there is incomming connection, otherwise false
         */
        bool waitForConnection(int secs);

        /*!
         * @return Pointer to connected socket
         */
        std::shared_ptr<TCPSocket> accept();

        /*!
         * @return Port on which server is listening on
         */
        uint16_t port() const;

        int getDescriptor() const;

        boost::signals2::signal<void()> incomingConnection;
    private:
        SocketFacade *socket_interface_;

        int socket_;
        bool closed_;
        SocketError error_;

        void setError(SocketError error);
    };
}


#endif //TIN_TCPSERVER_H
