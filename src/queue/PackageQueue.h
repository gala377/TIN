#ifndef PACKAGEQUEUE_H
#define PACKAGEQUEUE_H

#include <list>
#include "Package.h"
#include "FileReader.h"

class PackageQueue {
private:
    std::list<Package *> sent;
    std::list<Package *> waiting;
    std::string backup_file;
public:
    explicit PackageQueue(const std::string &);

    PackageQueue(const std::string &, std::list<Package *>);

    void push(Package *);

    Package* pop();

    void remove(int);

    bool isEmpty();

    bool isSentEmpty();

    unsigned long getSize();

    unsigned long getWaitingSize();

    void pushWithoutSaving(Package *);

};

#endif //PACKAGEQUEUE_H
