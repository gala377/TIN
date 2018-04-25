//
// Created by gajus123 on 25.04.18.
//

#ifndef TIN_SOCKETUNIX_H
#define TIN_SOCKETUNIX_H

#include "SocketFacade.h"

class SocketUnix : public SocketFacade {
public:
    virtual int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
    virtual int accept(int s, struct sockaddr * restrict_addr, socklen_t * restrict_addrlen);
    virtual int connect(int s, const struct sockaddr *name, socklen_t namelen);
    virtual int socket(int domain, int type, int protocol);
    virtual int listen(int s, int backlog);
    virtual int close(int fd);
    virtual int bind(int s, const struct sockaddr *addr, socklen_t addrlen);
    virtual int getErrno();
    virtual ssize_t write(int fd, const void *buf, size_t nbytes);
    virtual ssize_t read(int fd, void *buf, size_t nbytes);
    virtual int setFlags(int fd, int flags);
    virtual int availableBytes(int fd);
    virtual uint16_t port(int fd);
    virtual void setAsyncIO(int fd, pid_t process_pid);
    virtual void disableAsyncIO(int fd);
};


#endif //TIN_SOCKETUNIX_H
