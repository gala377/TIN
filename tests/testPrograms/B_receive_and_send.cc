//
// Created Martyna Kania on 30.05.18.
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
    sleep(1);
    std::cout << "B creating server\n";
    auto server = CommunicationModule::createServer(5612);
    auto client = CommunicationModule::createClient(5610, Sockets::IP({"::1"}), 5611);
    std::cout << "Connecting to signal\n";
    client.incommingMessage.connect([&client](){
        std::cout << " B Got message!\n";
        auto pack = client.read();
        server.send(pack.get());
        auto casted = std::dynamic_pointer_cast<MyMess>(pack);
        std::cout << " B send message " << casted->_data;
    });
}