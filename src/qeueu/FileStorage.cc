//
// Created by igor on 21.05.18.
//

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <queue/FileStorage.h>

using namespace Queue;
namespace fs = boost::filesystem;

FileStorage::FileStorage(std::string&& root_path): _root(root_path) {
    boost::filesystem::create_directory(_root);
}

FileStorage::FileStorage(const std::string& root_path): _root(root_path) {
    boost::filesystem::create_directory(_root);
}


void FileStorage::add(const Message& mess) {
    // todo error if file already exists
    std::fstream file(makePath(mess), std::fstream::in);
    boost::archive::text_oarchive o_archive(file);
    o_archive << mess;
    file.close();
}

void FileStorage::remove(int id) {
    // todo error if file doesn't exists
    boost::filesystem::remove(
            boost::filesystem::path(makePath(id)));
}

std::vector<Message> FileStorage::getAll() {
    return std::vector<Message>();
}


std::string FileStorage::makePath(int id) const {
    return _root.string() + "/" + std::to_string(id);
}

std::string FileStorage::makePath(const Message& mess) const {
    return _root.string() + "/" + std::to_string(mess.id_);
}


std::set<std::string> FileStorage::listFiles() const {
    std::set<std::string> files;
    for(auto& p: boost::make_iterator_range(fs::directory_iterator(_root))) {
        files.insert(p.path().string());
    }
    return files;
}


FileStorage::Iterator FileStorage::begin() const {
    return FileStorage::Iterator();
}

FileStorage::Iterator FileStorage::end() const {
    return FileStorage::Iterator();
}



FileStorage::Iterator::Iterator() {

}

bool FileStorage::Iterator::operator==(const FileStorage::Iterator& iterator) const {
    return false;
}

bool FileStorage::Iterator::operator!=(const FileStorage::Iterator& iterator) const {
    return false;
}

Message FileStorage::Iterator::operator*() const {
    return *Message::fromString("");
}

Message FileStorage::Iterator::operator->() const {
    return *Message::fromString("");
}

FileStorage::Iterator& FileStorage::Iterator::operator++() {
    return *this;
}

