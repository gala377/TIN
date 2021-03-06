//
// Created by gajus123 on 14.05.18.
//

#include <fstream>
#include "messages/Message.h"

Message::Message() :
        id_(global_id++) {
}

Message::~Message() {
}

std::string Message::toString() const {
    std::string result;
    boost::iostreams::back_insert_device<std::string> inserter(result);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive outputArchive(s);

    outputArchive << &(*this);
    s.flush();
    return result;
}

void Message::toFile(const std::string &path) const {
    std::ofstream ofs(path);
    boost::archive::binary_oarchive outputArchive(ofs);

    outputArchive << &(*this);
    ofs.close();
}

std::shared_ptr<Message> Message::fromString(const std::string& buffer) {
    Message* result;
    boost::iostreams::basic_array_source<char> device(buffer.data(), buffer.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > a(device);
    boost::archive::binary_iarchive inputArchive(a);

    inputArchive >> result;
    return std::shared_ptr<Message>(result);
}

std::shared_ptr<Message> Message::fromFile(const std::string &path) {
    Message *result;
    std::ifstream ifs(path);
    boost::archive::binary_iarchive ia(ifs);

    ia >> result;
    ifs.close();
    return std::shared_ptr<Message>(result);
}

std::shared_ptr<Message> Message::fromBuffer(boost::asio::streambuf& buffer) {
    Message* result;
    boost::archive::binary_iarchive inputArchive(buffer);

    inputArchive >> result;
    return std::shared_ptr<Message>(result);
}

void Message::setGlobalId(uint32_t id) {
    global_id = id;
}

uint32_t Message::global_id = 10;