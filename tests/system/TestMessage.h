//
// Created by Martyna on 28.05.18.
//

#ifndef TIN_TESTMESSAGE_H
#define TIN_TESTMESSAGE_H

#include <messages/Message.h>

class TestMess : public Message {
public:

    TestMess(): _data("") {}
    TestMess(std::string data) : _data(std::move(data)) {}

    std::string _data;

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Message>(*this);
        ar & _data;
    }
};

BOOST_CLASS_EXPORT(TestMess)

#endif //TIN_TESTMESSAGE_H
