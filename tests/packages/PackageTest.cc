//
// Created by Martyna on 26.04.18.
//

#include <gtest/gtest.h>
#include <string>
#include "../../inc/packages/Package.h"

TEST(PackageSerializationTest, PositiveNos) {

    Package firstPackage = Package(1, 2, 3);

    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive outputArchive(s);

    firstPackage.serialize(outputArchive,1);
    s.flush();

    boost::iostreams::basic_array_source<char> device(serial_str.data(), serial_str.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > a(device);
    boost::archive::binary_iarchive inputArchive(a);


    Package deserializedPackage;
    deserializedPackage.serialize(inputArchive,1);
    ASSERT_EQ(firstPackage.getId_(), deserializedPackage.getId_());
    ASSERT_EQ(firstPackage.getTypeId_(), deserializedPackage.getTypeId_());
    ASSERT_EQ(firstPackage.getSize_(), deserializedPackage.getSize_());
}
