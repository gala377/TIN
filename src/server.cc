//
// Created by igor on 28.05.18.
//

//
// Created by gajus123 on 23.04.18.
//

#include <iostream>
#include <future>
#include <unistd.h>

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


int main() {
    std::cout << "Creating server\nChoose port for server\n";
    int port;
    std::cin >> port;
    auto server = CommunicationModule::createServer(port);

    std::cout << "Connection is done!\nType to send a mesage!\n";

    char c;
    std::cin >> c;

    std::cout << "Sending message!\n";
    Message* mess = new MyMess("Hello World!");
    server.send(mess);
    delete mess;
    std::cout << "Mess send\nWaiting...\n";

    std::cin >> c;

}