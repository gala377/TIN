//
// Created Martyna Kania on 30.05.18.
//

#include <queue>
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


int main(){

    int serverPort, clientPort, otherServerPort;
    std::cout << "Creating MiddleMan (B)\nChoose port for MiddleMan server\n";
    std::cin >> serverPort;
    std::cout << "Choose port for MiddleMan client\n";
    std::cin >> clientPort;

    std::cout << "Choose second server (sender) port\n";
    std::cin >> otherServerPort;
    std::cout << "Give dwo dir paths\n1: ";
    std::string dir0, dir1;
    std::cin >> dir0;
    std::cout << "2: ";
    std::cin >> dir1;

    std::queue<std::shared_ptr<MyMess>> storedFromReceiver;
    std::queue<std::shared_ptr<MyMess>> storedFromSender;
    auto server = CommunicationModule::createServer(serverPort, dir0);
    sleep(2);
    auto client = CommunicationModule::createClient(otherServerPort, Sockets::IP({"::1"}), clientPort, dir1);

    server.incommingMessage.connect([&server, &client, &storedFromReceiver](){
        std::cout << "Got message from receiver!\n";
        auto pack = server.read();
        auto casted = std::dynamic_pointer_cast<MyMess>(pack);
        std::cout << "Content " << casted->_data << "\n";
        std::cout << "Send receivedMessage to sender.\n";
        client.send(pack.get());

        std::cout << "Adding to queue.\n";
        storedFromReceiver.push(casted);
    });
    client.incommingMessage.connect([&client, &server, &storedFromSender](){
        std::cout << "Got message from sender!\n";
        auto pack = client.read();
        auto casted = std::dynamic_pointer_cast<MyMess>(pack);
        std::cout << "Content " << casted->_data << "\n";
        std::cout << "Send receivedMessage to receiver.\n";
        server.send(pack.get());

        std::cout << "Adding to queue.\n";
        storedFromSender.push(casted);
    });
    while(true) {
        std::cout << "Q to quit. R to ack to receiver, S send ack to sender\n";
        char c;
        std::cin >> c;
        if(c == 'Q') {
            break;
        } else if(c == 'R') {
            auto mess = storedFromReceiver.front();
            storedFromReceiver.pop();
            server.acknowledge(mess.get());
        }else if(c == 'S') {
            auto mess = storedFromSender.front();
            storedFromSender.pop();
            client.acknowledge(mess.get());
        }
    }
    std::cout << "Closing...";
}