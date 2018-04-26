//
// Created by Martyna on 26.04.18.
//

#include <gtest/gtest.h>
#include <string>
#include "../../inc/packages/PackageFabric.h"

TEST(MessageGetFromPackageHasNotBeenCHanged, PositiveNos) {
    NumericalMessage message(2);
    Package package = createPackage(message);
    Message *msg = deserializePackage(package);

    ASSERT_EQ(message.getNum(), dynamic_cast<NumericalMessage *>(msg)->getNum());
}
