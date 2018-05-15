//
// Created by Martyna on 11.05.18.
//

#include <gtest/gtest.h>
#include <string>
#include "../../inc/packages/Acknowledge.h"

TEST(SerializeFunctionTest, PositiveNos) {

    auto ack = Acknowledge(10);
    Acknowledge deserializedPackage;
    std::string serializationString;

    {
        boost::iostreams::back_insert_device<std::string> inserter(serializationString);
        boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
        boost::archive::binary_oarchive outputArchive(s);

        outputArchive & ack;
        s.flush();
    }
    {
        boost::iostreams::basic_array_source<char> device(serializationString.data(), serializationString.size());
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > a(device);
        boost::archive::binary_iarchive inputArchive(a);

        inputArchive & deserializedPackage;
    }

    ASSERT_EQ(ack.getAcknowledgedPackedId(), deserializedPackage.getAcknowledgedPackedId());
    ASSERT_EQ(ack.getId(), deserializedPackage.getId());
}

TEST(BasePackageSerializationTest, PositiveNos) {
    Package *ack = new Acknowledge(5);
    ack->setId_(42);
    std::string serializationString = ack->serialize();
    Package *deserializedPack = Package::deserialize(serializationString);
    Acknowledge *deserializedAck = static_cast<Acknowledge *>(deserializedPack);

    ASSERT_EQ(ack->getId(), deserializedPack->getId());
    ASSERT_EQ(static_cast<Acknowledge *>(ack)->getAcknowledgedPackedId(), deserializedAck->getAcknowledgedPackedId());
}

