//
// Created by igor on 21.05.18.
//

#include <queue/FileStorage.h>

using namespace Queue;


FileStorage::FileStorage(std::string&& file_name): _file(
        std::move(file_name), std::fstream::in|std::fstream::out) {
}

FileStorage::FileStorage(const std::string& file_name): _file(
        file_name, std::fstream::in|std::fstream::out) {
}

void FileStorage::add(Message mess) {
    
}

void FileStorage::remove(int id) {

}

std::vector<Message> FileStorage::getAll() {
    return std::vector<Message>();
}

FileStorage::Iterator FileStorage::begin() {
    return FileStorage::Iterator();
}

FileStorage::Iterator FileStorage::end() {
    return FileStorage::Iterator();
}
