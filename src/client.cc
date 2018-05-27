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
    std::cout << "Creating client\nPass server port, client port\n";
    int server_port, client_port;
    std::cin >> server_port >> client_port;
    auto client = CommunicationModule::createClient(server_port, Sockets::IP({"::1"}), client_port);
    std::cout << "Connecting to signal\n";
    client.incommingMessage.connect([&client](){
        std::cout << "Got message!\n";
        auto pack = client.read();
        std::cout << "Read message!\n";
        auto casted = std::dynamic_pointer_cast<MyMess>(pack);
        std::cout << "Content " << casted->_data;
    });

    std::cout << "Connection is done!\nWaiting...\n";

    char c;
    std::cin >> c;

    std::cout << "Ending!\n";
}