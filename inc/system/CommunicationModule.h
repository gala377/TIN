//
// Created by gajus123 on 16.04.18.
//

#ifndef TIN_COMMUNICATIONMODULE_H
#define TIN_COMMUNICATIONMODULE_H

#include <memory>

#include "sockets/TCPServer.h"
#include "sockets/TCPSocket.h"
#include "sockets/SocketSupervisor.h"
#include "sockets/SocketUnix.h"
#include "queue/FileStorage.h"

static const std::string default_mess_dir_name_ = "gryphon" ;
class CommunicationModule {
public:
    CommunicationModule(CommunicationModule& other) = delete;
    CommunicationModule(CommunicationModule&& other);

    CommunicationModule& operator=(CommunicationModule& other) = delete;
    CommunicationModule& operator=(CommunicationModule&& other);
    ~CommunicationModule();

    static CommunicationModule createServer(uint16_t port,
                                            std::string mess_dir_name = default_mess_dir_name_);
    static CommunicationModule createClient(uint16_t server_port,
                                            Sockets::IP address, uint16_t port,
                                            std::string mess_dir_name = default_mess_dir_name_);

    std::string getMessDirName() const;
    std::uint32_t getMessageInStorageCount() const;

    std::shared_ptr<Message> read();
    void send(Message* mess);
    void acknowledge(Message* mess);


    boost::signals2::signal<void()> incommingMessage;
private:
    CommunicationModule(uint16_t port, std::string mess_dir_name = default_mess_dir_name_);

    void prepareSocket();
    void retransmitMessages();

    Sockets::SocketUnix socket_facade_;
    Sockets::TCPServer server_;
    std::shared_ptr<Sockets::TCPSocket> socket_;
    Sockets::SocketSupervisor supervisor_;

    enum class State {
        UNCONNECTED, CONNECTED, DISCONNECTED
    } state_;

    Queue::FileStorage queue_;
    // todo if needed make it random generated
    std::string mess_dir_name_;

    std::set<std::uint32_t> already_ack_;
};

#endif //TIN_COMMUNICATIONMODULE_H