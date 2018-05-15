//
// Created by Martyna on 24.04.18.
//

#include "../../inc/packages/Package.h"


uint32_t Package::getId() const {
    return id_;
}

void Package::setId_(uint32_t id_) {
    Package::id_ = id_;
}

std::string Package::serialize() {
    std::string serializedPackage;
    boost::iostreams::back_insert_device<std::string> inserter(serializedPackage);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive outputArchive(s);

    outputArchive & &(*this);
    s.flush();

    return serializedPackage;
}

Package *Package::deserialize(std::string serializedPackage) {
    Package* package;
    boost::iostreams::basic_array_source<char> device(serializedPackage.data(), serializedPackage.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > a(device);
    boost::archive::binary_iarchive inputArchive(a);
    inputArchive & package;
    return package;
}


