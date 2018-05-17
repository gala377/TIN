

#include "Package.h"

Package::Package() : id(id), value("") {}

Package::Package(int id, std::string value) : id(id), value(std::move(value)) {}

Package::Package(std::string data) {
    std::regex pattern(REGEX_PATERN);
    std::smatch match;

    std::regex_search(data, match, pattern);
    if (match.size() == 3) {
        auto it = match.begin();
        it++;
        id = std::stoi(*it);
        it++;
        value = *it;
    }
}


void Package::setId(int id) {
    this->id = id;
}

int Package::getId() {
    return id;
}

void Package::setValue(const std::string &value) {
    this->value = value;
}

std::string Package::getValue() {
    return value;
}

std::string Package::toStr() {
    return std::to_string(id) + "|" + value;
}