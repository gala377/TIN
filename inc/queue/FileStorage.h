//
// Created by igor on 21.05.18.
//

#ifndef TIN_FILESTORAGE_H
#define TIN_FILESTORAGE_H

#include <fstream>

#include <queue/Storage.h>

namespace Queue {

    class FileStorage: public Storage {
    public:
        explicit FileStorage(std::string&& file_name);
        explicit FileStorage(const std::string& file_name);

        void add(Message mess) override;
        void remove(int id) override;

        std::vector<Message> getAll() override;

        class Iterator: Storage::Iterator {
        public:
            Iterator();
            Iterator(const Iterator&);

            ~Iterator();

            Iterator& operator=(const Iterator&);

            bool operator==(const Iterator&);
            bool operator!=(const Iterator&);

            virtual Message operator*();
            virtual Message operator->();

            virtual Iterator& operator++();
            virtual const Storage::Iterator operator++(int);

        };

        friend class Iterator;

        Storage::Iterator begin() override;
        Storage::Iterator end() override;


    protected:
        std::fstream _file;
    };
}
#endif //TIN_FILESTORAGE_H
