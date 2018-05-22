#include "system/CommunicationModule.h"

CommunicationModule::CommunicationModule(uint16_t port) :
    socket_facade_(),
    server_(&socket_facade_),
    supervisor_(&socket_facade_) {
    server_.listen(port);
    //TODO setup business logic
}

CommunicationModule::~CommunicationModule() {
    server_.close();
    if(socket_.get())
        socket_->close();
}

CommunicationModule CommunicationModule::createServer(uint16_t port) {
    CommunicationModule result(port);
    return result;
}

CommunicationModule CommunicationModule::createClient(uint16_t port, Sockets::IP address) {
    CommunicationModule result(port);
    result.socket_ = std::make_shared<Sockets::TCPSocket>(&result.socket_facade_);
    result.socket_->connect(address, port);
    return result;
}

CommunicationModule::CommunicationModule(CommunicationModule&& other) :
    server_(std::move(other.server_)),
    socket_(std::move(other.socket_)),
    supervisor_(std::move(other.supervisor_)) {
}

CommunicationModule& CommunicationModule::operator=(CommunicationModule&& other) {
    server_ = std::move(other.server_);
    socket_ = std::move(other.socket_);
    supervisor_ = std::move(other.supervisor_);
}