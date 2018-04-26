//
// Created by Martyna on 24.04.18.
//

#ifndef TIN_PACKAGE_H
#define TIN_PACKAGE_H


#include <stdio.h>
#include <stdint.h>

#include <string.h>

class Package {

public:

    Package();

    Package(uint32_t typeId, uint32_t dataSize, const char *data);

    char *getPackageToSend() const;

    void deserialize(const char *);

//    uint32_t getId() const;
    uint32_t getTypeId() const;
    const char* getData() const;

    uint32_t id_;
private:
    uint32_t typeId_=0;
    uint32_t dataSize_=0;
    const char *data_;

};

#endif //TIN_PACKAGE_H
