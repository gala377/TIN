//
// Created by gajus123 on 25.04.18.
//

#include "sockets/SocketUnix.h"

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

int SocketUnix::bind(int s, const struct sockaddr *addr, socklen_t addrlen) {
    return ::bind(s, addr, addrlen);
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

uint16_t SocketUnix::port(int fd) {
    struct sockaddr_in6 address;
    socklen_t length = sizeof(address);
    getsockname(fd, (struct sockaddr *) &address, &length);
    return ntohs(address.sin6_port);
}

void SocketUnix::setAsyncIO(int fd, pid_t process_pid) {
    ioctl(fd, SIOCSPGRP, &process_pid);
    int on = 1;
    ioctl(fd, FIOASYNC, &on);
}

void SocketUnix::disableAsyncIO(int fd) {
    int on = 0;
    ioctl(fd, FIOASYNC, &on);
}