//
// Created by Martyna on 22.05.18.
//

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <queue/Exceptions.h>

#include "../../inc/queue/FileStorage.h"
#include "../../inc/messages/Acknowledge.h"
#include "../../inc/messages/Message.h"

#define PATH "./tempTest/"
using namespace Queue;


TEST(FileStorageTest, RestoreMessage) {
    std::string path = PATH;

    FileStorage fileStorage(path);
    Acknowledge ack(2);
    fileStorage.add(ack);

    std::vector<std::shared_ptr<Message>> messages = fileStorage.getAll();

    ASSERT_EQ(ack.getConsumedPacketId(), std::dynamic_pointer_cast<Acknowledge>(messages.back())->getConsumedPacketId());

    boost::filesystem::remove_all(path);

}

TEST(FileStorageTest, LoadMessagesToMemoryWhenStorageIsEmpty) {
    std::string path = PATH;
    FileStorage fileStorage(path);

    FileStorage fileStorage2(path);
    std::vector<std::shared_ptr<Message>> messages = fileStorage2.getAll();

    boost::filesystem::remove_all(path);

    ASSERT_EQ(0, messages.size());
}

TEST(FileStorageTest, LoadMessagesToMemoryWhenStorageIsNotEmpty) {
    std::string path = PATH;
    FileStorage fileStorage(path);
    Acknowledge ack(2);
    fileStorage.add(ack);

    FileStorage fileStorage2(path);
    std::vector<std::shared_ptr<Message>> messages = fileStorage2.getAll();

    auto received = std::dynamic_pointer_cast<Acknowledge>(messages.back());
    ASSERT_EQ(ack.getConsumedPacketId(), received->getConsumedPacketId());

    boost::filesystem::remove_all(path);
}

TEST(FileStorageTest, AddMessageWhichAlreadyExistInStorage) {
    std::string path = PATH;

    FileStorage fileStorage(path);
    Acknowledge ack(2);
    fileStorage.add(ack);

    ASSERT_THROW(fileStorage.add(ack), FileExists);

    boost::filesystem::remove_all(path);
}


TEST(FileStorageTest, AfterRemovingMessageShouldBeRemovedFromMemory) {
    std::string path = PATH;

    FileStorage fileStorage(path);
    Acknowledge ack(2), ack2(22);
    fileStorage.add(ack);
    fileStorage.add(ack2);

    fileStorage.remove(ack.id_);
    std::vector<std::shared_ptr<Message>> messages = fileStorage.getAll();

    auto read = std::dynamic_pointer_cast<Acknowledge>(messages.back());
    ASSERT_EQ(1, messages.size());

    ASSERT_EQ(ack2.getConsumedPacketId(), read->getConsumedPacketId());
    boost::filesystem::remove_all(path);
}

TEST(FileStorageTest, AfterRemovingMessageShouldBeRemovedFromStorage) {
    std::string path = PATH;

    FileStorage fileStorage(path);
    Acknowledge ack(2), ack2(22);
    fileStorage.add(ack);
    fileStorage.add(ack2);

    fileStorage.remove(ack.id_);

    FileStorage fileStorage2(path);
    std::vector<std::shared_ptr<Message>> receivedMessages = fileStorage2.getAll();

    ASSERT_EQ(1, receivedMessages.size());

    auto received = std::dynamic_pointer_cast<Acknowledge>(receivedMessages.back());
    ASSERT_EQ(ack2.getConsumedPacketId(), received->getConsumedPacketId());

    boost::filesystem::remove_all(path);
}