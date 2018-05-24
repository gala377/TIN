#ifndef TIN_QUEUE_EXCEPTIONS_H
#define TIN_QUEUE_EXCEPTIONS_H

#include <exception>
#include <string>

//todo add cpp file to cmake

namespace Queue {

    class StorageError: public std::exception {
    public:
        virtual const char* what() const throw();
    };

    class FileExists: public StorageError {
    public:
        FileExists(std::string file_path);

        virtual const char* what() const throw();
    protected:
        std::string _file_path;
    };

    class FileDoesNotExist: public FileExists {
    public:
        using FileExists::FileExists;

        virtual const char* what() const throw();
    };


}

#endif