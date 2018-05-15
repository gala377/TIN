//
// Created by Martyna on 24.04.18.
//

#ifndef TIN_PACKAGE_H
#define TIN_PACKAGE_H

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>

class Package {

public:

    Package()= default;

    uint32_t getId() const;

    void setId_(uint32_t id_);

    std::string serialize();

    static Package *deserialize(std::string);

    virtual ~Package() = default;

private:
    uint32_t id_ = 0;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & id_;
    }
};

#endif //TIN_PACKAGE_H
