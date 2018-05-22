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
        explicit FileStorage(std::string&& root_path);
        explicit FileStorage(const std::string& root_path);

        void add(const Message& mess) override;
        void add(int id, const std::string& data);
        void remove(int id) override;

        std::vector<Message> getAll() override;

        class Iterator {
        public:
            friend class FileStorage;

            bool operator==(const Iterator& iterator) const;
            bool operator!=(const Iterator& iterator) const;

            Message* operator*() const;
            Message* operator->() const;

            Iterator& operator++();

        private:
            std::set<std::string>::const_iterator _curr;
            const FileStorage& _parent;

            Iterator(std::set<std::string>::const_iterator curr_file,
                     const FileStorage& parent);

            Message* readFile() const;

        };

        friend class Iterator;

        Iterator begin() const;
        Iterator end() const;

        std::set<std::string> listFiles() const;

    protected:
        boost::filesystem::path _root;
        std::set<std::string> _files;

        std::string makePath(const Message& mess) const;
        std::string makePath(int id) const;

        std::string padInt(int num) const;

    };
}
#endif //TIN_FILESTORAGE_H
