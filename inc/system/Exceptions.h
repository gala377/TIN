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

    class ConnectionTimeOut: public SystemError {
    public:
        ConnectionTimeOut() = default;

        virtual const char *what() const throw();
    };

    class UnableToSentMessageConnectionClosed: public SystemError {
    public:
        UnableToSentMessageConnectionClosed() = default;

        virtual const char *what() const throw();
    };

    class UnknownMessageAcknowledge: public SystemError {
    public:
        UnknownMessageAcknowledge() = default;

        virtual const char *what() const throw();
    };

    class UnsupportedOption: public SystemError {
    public:
        UnsupportedOption() = default;

        virtual const char *what() const throw();
    };
}
#endif //TIN_EXCEPTIONS_H
