//
// \author Jakub Gajownik
// \date 22.05.2018
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
namespace Sockets {
    /*!
     * \class FDSet
     * \brief fd_set wrapper
     *
     * FDSet is a facade for UNIX fd_set structure
     */
    class FDSet {
    public:
        FDSet();
        ~FDSet();

        fd_set* getRead();
        fd_set* getWrite();
        fd_set* getExcept();

        /*!
         * Adds descriptor to read set
         * @param fd File descriptor
         */
        void addRead(int fd);
        /*!
         * Adds descriptor to write set
         * @param fd File descriptor
         */
        void addWrite(int fd);
        /*!
         * Adds descriptor to except set
         * @param fd File descriptor
         */
        void addExcept(int fd);

        /*!
         * @param fd File descriptor
         * @return Whether descriptor was active in read set
         */
        bool isSetRead(int fd);
        /*!
         * @param fd File descriptor
         * @return Whether descriptor was active in write set
         */
        bool isSetWrite(int fd);
        /*!
         * @param fd File descriptor
         * @return Whether descriptor was active in except set
         */
        bool isSetExcept(int fd);

        /*!
         * @return Biggest descriptor which was added
         */
        int getBiggestDescriptor();
    private:
        fd_set read_;
        fd_set write_;
        fd_set except_;

        int biggest_descriptor_;
    };
}


#endif //TIN_FDSET_H