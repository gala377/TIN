//
// Created by gajus123 on 25.04.18.
//

#include "../../inc/sockets/SocketUnix.h"

int SocketUnix::select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
    return ::select(nfds, readfds, writefds, exceptfds, timeout);
}

int SocketUnix::accept(int s, struct sockaddr * restrict_addr, socklen_t * restrict_addrlen) {
    return ::accept(s, restrict_addr, restrict_addrlen);
}

int SocketUnix::connect(int s, const struct sockaddr *name, socklen_t namelen) {
    return ::connect(s, name, namelen);
}

int SocketUnix::socket(int domain, int type, int protocol) {
    return ::socket(domain, type, protocol);
}

int SocketUnix::listen(int s, int backlog) {
    return ::listen(s, backlog);
}

int SocketUnix::close(int fd) {
    return ::close(fd);
}

int SocketUnix::getErrno() {
    return errno;
}

ssize_t SocketUnix::write(int fd, const void *buf, size_t nbytes) {
    return ::write(fd, buf, nbytes);
}

ssize_t SocketUnix::read(int fd, void *buf, size_t nbytes) {
    return ::read(fd, buf, nbytes);
}

int SocketUnix::setFlags(int fd, int flags) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | flags);
}

int SocketUnix::availableBytes(int fd) {
    int count = 0;
    ioctl(fd,  FIONREAD, &count);
    return count;
}