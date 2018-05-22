//
// Created by igor on 22.05.18.
//

#ifndef TIN_STORAGE_H
#define TIN_STORAGE_H

#include <messages/Message.h>

namespace Queue {
    /*
    *   Interface for @class Message storage.
    *   Useful for mocking purposes.
    */
    class Storage {
    public:
        virtual void add(const Message& mess) = 0;
        virtual void remove(int id) = 0;

        virtual std::vector<Message> getAll() = 0;
    };
}

#endif //TIN_STORAGE_H
