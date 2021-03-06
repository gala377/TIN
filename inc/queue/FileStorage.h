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
    
    /*
    *   File system implementation of Storage interface.
    *   FileStorage saves messages as files
    *   in the path specified upon creation of
    *   the object.
    */
    class FileStorage: public Storage {
    public:
        /*
        *   Creates directory tree from @param path
        *   upon creation. Saves it in @member _root.
        *  
        *   If directory already exists populates itself
        *   with files that are already existing inside
        *   the directory.
        */
        explicit FileStorage(std::string&& root_path);
        explicit FileStorage(const std::string& root_path);

        /*
        *   Adds file under @member _root path.
        * 
        *   Files' name is Messages' @member id_.
        *   Files content is serialized Message.
        *   
        *   Throws FileExists if file path is cached.
        */
        void add(const Message& mess) override;
        /*
        *   As before, just files name is @param id
        *   and files content is @param data.
        */
        void add(int id, const std::string& data);

        /*
        *   Removes file corresponding to @param id 
        *   from the path specified in @member _root;
        * 
        *   Throws FileDoesNotExist if file is not present
        *   in cache. 
        */
        void remove(int id) override;

        /*
        *   Returns collection of all messages stored
        *   under @member _root;
        */
        std::vector<std::shared_ptr<Message>> getAll() override;

        /*
         * Returns id of the last added message.
         * It works on the assumption that messages will be added in the increasing order.
         */
        std::uint32_t lastAddedId() const;

        /*
         * Returns number of file paths stored in cache
         */
        std::uint32_t cachedFilesCount() const;
        /*
         * Check if file exists in storage
         */
        bool containsFile(int id) const;
        /*
        *   Forward iterator for range loop support.
        *   Iterates over files stored under @member _root.
        *   Indirection (*) operator returns Message* stored in file.
        */
        class Iterator {
        public:
            friend class FileStorage;

            bool operator==(const Iterator& iterator) const;
            bool operator!=(const Iterator& iterator) const;

            std::shared_ptr<Message> operator*() const;

            Iterator& operator++();

        private:
            std::set<std::string>::const_iterator _curr;
            const FileStorage& _parent;

            Iterator(std::set<std::string>::const_iterator curr_file,
                     const FileStorage& parent);

            std::shared_ptr<Message> readFile() const;
        };

        friend class Iterator;

        /*
        *   Methods for range loop support
        */
        Iterator begin() const;
        Iterator end() const;

        /*
        *   Returns all files under @member _root.
        *   
        *   Important -iterators and getAll method
        *   base on cached directory list.
        *   This method checks for directories as it is.
        *   So contents between those three can vary. 
        */ 
        std::set<std::string> listFiles() const;

        /*
        *   Refreshes cached file names.
        */ 
        void updateCachedFiles();

    protected:
        boost::filesystem::path _root;
        std::set<std::string> _files;

        std::string makePath(const Message& mess) const;
        std::string makePath(int id) const;

        std::string padInt(int num) const;

    };
}
#endif //TIN_FILESTORAGE_H
