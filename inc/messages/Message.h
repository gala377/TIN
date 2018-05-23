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
#include <boost/asio/streambuf.hpp>

class Message {
public:
    virtual ~Message();

    std::string toString() const;
    void toFile(const std::string &pathName) const;

    static Message *fromString(const std::string &buffer);
    static Message *fromFile(const std::string &pathName);
    static Message *fromBuffer(boost::asio::streambuf &buffer);

    uint32_t id_;
protected:
    Message();
private:

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & id_;
    }

    static uint32_t global_id;
};

#endif //TIN_MESSAGE_H