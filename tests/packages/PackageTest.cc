//
// Created by Martyna on 26.04.18.
//

#include <gtest/gtest.h>
#include <string>
#include "../../inc/packages/Package.h"

TEST(PackageDuringSeralizationHasNotBeenChanged, PositiveNos) {
    std::string data("test");
    Package pack(1, data.size() + 1, data.c_str());
    Package testedPackage;
    testedPackage.deserialize(pack.getPackageToSend());

    ASSERT_EQ(pack.id_, testedPackage.id_);
    ASSERT_EQ(pack.getTypeId(), testedPackage.getTypeId());
    ASSERT_EQ(data, std::string(testedPackage.getData()));
}


