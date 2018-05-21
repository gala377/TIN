//
// Created by gajus123 on 15.05.18.
//

#include <gtest/gtest.h>

#include "../../inc/messages/Acknowledge.h"

TEST(AcknowledgeTests, SerializationEquality) {
    Acknowledge ack(1242);
    Message* mess = &ack;
    std::string temp = mess->toString();
    auto mess2 = Message::fromString(temp);
    Acknowledge* ack2 = dynamic_cast<Acknowledge*>(mess2.get());
    ASSERT_EQ(ack2 != nullptr, true);
    EXPECT_EQ(ack.id_, ack2->id_);
}

TEST(AcknowledgeTests, SerializationWithBuffer) {
    Acknowledge ack(1242);
    Message* mess = &ack;
    std::string temp = mess->toString();
    boost::asio::streambuf buffer;
    std::ostream os(&buffer);
    os << temp;
    auto mess2 = Message::fromBuffer(buffer);
    Acknowledge* ack2 = dynamic_cast<Acknowledge*>(mess2.get());
    ASSERT_EQ(ack2 != nullptr, true);
    EXPECT_EQ(ack.id_, ack2->id_);
}