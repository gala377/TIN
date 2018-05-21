//
// Created by igor on 22.05.18.
//

#ifndef TIN_STORAGE_H
#define TIN_STORAGE_H

#include <messages/Message.h>

namespace Queue {

    class Storage {
    public:
        virtual void add(Message mess) = 0;
        virtual void remove(int id) = 0;

        virtual std::vector<Message> getAll() = 0;

        class Iterator {
        public:
            Iterator();
            Iterator(const Iterator&);

            virtual Iterator& operator=(const Iterator&) = 0;

            virtual bool operator==(const Iterator&) = 0;
            virtual bool operator!=(const Iterator&) = 0;

            virtual Message operator*() = 0;
            virtual Message operator->() = 0;

            virtual Iterator& operator++() = 0;
            virtual const Iterator operator++(int) = 0;

        };

        virtual Iterator begin() = 0;
        virtual Iterator end() = 0;
    };
}

#endif //TIN_STORAGE_H
