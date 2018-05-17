#ifndef PACKAGE_H
#define PACKAGE_H

#include <iostream>
#include <string>
#include <regex>

#define REGEX_PATERN "(\\d+)\\|(.*)"

class Package
{
private:
    int id = 0;
    std::string value;
public:
    Package();

    Package(int, std::string);

    Package(std::string);

    void setId(int id);

    int getId();

    void setValue(const std::string &value);

    std::string getValue();

    std::string toStr();
};

#endif //PACKAGE_H