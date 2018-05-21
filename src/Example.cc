//
// Created by gajus123 on 23.04.18.
//

#include <iostream>
#include <future>
#include "sockets/TCPSocketBase.h"
#include "../inc/sockets/TCPServer.h"
#include "../inc/sockets/SocketSupervisor.h"
#include "../inc/sockets/SocketUnix.h"

int main() {
    SocketUnix unixSocket;
    uint16_t port = 56004;
    TCPServer server(&unixSocket);
    std::shared_ptr<TCPSocketBase> socket1;
    TCPSocketBase socket2(&unixSocket);
    char packet1[16];
    {
        uint32_t *temp = reinterpret_cast<uint32_t *>(&packet1[0]);
        *temp = uint32_t(10);
        temp = reinterpret_cast<uint32_t *>(&packet1[4]);
        *temp = uint32_t(15);
        temp = reinterpret_cast<uint32_t *>(&packet1[8]);
        *temp = uint32_t(4);
    }
    char packet2[20];
    {
        uint32_t *temp = reinterpret_cast<uint32_t*>(&packet2[0]);
        *temp = uint32_t(1);
        temp = reinterpret_cast<uint32_t*>(&packet2[4]);
        *temp = uint32_t(84);
        temp = reinterpret_cast<uint32_t*>(&packet2[8]);
        *temp = uint32_t(8);
    }
    char packet3[23];
    {
        uint32_t *temp = reinterpret_cast<uint32_t *>(&packet3[0]);
        *temp = uint32_t(5);
        temp = reinterpret_cast<uint32_t*>(&packet3[4]);
        *temp = uint32_t(13215);
        temp = reinterpret_cast<uint32_t*>(&packet3[8]);
        *temp = uint32_t(11);
    }
    auto test = std::async(std::launch::async, [&](){
        server.listen(port);
        server.waitForConnection(7000);
        socket1 = server.accept();
        sleep(1);
    });
    sleep(1);
    socket2.connect(IP({"::1"}), port);
    std::cout << "Port " << socket2.port() << "\n";
    int a;
    std::cin >> a;
    sleep(1);

    int i = 0;
    while(i++ < 1) {
        socket1->write(packet1, 16);
        sleep(2);
        socket1->write(packet2, 20);
        sleep(2);
        socket1->write(packet3, 23);
        sleep(2);
    }

    server.close();
    socket1->close();
    socket2.close();
}