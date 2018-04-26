//
// Created by Martyna on 24.04.18.
//

#include "../../inc/packages/Package.h"

Package::Package() {
}

Package::Package(uint32_t typeId, uint32_t dataSize, const char *data) : data_(data), typeId_(typeId), dataSize_(dataSize) {
}

//uint32_t Package::getId() const{
//    return id_;
//}
uint32_t Package::getTypeId() const {
    return typeId_;
}

const char *Package::getData() const {
    return data_;
}

char *Package::getPackageToSend() const {
    char *packageToSend = new char[dataSize_ + sizeof(dataSize_) + sizeof(id_) + sizeof(typeId_)];

    memcpy(packageToSend, &dataSize_, 4);
    memcpy(packageToSend + 4, &id_, 4);
    memcpy(packageToSend + 8, &typeId_, 4);
    memcpy(packageToSend + 12, &data_, dataSize_ + 1);

    return packageToSend;
}

void Package::deserialize(const char *serializedStr) {
    memcpy(&dataSize_, serializedStr, 4);
    memcpy(&id_, serializedStr + 4, 4);
    memcpy(&typeId_, serializedStr + 8, 4);


    data_ = new char[dataSize_];
    memcpy(&data_, serializedStr + 12, dataSize_ + 1);
}