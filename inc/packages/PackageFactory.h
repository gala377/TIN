//
// Created by Martyna on 26.04.18.
//

#ifndef TIN_PACKAGEFACTORY_H
#define TIN_PACKAGEFACTORY_H

#include "Package.h"
#include "Acknowledge.h"
#include <map>

class PackageFactory {

public:
    static PackageFactory *instance() {
        static PackageFactory instance;
        return &instance;
    }

    template<class Archive>
    Package *createFromArchive(uint32_t typeId, Archive &ar) {
        Package *package = create(typeId);
//tu by sie rzutowanie przydalo
//        Acknowledge* receivedAck = static_cast<Acknowledge*>(package);
//        ar & *receivedAck;
        ar & package;
        return package;
    }

    template<class Archive>
    Package *createFromArchive(Archive &ar) {
        Package package;
        ar & package;
        return createFromArchive(package.typeId_, ar);
    }

    Package *create(uint32_t typeId);

    std::string generateSerializationString(uint32_t typeId, Package *package);

    Package *createFromSerializationString(uint32_t typeId, const std::string &serializationString);

    template<class T>
    void registerPackage(uint32_t typeId) {
        map[typeId] = [](void) -> Package* { return new T();};
    }

private:
    std::map<uint32_t, std::function<Package *(void)>> map;
};


#endif //TIN_PACKAGEFACTORY_H
