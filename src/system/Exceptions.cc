//
// Created by Martyna on 29.05.18.
//

#include <system/Exceptions.h>

using namespace System;

const char* SystemError::what() const throw() {
    return "Gryphon module error";
}

const char* NoMessageToRead::what() const throw() {
    std::string info = SystemError::what();
    info +=  "No message to read!";
    return info.c_str();
}

const char* ConnectionTimeOut::what() const throw() {
    std::string info = SystemError::what();
    info +=  "Can not connect";
    return info.c_str();
}

const char* UnableToSentMessageConnectionClosed::what() const throw() {
    std::string info = SystemError::what();
    info +=  "Unable to sent message. Connection is closed";
    return info.c_str();
}

const char* UnknownMessageAcknowledge::what() const throw() {
    std::string info = SystemError::what();
    info +=  "Received Acknowledge to unknown message";
    return info.c_str();
}

const char* UnsupportedOption::what() const throw() {
    std::string info = SystemError::what();
    info +=  "Option not supported";
    return info.c_str();
}