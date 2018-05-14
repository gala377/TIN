//
// Created by gajus123 on 14.05.18.
//

#include "../../inc/messages/Message.h"

std::string Message::toString() {
    std::string result;
    boost::iostreams::back_insert_device<std::string> inserter(result);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive outputArchive(s);

    outputArchive << &(*this);
    s.flush();
    return result;
}

Message* Message::fromString(const std::string& buffer) {
    Message* result;
    boost::iostreams::basic_array_source<char> device(buffer.data(), buffer.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > a(device);
    boost::archive::binary_iarchive inputArchive(a);

    inputArchive >> result;
    return result;
}