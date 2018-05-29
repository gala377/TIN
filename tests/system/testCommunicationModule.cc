//
// Created by Martyna on 27.05.18.
//

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <queue/Exceptions.h>
#include <pthread.h>
#include "../../inc/system/CommunicationModule.h"
#include "TestMessage.h"

#define PATH "./gryphon"
#define PATH0 "./gryphonTest0"
#define PATH1 "./gryphonTest1"

//TODO define port numbers

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

TEST(CommunicationTest, EstablishConnectionClientFirst) {
    ASSERT_THROW({
                     CommunicationModule client = CommunicationModule::createClient(5614, Sockets::IP({"::1"}), 5615);
                     sleep(1);
                     CommunicationModule server = CommunicationModule::createServer(5614);
                 }, std::exception
    );
}

TEST(CommunicationTest, EstablishConnectionServerFirst) {
    ASSERT_NO_THROW(
            CommunicationModule server = CommunicationModule::createServer(5614, PATH0);
            sleep(1);
            CommunicationModule client = CommunicationModule::createClient(5614, Sockets::IP({"::1"}), 5615, PATH1);
    );
}

TEST(CommunicationTest, WhenServerReadingEmptyQueueExceptionIsThrown) {
    CommunicationModule server = CommunicationModule::createServer(5618, PATH0);
    CommunicationModule client = CommunicationModule::createClient(5618, Sockets::IP({"::1"}), 5619);
    sleep(1);
    ASSERT_THROW(server.read(), std::exception);
}

TEST(CommunicationTest, WhenClientReadingEmptyQueueExceptionIsThrown) {

    CommunicationModule server = CommunicationModule::createServer(5616, PATH0);
    CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617);
    sleep(1);
    ASSERT_THROW(client.read(), std::exception);
}

TEST(CommunicationTest, OnlyOneClientCanConnect) {

    CommunicationModule server = CommunicationModule::createServer(5618, PATH0);
    CommunicationModule client = CommunicationModule::createClient(5618, Sockets::IP({"::1"}), 5619);
    sleep(1);
    ASSERT_THROW(CommunicationModule client = CommunicationModule::createClient(5618, Sockets::IP({"::1"}), 5614);,
                 std::exception);
}


TEST(CommunicationTest, AfterDisconactionShouldNotBeAbleToSendMessage) {

    TestMess *mess = new TestMess("first mss");
    CommunicationModule server = CommunicationModule::createServer(5616, PATH0);
    {
        CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617);
    }
    sleep(1);

    ASSERT_THROW(server.send(mess), std::exception);

    delete mess;
}

TEST(CommunicationTest, AfterDisconactionShouldBeAbleToReadReceivedMessage) {

    TestMess *mess = new TestMess("first mss");
    CommunicationModule server = CommunicationModule::createServer(5616, PATH0);
    {
        CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617);
        client.send(mess);
    }
    sleep(1);

    std::shared_ptr<Message> receivedMess = server.read();
    ASSERT_EQ(mess->id_, receivedMess.get()->id_);
    ASSERT_EQ(mess->id_, dynamic_cast<TestMess *>(receivedMess.get())->id_);

    delete mess;
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
    ASSERT_EQ(sendMess->_data, dynamic_cast<TestMess *>(receivedMess.get())->_data);

    delete sendMess;
    boost::filesystem::remove_all(PATH);
}

TEST(CommunicationTest, ReceiveMessageWithCustomDictionaries) {

    CommunicationModule server = CommunicationModule::createServer(5618, PATH0);
    CommunicationModule client = CommunicationModule::createClient(5618, Sockets::IP({"::1"}), 5619, PATH1);
    TestMess *sendMess = new TestMess("first mss");
    client.send(sendMess);
    sleep(1);
    std::shared_ptr<Message> receivedMess = server.read();

    ASSERT_EQ(sendMess->id_, receivedMess.get()->id_);
    ASSERT_EQ(sendMess->_data, dynamic_cast<TestMess *>(receivedMess.get())->_data);

    delete sendMess;

    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, AfterReceivedAckRemoveItFromStorage) {
    boost::filesystem::remove_all(PATH1);
    CommunicationModule server = CommunicationModule::createServer(5616, PATH0);
    sleep(1);
    CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617, PATH1);
    TestMess *messToSend = new TestMess("first mss");
    client.send(messToSend);
    sleep(1);
    std::shared_ptr<Message> receivedMess = server.read();
    server.acknowledge(receivedMess.get());
    sleep(1);

    ASSERT_EQ(0, client.getMessageInStorageCount());

    delete messToSend;
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, ReceivedMessagesAreNotSavedInStorage) {
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
    CommunicationModule server = CommunicationModule::createServer(5616, PATH0);
    CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617, PATH1);
    TestMess *messToSend = new TestMess("first mss");
    client.send(messToSend);
    sleep(1);
    std::shared_ptr<Message> receivedMess = server.read();
    server.acknowledge(receivedMess.get());
    sleep(1);
    ASSERT_EQ(0, server.getMessageInStorageCount());

    delete messToSend;
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, AfterRestartContainsUnconfirmedMessage) {

    boost::filesystem::remove_all(PATH1);
    TestMess *messToSend = new TestMess("first mss");

    CommunicationModule server = CommunicationModule::createServer(5616);
    {
        CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617, PATH1);
        sleep(1);
        client.send(messToSend);
        sleep(1);
    }
    CommunicationModule client1 = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617, PATH1);
    ASSERT_EQ(1, client1.getMessageInStorageCount());

    delete messToSend;
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, AfterReconnectToSomeServerResendUnconfirmedMessage) {

    TestMess *messToSend = new TestMess("first mss");
    boost::filesystem::remove_all(PATH1);
    {
        CommunicationModule server = CommunicationModule::createServer(5616);
        CommunicationModule client0 = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617, PATH1);
        sleep(1);
        client0.send(messToSend);
        sleep(1);
        std::shared_ptr<Message> receivedMess = server.read(); //msg received, but ack is not sent
    }

    CommunicationModule server = CommunicationModule::createServer(5618);
    CommunicationModule client1 = CommunicationModule::createClient(5618, Sockets::IP({"::1"}), 5617, PATH1);
    sleep(1);//wait for resend
    std::shared_ptr<Message> receivedMess1 = server.read();
    auto *receivedTest = dynamic_cast<TestMess *>(receivedMess1.get());
    server.acknowledge(receivedMess1.get());
    sleep(1);

    ASSERT_EQ(messToSend->id_, receivedTest->id_);
    ASSERT_EQ(messToSend->_data, receivedTest->_data);
    ASSERT_EQ(0, client1.getMessageInStorageCount());

    delete messToSend;
    boost::filesystem::remove_all(PATH);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, AfterReconnectToTheSameServerResendUnconfirmedMessage) {

    boost::filesystem::remove_all(PATH1);
    CommunicationModule server = CommunicationModule::createServer(5618);
    TestMess *messToSend = new TestMess("first mss");
    {
        sleep(1);
        CommunicationModule client0 = CommunicationModule::createClient(5618, Sockets::IP({"::1"}), 5617, PATH1);
        sleep(1);
        client0.send(messToSend);
        sleep(1);
        std::shared_ptr<Message> receivedMess = server.read(); //msg received, but ack is not sent
    }
    //client 0 have not received ack, client1 should load unconfirmed message and resend it
    CommunicationModule client1 = CommunicationModule::createClient(5618, Sockets::IP({"::1"}), 5620, PATH1);
    sleep(1);//wait for resend
    std::shared_ptr<Message> receivedMess1 = server.read();
    auto *receivedTest = dynamic_cast<TestMess *>(receivedMess1.get());
    server.acknowledge(receivedMess1.get());
    sleep(1);

    ASSERT_EQ(messToSend->id_, receivedTest->id_);
    ASSERT_EQ(messToSend->_data, receivedTest->_data);
    ASSERT_EQ(0, client1.getMessageInStorageCount());

    delete messToSend;
    boost::filesystem::remove_all(PATH);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, AfterOtherReconnectResendYourUnconfirmedMessages) {

    boost::filesystem::remove_all(PATH);
    boost::filesystem::remove_all(PATH1);
    CommunicationModule server = CommunicationModule::createServer(5617);
    TestMess *messToSend = new TestMess("first mss");
    {
        CommunicationModule client0 = CommunicationModule::createClient(5617, Sockets::IP({"::1"}), 5619, PATH1);
        sleep(1);
        server.send(messToSend);
    }

    CommunicationModule client = CommunicationModule::createClient(5617, Sockets::IP({"::1"}), 5619, PATH1);
    sleep(1);//wait for resend

    std::shared_ptr<Message> receivedMess1 = client.read();
    auto *receivedTest = dynamic_cast<TestMess *>(receivedMess1.get());

    ASSERT_EQ(messToSend->id_, receivedTest->id_);
    ASSERT_EQ(messToSend->_data, receivedTest->_data);

    client.acknowledge(receivedMess1.get());
    sleep(1);
    ASSERT_EQ(0, server.getMessageInStorageCount());

    delete messToSend;
    boost::filesystem::remove_all(PATH1);
    boost::filesystem::remove_all(PATH);
}


TEST(CommunicationTest, AfterBothReconnectResendUnconfirmedMessages) {

    boost::filesystem::remove_all(PATH);
    boost::filesystem::remove_all(PATH1);

    TestMess *messToSend = new TestMess("first mss");
    {
        CommunicationModule server = CommunicationModule::createServer(5617);
        CommunicationModule client0 = CommunicationModule::createClient(5617, Sockets::IP({"::1"}), 5619, PATH1);
        sleep(1);
        server.send(messToSend);
    }

    CommunicationModule server = CommunicationModule::createServer(5617);
    CommunicationModule client = CommunicationModule::createClient(5617, Sockets::IP({"::1"}), 5619, PATH1);
    sleep(1);//wait for resend

    std::shared_ptr<Message> receivedMess1 = client.read();
    auto *receivedTest = dynamic_cast<TestMess *>(receivedMess1.get());
    client.acknowledge(receivedMess1.get());
    sleep(1);

    ASSERT_EQ(messToSend->id_, receivedTest->id_);
    ASSERT_EQ(messToSend->_data, receivedTest->_data);
    ASSERT_EQ(0, server.getMessageInStorageCount());

    delete messToSend;
    boost::filesystem::remove_all(PATH1);
    boost::filesystem::remove_all(PATH);
}

TEST(CommunicationTest, WhenSenderHasNotReceivedAckDoNotReadTheSameMessageAgain) {

    CommunicationModule server = CommunicationModule::createServer(5616);
    TestMess *messToSend = new TestMess("first mss");
    {
        sleep(1);
        CommunicationModule client0 = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617, PATH1);
        sleep(1);
        client0.send(messToSend);
        sleep(1);
    }

    std::shared_ptr<Message> receivedMess = server.read();
    server.acknowledge(receivedMess.get());

    CommunicationModule client1 = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5618, PATH1);
    sleep(1); //client1 will resend message

    ASSERT_THROW(server.read(), std::exception); //queue is empty

    delete messToSend;
    boost::filesystem::remove_all(PATH);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, WhenReceivedAckRemoveTheProperMessage) {

    boost::filesystem::remove_all(PATH1);
    TestMess *mess1 = new TestMess("sec");
    {
        CommunicationModule server = CommunicationModule::createServer(5616, PATH0);
        TestMess *mess0 = new TestMess("first");
        CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617, PATH1);
        sleep(1);
        server.send(mess0);
        sleep(1);
        server.send(mess1);
        sleep(1);
        std::shared_ptr<Message> receivedMess0 = client.read();
        std::shared_ptr<Message> receivedMess1 = client.read();
        client.acknowledge(receivedMess0.get());
        delete mess0;
    }

    CommunicationModule server = CommunicationModule::createServer(5616, PATH0);
    CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5618, PATH1);
    sleep(1); //server will resend message

    std::shared_ptr<Message> receivedMess = client.read();

    ASSERT_EQ(mess1->_data, dynamic_cast<TestMess *>(receivedMess.get())->_data);

    delete mess1;
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, WhenReceivedAckRemoveTheProperMessage1) {


    TestMess *mess1 = new TestMess("sec");
    {
        CommunicationModule server = CommunicationModule::createServer(5616, PATH0);
        TestMess *mess0 = new TestMess("first");
        CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617, PATH1);
        sleep(1);
        server.send(mess1);
        sleep(1);
        server.send(mess0);
        sleep(1);
        std::shared_ptr<Message> receivedMess0 = client.read();
        std::shared_ptr<Message> receivedMess1 = client.read();
        client.acknowledge(receivedMess1.get());
        delete mess0;
    }

    CommunicationModule server = CommunicationModule::createServer(5616, PATH0);
    CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5618, PATH1);
    sleep(1); //server will resend message

    std::shared_ptr<Message> receivedMess = client.read();

    ASSERT_EQ(mess1->_data, dynamic_cast<TestMess *>(receivedMess.get())->_data);

    delete mess1;
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, WhenReceivedSameAckManyTimesRemoveOnlyOneMessage) {


    TestMess *mess1 = new TestMess("sec");
    CommunicationModule server = CommunicationModule::createServer(5616, PATH0);
    CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617, PATH1);
    server.send(mess1);
    server.send(mess1);
    sleep(1);
    std::shared_ptr<Message> receivedMess0 = client.read();

    client.acknowledge(receivedMess0.get());
    client.acknowledge(receivedMess0.get());
    client.acknowledge(receivedMess0.get());
    client.acknowledge(receivedMess0.get());
    client.acknowledge(receivedMess0.get());
    sleep(1);

    ASSERT_EQ(1, server.getMessageInStorageCount()); //queue is empty

    delete mess1;
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, WhenReceivedSameAckToUnknowMessageDoNothing) {


    TestMess *mess1 = new TestMess("sec");
    TestMess *mess2 = new TestMess("sec");
    CommunicationModule server = CommunicationModule::createServer(5616, PATH0);
    CommunicationModule client = CommunicationModule::createClient(5616, Sockets::IP({"::1"}), 5617, PATH1);
    server.send(mess1);
    client.acknowledge(mess2);
    sleep(1);
    ASSERT_EQ(1, server.getMessageInStorageCount());

    delete mess1;
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}