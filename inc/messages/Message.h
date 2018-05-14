//
// Created by gajus123 on 14.05.18.
//

#ifndef TIN_MESSAGE_H
#define TIN_MESSAGE_H

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/export.hpp>

class Message {
public:
    virtual ~Message() {}
    int a;

    std::string toString();
    static Message* fromString(const std::string& buffer);
protected:
    Message() {};
private:

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & a;
    }
};

#endif //TIN_MESSAGE_H