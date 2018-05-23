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

    std::vector<Message *> messages = fileStorage.getAll();

    ASSERT_EQ(ack.getConsumedPacketId(), dynamic_cast<Acknowledge *>(messages.back())->getConsumedPacketId());

    boost::filesystem::remove_all(path);

}

TEST(FileStorageTest, LoadMessagesFromMemoryWhenStorageIsEmpty) {
    std::string path = PATH;
    FileStorage fileStorage(path);

    FileStorage fileStorage2(path);
    std::vector<Message *> messages = fileStorage2.getAll();

    boost::filesystem::remove_all(path);

    ASSERT_EQ(0, messages.size());
}

TEST(FileStorageTest, LoadMessagesFromMemoryWhenStorageIsNotEmpty) {
    std::string path = PATH;
    FileStorage fileStorage(path);
    Acknowledge ack(2);
    fileStorage.add(ack);

    FileStorage fileStorage2(path);
    std::vector<Message *> messages = fileStorage2.getAll();

    Acknowledge *received = dynamic_cast<Acknowledge *>(messages.back());
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
