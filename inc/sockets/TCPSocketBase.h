//
// \author Jakub Gajownik
// \date 16.04.2018
//

#ifndef TIN_TCPSOCKET_H
#define TIN_TCPSOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory.h>
#include <sys/ioctl.h>
#include <boost/signals2.hpp>
#include <sstream>
#include <boost/asio/streambuf.hpp>

#include "SocketHelpers.h"
#include "SocketFacade.h"
#include "FDSet.h"

namespace Sockets {
    /*!
     * \class TCPSocketBase
     * \brief Provides basic operation for sockets
     */
    class TCPSocketBase {
    public:
        /*!
         * @param socket_interface Pointer to interface appriopriate to current platform
         */
        TCPSocketBase(SocketFacade* socket_interface);
        /*!
         * @param socket_interface Pointer to interface appriopriate to current platform
         * @param socket Socket file descriptor
         * @param state State to be set, default CONNECTED
         */
        TCPSocketBase(SocketFacade* socket_interface, int socket, SocketState state = SocketState::CONNECTED);
        /*!
         * @param socket_interface Pointer to interface appriopriate to current platform
         * @param socket Socket file descriptor
         * @param address Address to which socket is connected
         * @param port Port to which socket is connected
         * @param state State to be set, default CONNECTED
         */
        TCPSocketBase(SocketFacade* socket_interface, int socket, in6_addr address, uint16_t port, SocketState state = SocketState::CONNECTED);
        TCPSocketBase(TCPSocketBase&) = delete;
        TCPSocketBase(TCPSocketBase&& other);
        TCPSocketBase& operator=(TCPSocketBase&) = delete;
        TCPSocketBase& operator=(TCPSocketBase&& other);

        ~TCPSocketBase();

        void close();

        /*!
         * @param address Address which socket has to connect
         * @param port Port which socket has to connect
         * @return Result whether connection was succeded
         */
        bool connect(in6_addr address, uint16_t port);
        /*!
         * @param address IP address which socket has to connect
         * @param port Port which socket has to connect
         * @return Result whether connection was succeded
         */
        bool connect(IP address, uint16_t port);
        /*!
         * @param address DNS address which socket has to connect
         * @param port Port which socket has to connect
         * @return Result whether connection was succeded
         */
        bool connect(DNS address, uint16_t port);
        /*!
         * Connect to saved address and port
         * @return Result whether connection was succeded
         */
        bool connect();

        /*!
         * Blocking method which waits until the socket is connected, up to secs seconds
         * @param secs Number of seconds to wait
         * @return Result of connecting, true if connection is established, otherwise false
         */
        bool waitForConnected(int secs);

        /*!
         * @return Saved port to connect to
         */
        uint16_t port() const;
        /*!
         * @return Saved address to connect to
         */
        in6_addr address() const;

        /*!
         * Writes up to size bytes to the socket
         * @param buffer Pointer to data
         * @param size Size of data in bytes
         * @return Number of written bytes
         */
        int write(char* buffer, unsigned int size);
        /*!
         * @param buffer Sequence of character to write to the socket
         * @return Number of written bytes
         */
        int write(std::string buffer);

        /*!
         * Reads up to size butes to given buffer
         * @param buffer Pointer to data space
         * @param size Size of data to be readed
         */
        void read(char* buffer, unsigned int size);

        /*!
         * @return Number of bytes ready to be readed
         */
        int availableBytes() const;

        SocketError getError() const;
        SocketState getState() const;

        int getDescriptor() const;

        boost::signals2::signal<void ()> readyRead;
        boost::signals2::signal<void ()> connected;
        boost::signals2::signal<void ()> disconnected;

        void setConnected();
        /*!
         * Reads all available bytes to object buffer. If available bytes equals 0, close socket and run disconnected signal
         * @return Whether any bytes was readed. If false, connection was closed
         */
        bool readFromSocket();
    protected:
        boost::asio::streambuf buffer_;
        SocketFacade* socket_interface_;
    private:

        int socket_;
        SocketState state_;
        SocketError error_;

        in6_addr address_;
        uint16_t port_;

        void setState(SocketState state);
        void setError(SocketError error);

        int privateRead(char* buffer, unsigned int size);
        int socketAvailableBytes() const;
    };
}


#endif //TIN_TCPSOCKET_H
