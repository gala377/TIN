//
// Created by Martyna on 24.04.18.
//

#include "../../inc/packages/PackageFactory.h"

Package *PackageFactory::create(uint32_t typeId) {//public only or test purpose
    auto it = map.find(typeId);
    if (it == map.end()) {
        return nullptr;
    }
    Package *package = it->second();
    return package;
}

std::string PackageFactory::generateSerializationString(uint32_t typeId, Package *package) {
    std::string serial_str;
    package->typeId_=typeId;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive outputArchive(s);

    outputArchive & package;
    s.flush();

    return serial_str;
}

Package *PackageFactory::createFromSerializationString(uint32_t typeId, const std::string &serializationString) {
    boost::iostreams::basic_array_source<char> device(serializationString.data(), serializationString.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > a(device);
    boost::archive::binary_iarchive ia(a);
    Package *package = createFromArchive(typeId, ia);

    return package;
}
