//
// Created by gajus123 on 25.04.18.
//

#ifndef TIN_SOCKETUNIX_H
#define TIN_SOCKETUNIX_H

#include "SocketFacade.h"

class SocketUnix {
public:
    virtual int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
    virtual int accept(int s, struct sockaddr * restrict_addr, socklen_t * restrict_addrlen);
    virtual int connect(int s, const struct sockaddr *name, socklen_t namelen);
    virtual int socket(int domain, int type, int protocol);
    virtual int listen(int s, int backlog);
    virtual int close(int fd);
    virtual int getErrno();
};


#endif //TIN_SOCKETUNIX_H
