//
// Created by Martyna on 27.05.18.
//

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <queue/Exceptions.h>
#include <pthread.h>
#include "../../inc/system/CommunicationModule.h"
#include "TestMessage.h"

#define PATH "./gryphon/"
#define PATH0 "./gryphonTest0/"
#define PATH1 "./gryphonTest1/"

using namespace Queue;

TEST(CommunicationTest, ClientAndServerEstablishConnectionsWithoutExceptionsWithDefaultDictionaries) {
    ASSERT_NO_THROW(
            CommunicationModule server = CommunicationModule::createServer(5614);
            CommunicationModule client = CommunicationModule::createClient(5614, Sockets::IP({"::1"}), 5615);
    );
}

TEST(CommunicationTest, ClientAndServerEstablishConnectionsWithoutExceptionsWithCustomDictionaries) {
    ASSERT_NO_THROW(
            CommunicationModule server = CommunicationModule::createServer(5614, PATH0);
            CommunicationModule client = CommunicationModule::createClient(5614, Sockets::IP({"::1"}), 5615, PATH1);
    );
}

TEST(CommunicationTest, ReceiveMessageWithDefaultDictionaries) {

    boost::filesystem::remove_all(PATH);
    CommunicationModule server = CommunicationModule::createServer(5616);
    CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617);
    TestMess *sendMess = new TestMess("first mss");
    client.send(sendMess);
    sleep(1);
    std::shared_ptr<Message> receivedMess = server.read();

    ASSERT_EQ(sendMess->id_, receivedMess.get()->id_);
    ASSERT_EQ(sendMess->_data, static_cast<TestMess *>(receivedMess.get())->_data);

    delete sendMess;
    boost::filesystem::remove_all(PATH);
}

TEST(CommunicationTest, ReceiveMessageWithCustomDictionaries) {

    CommunicationModule server = CommunicationModule::createServer(5616, PATH0);
    CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617, PATH1);
    TestMess *sendMess = new TestMess("first mss");
    client.send(sendMess);
    sleep(1);
    std::shared_ptr<Message> receivedMess = server.read();

    ASSERT_EQ(sendMess->id_, receivedMess.get()->id_);
    ASSERT_EQ(sendMess->_data, static_cast<TestMess *>(receivedMess.get())->_data);

    delete sendMess;
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, AfterServerReceivedMessageClientRemovesItFromStorage) {

    CommunicationModule server = CommunicationModule::createServer(5616, PATH0);
    sleep(1);
    CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617, PATH1);
    TestMess *sendMess = new TestMess("first mss");
    client.send(sendMess);
    sleep(1);
    std::shared_ptr<Message> receivedMess = server.read();
    sleep(3);

    ASSERT_EQ(0, client.getMessageInStorageCount());

    delete sendMess;
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}
