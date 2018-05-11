//
// Created by Martyna on 24.04.18.
//

#include "../../inc/packages/Package.h"

Package::Package(uint32_t typeId,uint32_t size, uint32_t id) : id_(id), typeId_(typeId),size_(size) {
}

uint32_t Package::getTypeId_() const {
    return typeId_;
}

uint32_t Package::getSize_() const {
    return size_;
}

uint32_t Package::getId_() const {
    return id_;
}

