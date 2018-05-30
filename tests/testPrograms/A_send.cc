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
    std::cout << "A creating server\n";
    auto server = CommunicationModule::createServer(5610);
    sleep(3);

    Message* mess = new MyMess("Hello World!");
    server.send(mess);
    delete mess;
}