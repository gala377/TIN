//
// \authors Jakub Gajownik, Rafał Galczak
// \date 26.04.2018
//

#ifndef TIN_TCPSOCKETWITHPACKET_H
#define TIN_TCPSOCKETWITHPACKET_H

#include <deque>
#include <cinttypes>

#include "TCPSocketBase.h"
#include "../messages/Message.h"
#include "../messages/Acknowledge.h"

namespace Sockets {
    /*!
     * \class TCPSocket
     * \brief Provides basic operation for sockets and message sending/receiving
     */
    class TCPSocket : public TCPSocketBase {
    public:
        /*!
         * @param socket_interface Pointer to interface appriopriate to current platform
         */
        TCPSocket(SocketFacade *socket_interface);
        /*!
         * @param socket_interface Pointer to interface appriopriate to current platform
         * @param socket Socket file descriptor
         * @param address Address to which socket is connected
         * @param port Port to which socket is connected
         * @param state State to be set, default CONNECTED
         */
        TCPSocket(SocketFacade *socket_interface, int socket, in6_addr address, uint16_t port, SocketState state = SocketState::CONNECTED);
        TCPSocket(TCPSocket &) = delete;
        TCPSocket(TCPSocket &&other);
        TCPSocket &operator=(TCPSocket &) = delete;
        TCPSocket &operator=(TCPSocket &&other);
        ~TCPSocket();

        /*!
         * Serialize message and write it to socket
         * @param message Message to be send
         */
        int writeMessage(Message& message);

        /*!
         * @return Pointer to received message
         */
        std::shared_ptr<Message> readMessage();
        /*!
         * @return Pointer to the last received message
         */
        std::shared_ptr<Message> peekBack();
        /*!
         * Remove the last received message
         */
        void popBack();

        /*!
         * @return Number of messages available to read
         */
        int availableMessages();
        /*!
         * Blocking method whick waits until at least one message is ready to be readed, up to secs seconds
         * @param secs Number of seconds to wait
         * @return Result of waiting, true if there is new message, otherwise false
         */
        bool waitForMessage(int secs);

        boost::signals2::signal<void()> incommingMessage;
    private:
        std::deque<std::shared_ptr<Message>> messages_;
        void createMessageHandler();

        uint32_t getPendingMessageSize() const;
        bool privateReadMessage();
    };
}


#endif //TIN_TCPSOCKETWITHPACKET_H
