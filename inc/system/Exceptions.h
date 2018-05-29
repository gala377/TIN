//
// Created by Martyna on 29.05.18.
//

#ifndef TIN_EXCEPTIONS_H
#define TIN_EXCEPTIONS_H

#include <exception>
#include <string>

namespace System {

    class SystemError: public std::exception {
    public:
        virtual const char* what() const throw();
    };

    class NoMessageToRead: public SystemError {
    public:
        NoMessageToRead() = default;

        virtual const char* what() const throw();
    };

    class CanNotConnect: public SystemError {
    public:
        CanNotConnect() = default;

        virtual const char *what() const throw();
    };

    class UnableToSentMessageClosedConnection: public SystemError {
    public:
        UnableToSentMessageClosedConnection() = default;

        virtual const char *what() const throw();
    };
}
#endif //TIN_EXCEPTIONS_H
