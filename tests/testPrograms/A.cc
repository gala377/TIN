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
#include <queue>


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
    std::cout << "Creating sender(A) \nChoose port for sender server\n";
    int port;
    std::cin >> port;
    std::cout << "Give module dir path\n";
    std::string dir;
    std::cin >> dir;
    auto server = CommunicationModule::createServer(port, dir);

    std::queue<std::shared_ptr<MyMess>> stored;
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