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


int main() {
    int clientPort, otherServerPort;
    std::cout << "Creating receiver (C)\nChoose port for receiver client\n";
    std::cin >> clientPort;
    std::cout << "Choose port for middleMan server\n";
    std::cin >> otherServerPort;
    std::cout << "Give module dir path\n";
    std::string dir;
    std::cin >> dir;

    auto client = CommunicationModule::createClient(otherServerPort, Sockets::IP({"::1"}), clientPort, dir);
    sleep(2);

    std::queue<std::shared_ptr<MyMess>> stored;
    client.incommingMessage.connect([&client, &stored](){
        std::cout << "Got message!\n";
        auto pack = client.read();
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
            client.acknowledge(mess.get());
        } else {
            std::string data = "Message by char: ";
            data += c;
            MyMess mess(data);
            client.send(&mess);
            std::cout << "Mess " << c << " send.\n";
        }
    }
    std::cout << "Closing...";
}