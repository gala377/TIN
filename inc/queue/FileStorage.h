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
    protected:
        std::fstream _file;
    };
}
#endif //TIN_FILESTORAGE_H
