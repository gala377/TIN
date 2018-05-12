//
// Created by Martyna on 26.04.18.
//

#include <gtest/gtest.h>
#include <string>
#include "../../inc/packages/PackageFactory.h"
#include "../../inc/packages/Acknowledge.h"

TEST(PackageFactoryCreationTest, PositiveNos) {
    PackageFactory::instance()->registerPackage<Acknowledge>(1);
    Package *pack = (PackageFactory::instance()->create(1));
    Acknowledge *ack = static_cast<Acknowledge *>(pack);
    ASSERT_EQ(0, ack->getAcknowledgedPackedId());
}


TEST(PackageFactorySerializaionCompleteTest, PositiveNos) {
    PackageFactory::instance()->registerPackage<Acknowledge>(1);
    Acknowledge ack(111);

    std::string serializationStr = PackageFactory::instance()->generateSerializationString(1, &ack);

    Package *pack = (PackageFactory::instance()->createPackageFromSerializationString(serializationStr));
    Acknowledge *receivedAck = static_cast<Acknowledge *>(pack);
    ASSERT_EQ(ack.getAcknowledgedPackedId(), receivedAck->getAcknowledgedPackedId());
    ASSERT_EQ(1, pack->typeId_);
}
