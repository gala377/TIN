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

#include <system/CommunicationModule.h>
#include <sockets/SocketHelpers.h>

#include "MyMess.h"


CommunicationModule initServer() {
    int serverPort;
    std::cout << "Creating MiddleMan (B)\nChoose port for MiddleMan server\n";
    std::cin >> serverPort;
    std::cout << "Choose module dir path\n1: ";
    std::string dir0;
    std::cin >> dir0;
   return CommunicationModule::createServer(serverPort, dir0);
}
CommunicationModule initClient(){
    int clientPort, otherServerPort;
    std::cout << "Choose port for MiddleMan client\n";
    std::cin >> clientPort;
    std::cout << "Choose second server (sender) port\n";
    std::cin >> otherServerPort;
    std::cout << "Choose second dir path\n1: ";
    std::string dir1;
    std::cin >> dir1;

    return CommunicationModule::createClient(otherServerPort, Sockets::IP({"::1"}), clientPort, dir1);
}
int main(){

    std::queue<std::shared_ptr<MyMess>> storedFromReceiver;
    std::queue<std::shared_ptr<MyMess>> storedFromSender;
    auto server = initServer();
    auto client = initClient();

    server.incommingMessage.connect([&server, &client](){
        std::cout << "Got message from receiver!\n";
        auto pack = server.read();
        server.acknowledge(pack.get());
        auto casted = std::dynamic_pointer_cast<MyMess>(pack);
        std::cout << "Content " << casted->_data << "\n";
        std::cout << "Send receivedMessage to sender.\n";
        client.send(pack.get());
    });
    client.incommingMessage.connect([&client, &server](){
        std::cout << "Got message from sender!\n";
        auto pack = client.read();
        client.acknowledge(pack.get());
        auto casted = std::dynamic_pointer_cast<MyMess>(pack);
        std::cout << "Content " << casted->_data << "\n";
        std::cout << "Send receivedMessage to receiver.\n";
        server.send(pack.get());
    });
    while(true) {
        std::cout << "Q to quit.\n";
        char c;
        std::cin >> c;
        if(c == 'Q') {
            break;
        }
    }
    std::cout << "Closing...";
}