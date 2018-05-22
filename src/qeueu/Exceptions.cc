#include <queue/Exceptions.h>

using namespace Queue;

const char* StorageError::what() const {
    return "Storage error";
}

FileExists::FileExists(std::string file_path): _file_path(
    std::move(file_path)) {};

const char* FileExists::what() const {
    std::string info = StorageError::what();
    info +=  " File " + _file_path + " already exists!";
    return info.c_str();
}

const char* FileDoesNotExist::what() const {
    std::string info = StorageError::what();
    info += " File " + _file_path + " doesn't exist!";\
    return info.c_str();
}