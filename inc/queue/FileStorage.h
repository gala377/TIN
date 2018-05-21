//
// Created by igor on 21.05.18.
//

#ifndef TIN_FILESTORAGE_H
#define TIN_FILESTORAGE_H

#include <fstream>
#include <set>
#include <boost/filesystem.hpp>

#include <queue/Storage.h>


namespace Queue {

    class FileStorage: public Storage {
    public:
        // todo file names caching
        // todo implement iterator

        explicit FileStorage(std::string&& root_path);
        explicit FileStorage(const std::string& root_path);

        void add(const Message& mess) override;
        void add(int id, const std::string& data);
        void remove(int id) override;

        std::vector<Message> getAll() override;

        class Iterator {
        public:
            Iterator();

            bool operator==(const Iterator& iterator) const;
            bool operator!=(const Iterator& iterator) const;

            Message operator*() const;
            Message operator->() const;

            Iterator& operator++();
        };

        friend class Iterator;

        Iterator begin() const;
        Iterator end() const;

        std::set<std::string> listFiles() const;

    protected:
        boost::filesystem::path _root;

        std::string makePath(const Message& mess) const;
        std::string makePath(int id) const;

        std::string padInt(int num) const;

    };
}
#endif //TIN_FILESTORAGE_H
