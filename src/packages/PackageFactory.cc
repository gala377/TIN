//
// Created by Martyna on 24.04.18.
//

#include "../../inc/packages/PackageFactory.h"

Package *PackageFactory::create(uint32_t typeId) {//public only or test purpose
    auto it = constructorsMap.find(typeId);
    if (it == constructorsMap.end()) {
        return nullptr;
    }
    return it->second();
}

std::string PackageFactory::generateSerializationString(uint32_t typeId, Package *package) {
    package->typeId_= typeId;
    auto it = serializingFunctiosMap.find(typeId);
    if (it == serializingFunctiosMap.end()) {
        return nullptr;
    }
    return it->second(package);

}

Package *PackageFactory::createFromSerializationString(uint32_t typeId, const std::string &serializationString) {
    Package *package = create(typeId);
    auto it = deserializingFunctionsMap.find(1);
    if (it == deserializingFunctionsMap.end()) {
        return nullptr;
    }
    it->second(package, serializationString);
    return package;
}
Package *PackageFactory::createPackageFromSerializationString(const std::string &serializedString) {
    Package base_package;
    boost::iostreams::basic_array_source<char> device(serializedString.data(), serializedString.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > a(device);
    boost::archive::binary_iarchive inputArchive(a);
    inputArchive & base_package ;
    return createFromSerializationString(base_package.typeId_, serializedString);
}
