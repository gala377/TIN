#include "sockets/FDSet.h"

FDSet::FDSet() :
    biggest_descriptor_(0) {
    FD_ZERO(&read_);
    FD_ZERO(&write_);
    FD_ZERO(&except_);
}

FDSet::~FDSet() {
}

void FDSet::addRead(int fd) {
    biggest_descriptor_ = (fd > biggest_descriptor_) ? fd : biggest_descriptor_;
    FD_SET(fd, &read_);
}

bool FDSet::isSetRead(int fd) {
    return FD_ISSET(fd, &read_);
}

void FDSet::addWrite(int fd) {
    biggest_descriptor_ = (fd > biggest_descriptor_) ? fd : biggest_descriptor_;
    FD_SET(fd, &write_);
}

bool FDSet::isSetWrite(int fd) {
    return FD_ISSET(fd, &write_);
}

void FDSet::addExcept(int fd) {
    biggest_descriptor_ = (fd > biggest_descriptor_) ? fd : biggest_descriptor_;
    FD_SET(fd, &except_);
}

bool FDSet::isSetExcept(int fd) {
    return FD_ISSET(fd, &except_);
}

fd_set* FDSet::getRead() {
    return &read_;
}

fd_set* FDSet::getWrite() {
    return &write_;
}

fd_set* FDSet::getExcept() {
    return &except_;
}

int FDSet::getBiggestDescriptor() {
    return biggest_descriptor_;
}