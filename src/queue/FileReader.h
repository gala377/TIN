#ifndef FILEREADER_H
#define FILEREADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include "Package.h"

#define TEMP_NAME "temp.tmp"


class FileReader
{
public:
    static void write(std::string, Package*);

    static void remove(std::string, Package*);

    static std::list<Package*> getAll(const std::string&);
};

#endif //FILEREADER