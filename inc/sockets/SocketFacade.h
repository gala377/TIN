//
// \author Jakub Gajownik
// \date 25.04.2018
//

#ifndef TIN_SOCKETFACADE_H
#define TIN_SOCKETFACADE_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

namespace Sockets {
    class SocketFacade {
    public:
        virtual int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) = 0;
        virtual int accept(int s, struct sockaddr * restrict_addr, socklen_t * restrict_addrlen) = 0;
        virtual int connect(int s, const struct sockaddr *name, socklen_t namelen) = 0;
        virtual int socket(int domain, int type, int protocol) = 0;
        virtual int listen(int s, int backlog) = 0;
        virtual int close(int fd) = 0;
        virtual int bind(int s, const struct sockaddr *addr, socklen_t addrlen) = 0;
        virtual int getErrno() = 0;
        virtual ssize_t write(int fd, const void *buf, size_t nbytes) = 0;
        virtual ssize_t read(int fd, void *buf, size_t nbytes) = 0;
        virtual int setFlags(int fd, int flags) = 0;
        virtual int availableBytes(int fd) = 0;
        virtual uint16_t port(int fd) = 0;
        virtual void setAsyncIO(int fd, pid_t process_pid) = 0;
        virtual void disableAsyncIO(int fd) = 0;
    };
}


#endif //TIN_SOCKETFACADE_H
