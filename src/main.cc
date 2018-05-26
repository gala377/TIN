//
// Created by gajus123 on 23.04.18.
//

#include <iostream>
#include <future>

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


int main() {
    std::cout << "Creating server\n";
    auto server = CommunicationModule::createServer(6543);
    std::cout << "Creating client\n";
    auto client = CommunicationModule::createClient(6543, Sockets::IP({"::1"}), 6542);
    std::cout << "Connecting to signal\n";
    client.incommingMessage.connect([&client](){
        std::cout << "Got message!\n";
        auto pack = client.read();
        auto casted = std::dynamic_pointer_cast<MyMess>(pack);
        std::cout << "Content " << casted->_data;
    });

    std::cout << "Connection is done!\n";

    char c;
    std::cin >> c;

    std::cout << "Sending message!\n";
    Message* mess = new MyMess("Hello World!");
    server.send(mess);
    delete mess;
    std::cout << "Mess send\nReceaving it";



}