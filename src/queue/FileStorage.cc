//
// Created by igor on 21.05.18.
//

#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <queue/FileStorage.h>
#include <queue/Exceptions.h>

using namespace Queue;
namespace fs = boost::filesystem;


FileStorage::FileStorage(std::string&& root_path): _root(root_path) {
    boost::filesystem::create_directory(_root);
    _files = listFiles();
}

FileStorage::FileStorage(const std::string& root_path): _root(root_path) {
    boost::filesystem::create_directory(_root);
    _files = listFiles();
}


void FileStorage::add(const Message& mess) {
    auto file_path = makePath(mess);

    if(_files.find(file_path) != _files.end()){
        throw FileExists(file_path);
    }

    mess.toFile(file_path);

    _files.insert(file_path);
}

void FileStorage::add(int id, const std::string& data) {
    auto file_path = makePath(id);

    if (_files.find(file_path) != _files.end()) {
        throw FileExists(file_path);
    }

    std::fstream file;
    file.open(file_path, std::fstream::out);
    file << data;
    file.close();
    _files.insert(file_path);
}

void FileStorage::remove(int id) {
    auto file_path = makePath(id);
    if(_files.find(file_path) == _files.end()) {
        throw FileDoesNotExist(file_path);
    }

    boost::filesystem::remove(
            boost::filesystem::path(file_path));

    _files.erase(file_path);
}

std::vector<std::shared_ptr<Message>> FileStorage::getAll() {
    std::vector<std::shared_ptr<Message>> mess;
    for(auto it = begin(); it != end(); ++it) {
        mess.push_back(*it);
    }
    return mess;
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


std::uint32_t FileStorage::lastAddedId() const {
    if(!cachedFilesCount()) {
        return 0;
    }
    std::string last_mess_path = *(--_files.end());
    std::string last_mess_file_name = last_mess_path.substr(
            last_mess_path.find_last_of('/')+1);
    std::cout << "Last file name is " << last_mess_file_name << "\n";
    return std::stoi(last_mess_file_name);
}

std::uint32_t FileStorage::cachedFilesCount() const {
    return _files.size();
}

std::set<std::string> FileStorage::listFiles() const {
    std::set<std::string> files;
    for(auto& p: boost::make_iterator_range(fs::directory_iterator(_root))) {
        files.insert(p.path().string());
    }
    return files;
}

void FileStorage::updateCachedFiles() {
    _files = listFiles();
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

std::shared_ptr<Message> FileStorage::Iterator::operator*() const {
    return readFile();
}

std::shared_ptr<Message> FileStorage::Iterator::readFile() const {
    return Message::fromFile(*_curr);
}

FileStorage::Iterator& FileStorage::Iterator::operator++() {
    ++_curr;
}

