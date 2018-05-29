//
// Created by igor on 29.05.18.
//

#include <iostream>
#include <future>
#include <unistd.h>
#include <queue>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/export.hpp>
#include <boost/asio/streambuf.hpp>

#include <messages/Message.h>
#include <system/CommunicationModule.h>
#include <sockets/SocketHelpers.h>


class MyMess: public Message {
public:
    MyMess(): _data("") {}
    MyMess(std::string data): _data(std::move(data)) {}
    std::string _data;

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Message>(*this);
        ar & _data;
    }
};

BOOST_CLASS_EXPORT(MyMess)

CommunicationModule getModule(char option) {
    if(option == 's') {
        std::cout << "Creating server\nChoose port for server\n";
        int port;
        std::cin >> port;
        std::cout << "Comm module temp dir?\n";
        std::string temp_dir;
        std::cin >> temp_dir;
        return CommunicationModule::createServer(port, temp_dir);
    } else if(option == 'c') {
            std::cout << "Creating client\nPass server port, client port\n";
        int server_port, client_port;
        std::cin >> server_port >> client_port;
        std::cout << "Comm module temp dir?\n";
        std::string temp_dir;
        std::cin >> temp_dir;
        return CommunicationModule::createClient(server_port, Sockets::IP({"::1"}), client_port, temp_dir);
    } else {
        throw std::runtime_error("Option not supported.");
    }
}

int main() {
    std::cout << "Client (c) or server (s)?\n"; 
    char c;
    std::cin >> c;
    auto server = getModule(c);
    std::cout << "Connection is done!\n";
    std::queue<std::shared_ptr<MyMess>> stored;
    std::cout << "Connecting to signal\n";
    server.incommingMessage.connect([&server, &stored](){
        std::cout << "Got message!\n";
        auto pack = server.read();
        std::cout << "Read message!\n";
        auto casted = std::dynamic_pointer_cast<MyMess>(pack);
        std::cout << "Content " << casted->_data << "\n";
        std::cout << "Adding to queue.\n";
        stored.push(casted);
    });
    while(true) {
        std::cout << "Q to quit. A to ack. Whatever else to send a mess.\n";
        char c;
        std::cin >> c;
        if(c == 'Q') {
            break;
        } else if(c == 'A') {
            auto mess = stored.front();
            stored.pop();
            server.acknowledge(mess.get());
        } else {
            std::string data = "Message by char: ";
            data += c;
            MyMess mess(data);
            server.send(&mess);
            std::cout << "Mess " << c << " send.\n";
        }
    }
    std::cout << "Closing...";
}