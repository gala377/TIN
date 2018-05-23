#include "system/CommunicationModule.h"

CommunicationModule::CommunicationModule(uint16_t port) :
        socket_facade_(),
        server_(&socket_facade_),
        supervisor_(&socket_facade_),
        state_(State::UNCONNECTED) {
    server_.listen(port);
    supervisor_.add(&server_);
    server_.incomingConnection.connect([=](){
        if(state_ == State::UNCONNECTED) {
            std::cout << "Connected now\n";
            socket_ = server_.accept();
            prepareSocket();
            state_ = State::CONNECTED;
        }
        else if(state_ == State::CONNECTED) {
            std::cout << "New connection - refused\n";
            auto temp_socket = server_.accept();
            temp_socket->close();
        }
        else if(state_ == State::DISCONNECTED) {
            std::cout << "New connection - accepted\n";
            socket_ = server_.accept();
            prepareSocket();
            state_ = State::CONNECTED;
            //TODO retransmit
        }
    });
}

void CommunicationModule::prepareSocket() {
    socket_->packetReady.connect([=](){
        std::cout << "Incomming message\n";
        incommingMessage();
    });
    socket_->disconnected.connect([=](){
        std::cout << "Disconnected\n";
        state_ = State::DISCONNECTED;
    });
    supervisor_.add(socket_.get());
}

CommunicationModule::~CommunicationModule() {
    std::cout << "Communication destructor\n";
    server_.close();
    if(socket_.get())
        socket_->close();
}

CommunicationModule CommunicationModule::createServer(uint16_t port) {
    CommunicationModule result(port);
    return result;
}

CommunicationModule CommunicationModule::createClient(uint16_t port, Sockets::IP address, uint16_t server_port) {
    CommunicationModule result(server_port);
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