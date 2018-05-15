//
// Created by Martyna on 11.05.18.
//

#ifndef TIN_ACKNOWLEDGE_H
#define TIN_ACKNOWLEDGE_H

#include "Package.h"

class Acknowledge : public Package {

public:
    Acknowledge() = default;

    explicit Acknowledge(uint32_t acknowledgedPackedId_);

    uint32_t getAcknowledgedPackedId() const;

private:
    uint32_t acknowledgedPackedId_ = 0;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Package>(*this);
        ar & acknowledgedPackedId_;
    }


};


#endif //TIN_ACKNOWLEDGE_H
