//
// Created by Martyna on 26.04.18.
//

#include <gtest/gtest.h>
#include <string>
#include "../../inc/packages/PackageFactory.h"
#include "../../inc/packages/Registrator.h"
#include "../../inc/packages/Acknowledge.h"

TEST(PackageFactoryCreationTest, PositiveNos) {
    PackageFactory::instance()->registerPackage<Acknowledge>(1);
    Package *pack = (PackageFactory::instance()->create(1));
    Acknowledge *ack = static_cast<Acknowledge *>(pack);
    ASSERT_EQ(0, ack->getAcknowledgedPackedId());
}


TEST(PackageFactoryDeserializaionTest2, PositiveNos) {
    PackageFactory::instance()->registerPackage<Acknowledge>(1);
    Acknowledge ack(111);
    std::string serial_str;
    {
        boost::iostreams::back_insert_device<std::string> inserter(serial_str);
        boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
        boost::archive::binary_oarchive outputArchive(s);

        outputArchive & ack;
        s.flush();
    }

    boost::iostreams::basic_array_source<char> device(serial_str.data(), serial_str.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > a(device);
    boost::archive::binary_iarchive archive(a);

    Package *pack = (PackageFactory::instance()->createFromArchive(1, archive));
    Acknowledge *receivedAck = static_cast<Acknowledge *>(pack);

    ASSERT_EQ(ack.getAcknowledgedPackedId(), receivedAck->getAcknowledgedPackedId());
}

TEST(PackageFactorySerializaionTest, PositiveNos) {
    PackageFactory::instance()->registerPackage<Acknowledge>(1);
    Acknowledge ack(111);

    std::string serial_str = PackageFactory::instance()->generateSerializationString(1, &ack);

    Package *pack = (PackageFactory::instance()->createFromSerializationString(1, serial_str));
    Acknowledge *receivedAck = static_cast<Acknowledge *>(pack);
    ASSERT_EQ(ack.getAcknowledgedPackedId(), receivedAck->getAcknowledgedPackedId());
}
