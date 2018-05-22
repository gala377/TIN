//
// Created by gajus123 on 22.05.18.
//

#ifndef TIN_FDSET_H
#define TIN_FDSET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory.h>
#include <sys/ioctl.h>

class FDSet {
public:
    FDSet();
    ~FDSet();

    fd_set* getRead();
    fd_set* getWrite();
    fd_set* getExcept();

    void addRead(int fd);
    bool isSetRead(int fd);
    void addWrite(int fd);
    bool isSetWrite(int fd);
    void addExcept(int fd);
    bool isSetExcept(int fd);

    int getBiggestDescriptor();
private:
    fd_set read_;
    fd_set write_;
    fd_set except_;

    int biggest_descriptor_;
};

#endif //TIN_FDSET_H