//
// Created by igor on 21.05.18.
//

#ifndef TIN_STORAGE_H
#define TIN_STORAGE_H


#vec
#include <messages/Message.h>

namespace Queue {

    class Storage {
    public:
        virtual void add(Message mess) = 0;
        virtual void remove(int id) = 0;

        virtual std::vector<Message> getAll() = 0;

        class Iterator {
        public:
            virtual Message operator*() = 0;

            virtual Iterator& operator++() = 0;
            virtual Iterator& operator++(int) = 0;
        };

        virtual Iterator begin() = 0;
        virtual Iterator end() = 0;

    };

}

#endif //TIN_STORAGE_H
