#include "system/CommunicationModule.h"

CommunicationModule::CommunicationModule(uint16_t port, std::string mess_dir_name) :
        socket_facade_(),
        server_(&socket_facade_),
        supervisor_(&socket_facade_),
        state_(State::UNCONNECTED),
        mess_dir_name_(mess_dir_name),
        queue_(mess_dir_name) {
    server_.listen(port);
    supervisor_.add(&server_);
    server_.incomingConnection.connect([=]() {
        // todo which messege id should we use?
        if (state_ == State::UNCONNECTED) {
            std::cout << "Connected now\n";
            socket_ = server_.accept();
            prepareSocket();
            state_ = State::CONNECTED;
            retransmitMessages();
        } else if (state_ == State::CONNECTED) {
            std::cout << "New connection - refused\n";
            auto temp_socket = server_.accept();
            temp_socket->close();
        } else if (state_ == State::DISCONNECTED) {
            std::cout << "New connection - accepted\n";
            socket_ = server_.accept();
            prepareSocket();
            state_ = State::CONNECTED;
            retransmitMessages();
        }
    });
}


std::string CommunicationModule::getMessDirName() const {
    return mess_dir_name_;
}

std::uint32_t CommunicationModule::getMessageInStorageCount() const {
    return queue_.cachedFilesCount();
}


void CommunicationModule::retransmitMessages() {
    for (auto it: queue_) {
        socket_->writeMessage(*it);
    }
}

void CommunicationModule::prepareSocket() {
    std::cout << "Preparing sockets\n";
    socket_->packetReady.connect([=]() {
        std::cout << "Incomming message\n";
        std::shared_ptr<Message> mess = socket_->peekBack();
        auto ack = std::dynamic_pointer_cast<Acknowledge>(mess);
        if(ack != nullptr) {
            std::cout << "Ack received for id " << ack->getConsumedPacketId() << "\n";
            socket_->popBack();
            queue_.remove(ack->getConsumedPacketId());
        } else if(already_ack_.find(mess->id_) != already_ack_.end()) {
            Acknowledge retransmit_ack(mess->id_);
            socket_->popBack();
            socket_->writeMessage(retransmit_ack);
        } else {
            incommingMessage();
        }

    });
    socket_->disconnected.connect([=]() {
        std::cout << "Disconnected\n";
        state_ = State::DISCONNECTED;
    });
    supervisor_.add(socket_.get());
}

CommunicationModule::~CommunicationModule() {
    std::cout << "Communication destructor\n";
    server_.close();
    if(socket_.get()) {
        socket_->close();
    }
}


CommunicationModule CommunicationModule::createServer(uint16_t port, std::string mess_dir_name) {
    CommunicationModule result(port, std::move(mess_dir_name));
    return result;
}

CommunicationModule CommunicationModule::createClient(uint16_t server_port,
                                                      Sockets::IP address, uint16_t port,
                                                      std::string mess_dir_name) {
    CommunicationModule result(port, std::move(mess_dir_name));
    result.socket_ = std::make_shared<Sockets::TCPSocket>(&result.socket_facade_);
    result.socket_->connect(address, server_port);
    if (result.socket_->waitForConnected(30)) {
        result.prepareSocket();
        result.retransmitMessages();
    } else {
        throw System::CanNotConnect();
    }
    return result;
}


CommunicationModule::CommunicationModule(CommunicationModule &&other) :
        server_(std::move(other.server_)),
        socket_(std::move(other.socket_)),
        supervisor_(other.supervisor_),
        queue_(other.mess_dir_name_) {
}

CommunicationModule &CommunicationModule::operator=(CommunicationModule &&other) {
    server_ = std::move(other.server_);
    socket_ = std::move(other.socket_);
    supervisor_ = std::move(other.supervisor_);
    queue_= std::move(other.queue_);
}


std::shared_ptr<Message> CommunicationModule::read() {
    if (!socket_->availableMessages()) {
        // todo custom exception
        throw System::NoMessageToRead();
    }
    std::shared_ptr<Message> receivedMess = socket_->readMessage();
    return receivedMess;
}

void CommunicationModule::send(Message *mess) {
    queue_.add(*mess);
    std::cout << "Last send mess is: " << queue_.lastAddedId() << "\n";
    if(socket_->writeMessage(*mess) <0){
        throw System::UnableToSentMessageClosedConnection();
    };
}

void CommunicationModule::acknowledge(Message* mess) {
    if(already_ack_.find(mess->id_) != already_ack_.end()) {
        // todo should it really? maybe just ignore it?
        //throw std::runtime_error("Trying to ack the same mess twice");
        return;
    }
    Acknowledge ack(mess->id_);
    socket_->writeMessage(ack);
    already_ack_.insert(mess->id_);
}
