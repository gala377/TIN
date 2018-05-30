//
// Created by Martyna on 27.05.18.
//

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <queue/Exceptions.h>
#include <system/Exceptions.h>
#include <pthread.h>
#include <system/CommunicationModule.h>
#include "TestMessage.h"

#define PATH "./gryphon"
#define PATH0 "./Test0"
#define PATH1 "./Test1"

#define PORT0 5620
#define PORT1 5621
#define PORT2 5622
#define PORT3 5633

using namespace Queue;

TEST(CommunicationTest, ClientAndServerEstablishConnectionsWithoutExceptionsWithDefaultDictionaries) {
    ASSERT_NO_THROW(
            CommunicationModule server = CommunicationModule::createServer(PORT0);
            CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1);
    );
}

TEST(CommunicationTest, ClientAndServerEstablishConnectionsWithoutExceptionsWithCustomDictionaries) {
    ASSERT_NO_THROW(
            CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
            CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
    );
}

TEST(CommunicationTest, EstablishConnectionClientFirst) {
    ASSERT_THROW({
                     CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1);
                     sleep(1);
                     CommunicationModule server = CommunicationModule::createServer(PORT0);
                 }, System::ConnectionTimeOut
    );
}

TEST(CommunicationTest, EstablishConnectionServerFirst) {
    ASSERT_NO_THROW(
            CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
            sleep(1);
            CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
    );
}

TEST(CommunicationTest, WhenServerReadingEmptyQueueExceptionIsThrown) {

    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
    CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
    CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
    sleep(1);
    ASSERT_THROW(server.read(), System::NoMessageToRead);
}

TEST(CommunicationTest, WhenClientReadingEmptyQueueExceptionIsThrown) {

    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
    CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
    CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
    sleep(1);
    ASSERT_THROW(client.read(), System::NoMessageToRead);
}

TEST(CommunicationTest, OnlyOneClientCanConnect) {

    CommunicationModule server = CommunicationModule::createServer(5618, PATH0);
    CommunicationModule client = CommunicationModule::createClient(5618, Sockets::IP({"::1"}), 5619);
    sleep(1);
    CommunicationModule client2 = CommunicationModule::createClient(5618, Sockets::IP({"::1"}), 5614, PATH1);
    Message *mess = new TestMess("A");
    server.send(mess);
    sleep(1);

    ASSERT_NO_THROW(client.read());
    ASSERT_THROW(client2.read(), System::NoMessageToRead);

    delete mess;
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}


TEST(CommunicationTest, AfterDisconnactionShouldNotBeAbleToSendMessage) {

    boost::filesystem::remove_all(PATH0);
    TestMess *mess = new TestMess("first mss");
    CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
    {
        CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1);
    }
    sleep(1);

    ASSERT_THROW(server.send(mess), System::UnableToSentMessageConnectionClosed);

    delete mess;
    boost::filesystem::remove_all(PATH0);
}

TEST(CommunicationTest, AfterDisconnactionShouldBeAbleToReadReceivedMessage) {

    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);

    TestMess *mess = new TestMess("first mss");
    CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
    {
        CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1 , PATH1);
        client.send(mess);
    }
    sleep(1);

    std::shared_ptr<Message> receivedMess = server.read();
    ASSERT_EQ(mess->id_, receivedMess.get()->id_);
    ASSERT_EQ(mess->id_, dynamic_cast<TestMess *>(receivedMess.get())->id_);

    delete mess;

    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);

}


TEST(CommunicationTest, ReceiveMessageWithDefaultDictionaries) {

    boost::filesystem::remove_all(PATH);
    CommunicationModule server = CommunicationModule::createServer(PORT0);
    CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1);
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

    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);

    CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
    CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
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
    CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
    sleep(1);
    CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
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
    CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
    CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
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

    CommunicationModule server = CommunicationModule::createServer(PORT0);
    {
        CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
        sleep(1);
        client.send(messToSend);
        sleep(1);
    }
    CommunicationModule client1 = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
    ASSERT_EQ(1, client1.getMessageInStorageCount());

    delete messToSend;
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, AfterReconnectToSomeServerResendUnconfirmedMessage) {

    TestMess *messToSend = new TestMess("first mss");
    boost::filesystem::remove_all(PATH1);
    {
        CommunicationModule server = CommunicationModule::createServer(PORT0);
        CommunicationModule client0 = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
        sleep(1);
        client0.send(messToSend);
        sleep(1);
        std::shared_ptr<Message> receivedMess = server.read(); //msg received, but ack is not sent
    }

    CommunicationModule server = CommunicationModule::createServer(PORT2);
    CommunicationModule client1 = CommunicationModule::createClient(PORT2, Sockets::IP({"::1"}), PORT1, PATH1);
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
    CommunicationModule server = CommunicationModule::createServer(PORT0);
    TestMess *messToSend = new TestMess("first mss");
    {
        sleep(1);
        CommunicationModule client0 = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
        sleep(1);
        client0.send(messToSend);
        sleep(1);
        std::shared_ptr<Message> receivedMess = server.read(); //msg received, but ack is not sent
    }
    //client 0 have not received ack, client1 should load unconfirmed message and resend it
    CommunicationModule client1 = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT3, PATH1);
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
    CommunicationModule server = CommunicationModule::createServer(PORT0);
    TestMess *messToSend = new TestMess("first mss");
    {
        CommunicationModule client0 = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
        sleep(1);
        server.send(messToSend);
    }

    CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
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
        CommunicationModule server = CommunicationModule::createServer(PORT0);
        CommunicationModule client0 = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
        sleep(1);
        server.send(messToSend);
    }

    CommunicationModule server = CommunicationModule::createServer(PORT0);
    CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
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

    CommunicationModule server = CommunicationModule::createServer(PORT0);
    TestMess *messToSend = new TestMess("first mss");
    {
        sleep(1);
        CommunicationModule client0 = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
        sleep(1);
        client0.send(messToSend);
        sleep(1);
    }

    std::shared_ptr<Message> receivedMess = server.read();
    server.acknowledge(receivedMess.get());

    CommunicationModule client1 = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT3, PATH1);
    sleep(1);

    ASSERT_THROW(server.read(), System::NoMessageToRead); //queue is empty

    delete messToSend;
    boost::filesystem::remove_all(PATH);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, WhenReceivedAckRemoveTheProperMessage) {

    boost::filesystem::remove_all(PATH0);
    TestMess *mess0 = new TestMess("first");
    TestMess *mess1 = new TestMess("sec");
    CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
    {
        CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
        sleep(1);
        server.send(mess0);
        sleep(1);
        server.send(mess1);
        sleep(1);
        std::shared_ptr<Message> receivedMess0 = client.read();
        std::shared_ptr<Message> receivedMess1 = client.read();
        client.acknowledge(receivedMess0.get());
    }

    CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT3, PATH1);
    sleep(1); //server will resend message

    std::shared_ptr<Message> receivedMess = client.read();

    ASSERT_EQ(mess1->_data, dynamic_cast<TestMess *>(receivedMess.get())->_data);

    delete mess0;
    delete mess1;
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, WhenReceivedAckRemoveTheProperMessage1) {

    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
    TestMess *mess1 = new TestMess("sec");
    TestMess *mess0 = new TestMess("first");
    CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
    {
        CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
        sleep(1);
        server.send(mess0);
        server.send(mess1);
        sleep(1);
        std::shared_ptr<Message> receivedMess0 = client.read();
        sleep(1);
        std::shared_ptr<Message> receivedMess1 = client.read();
        client.acknowledge(receivedMess1.get());
    }

    CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT3, PATH1);
    sleep(1); //server will resend message

    std::shared_ptr<Message> receivedMess = client.read();

    ASSERT_EQ(mess0->_data, dynamic_cast<TestMess *>(receivedMess.get())->_data);

    delete mess0;
    delete mess1;
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, WhenReceivedAckManyTimesRemoveOnlyOneMessage) {

    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);

    TestMess *mess1 = new TestMess("first");
    TestMess *mess2 = new TestMess("sec");
    CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
    CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
    sleep(1);
    server.send(mess1);
    sleep(1);
    server.send(mess2);
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
    delete mess2;
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}

TEST(CommunicationTest, WhenReceivedAckToUnknowMessageThrowException) {

    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);

    TestMess *mess1 = new TestMess("sec");
    TestMess *mess2 = new TestMess("sec");
    CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
    CommunicationModule client = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
    sleep(1);
    server.send(mess1);

    ASSERT_THROW(
            client.acknowledge(mess2);//mess2 does not exist in server's storage
            sleep(1);
            ,System::UnknownMessageAcknowledge
    );

    delete mess1;
    delete mess2;
    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
}


TEST(CommunicationTest, MessagesInStorageShouldNotBeOverwritten) {

    boost::filesystem::remove_all(PATH0);
    boost::filesystem::remove_all(PATH1);
    CommunicationModule server = CommunicationModule::createServer(PORT0, PATH0);
    sleep(1);
    int recipientsCount = 5;
    int messagesCount = 5;
    for (int j = 0; j < recipientsCount; ++j) {
        CommunicationModule client0 = CommunicationModule::createClient(PORT0, Sockets::IP({"::1"}), PORT1, PATH1);
        sleep(0.2);
        for (int i = 0; i < messagesCount; ++i) {
            TestMess *messToSend = new TestMess("first mss");
            server.send(messToSend);
            sleep(0.1);
            delete messToSend;
        }
        boost::filesystem::remove_all(PATH1);
        sleep(0.2);
    }

    ASSERT_EQ(recipientsCount*messagesCount, server.getMessageInStorageCount());

    boost::filesystem::remove_all(PATH1);
    boost::filesystem::remove_all(PATH);
}