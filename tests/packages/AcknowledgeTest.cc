//
// Created by Martyna on 11.05.18.
//

#include <gtest/gtest.h>
#include <string>
#include "../../inc/packages/Acknowledge.h"

TEST(AcknowledgePackageSerializationTest, PositiveNos) {

    auto ack = Acknowledge(10);
    Acknowledge deserializedPackage;
    std::string serial_str;

    {
        boost::iostreams::back_insert_device<std::string> inserter(serial_str);
        boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
        boost::archive::binary_oarchive outputArchive(s);

        outputArchive & ack;
        s.flush();
    }
    {
        boost::iostreams::basic_array_source<char> device(serial_str.data(), serial_str.size());
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > a(device);
        boost::archive::binary_iarchive inputArchive(a);

        inputArchive & deserializedPackage;
    }

    ASSERT_EQ(ack.getAcknowledgedPackedId(), deserializedPackage.getAcknowledgedPackedId());
    ASSERT_EQ(ack.getTypeId_(), deserializedPackage.getTypeId_());
    ASSERT_EQ(ack.getSize_(), deserializedPackage.getSize_());
}