//
// Created by gajus123 on 14.05.18.
//

#ifndef TIN_ACKNOWLEDGE_H
#define TIN_ACKNOWLEDGE_H

#include "Message.h"

class Acknowledge : public Message {
public:
    Acknowledge(int t) : b(t) {}

    int b;
private:
    Acknowledge() {}
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Message>(*this);
        ar & b;
    }
};

#endif //TIN_ACKNOWLEDGE_H