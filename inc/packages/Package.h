//
// Created by Martyna on 24.04.18.
//

#ifndef TIN_PACKAGE_H
#define TIN_PACKAGE_H


#include <stdio.h>
#include <stdint.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>

class Package {

public:

    Package() = default;

    Package(uint32_t typeId, uint32_t size, uint32_t id);

    uint32_t getTypeId_() const;

    uint32_t getSize_() const;

    uint32_t getId_() const;

private:
    uint32_t typeId_ = 0;
    uint32_t size_ = 0;
    uint32_t id_ = 0;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & typeId_;
        ar & size_;
        ar & id_;
    }
};

#endif //TIN_PACKAGE_H
