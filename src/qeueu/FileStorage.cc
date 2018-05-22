//
// Created by igor on 21.05.18.
//

#include <fstream>
#include <iostream>
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
    std::fstream file;
    file.open(makePath(mess), std::fstream::out);
    boost::archive::text_oarchive o_archive(file);
    o_archive << mess;
    file.close();
    _files.insert(makePath(mess));
}

void FileStorage::add(int id, const std::string& data) {
    std::fstream file;
    file.open(makePath(id), std::fstream::out);
    file << data;
    file.close();
    _files.insert(makePath(id));
}

void FileStorage::remove(int id) {
    // todo error if file doesn't exists
    boost::filesystem::remove(
            boost::filesystem::path(makePath(id)));
    _files.erase(makePath(id));
}

std::vector<Message> FileStorage::getAll() {
    // todo implement me
    return std::vector<Message>();
}


std::string FileStorage::makePath(int id) const {
    return _root.string() + "/" + padInt(id);
}

std::string FileStorage::makePath(const Message& mess) const {
    return _root.string() + "/" + padInt(mess.id_);
}

std::string FileStorage::padInt(int num) const {
    auto padded = std::to_string(num);
    padded.insert(0, 20U - std::min(std::string::size_type(20), padded.length()), '0');
    return padded;
}


std::set<std::string> FileStorage::listFiles() const {
    std::set<std::string> files;
    for(auto& p: boost::make_iterator_range(fs::directory_iterator(_root))) {
        files.insert(p.path().string());
    }
    return files;
}


FileStorage::Iterator FileStorage::begin() const {
    return FileStorage::Iterator(_files.begin(), *this);
}

FileStorage::Iterator FileStorage::end() const {
    return FileStorage::Iterator(_files.end(), *this);
}


FileStorage::Iterator::Iterator(std::set<std::string>::const_iterator curr_file,
                                const FileStorage& parent): _parent(parent), _curr(curr_file) { }

bool FileStorage::Iterator::operator==(const FileStorage::Iterator& iterator) const {
    return _curr == iterator._curr;
}

bool FileStorage::Iterator::operator!=(const FileStorage::Iterator& iterator) const {
    return !(*this == iterator);
}

Message* FileStorage::Iterator::operator*() const {
    return readFile();
}

Message* FileStorage::Iterator::operator->() const {
    return readFile();
}

Message* FileStorage::Iterator::readFile() const {
    std::fstream file(*_curr, std::fstream::in);
    std::string data;
    file >> data;
    return Message::fromString(data);
}

FileStorage::Iterator& FileStorage::Iterator::operator++() {
    ++_curr;
}

