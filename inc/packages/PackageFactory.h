//
// Created by Martyna on 26.04.18.
//

#ifndef TIN_PACKAGEFACTORY_H
#define TIN_PACKAGEFACTORY_H

#include "Package.h"
#include <map>

class PackageFactory {

public:
    static PackageFactory *instance() {
        static PackageFactory instance;
        return &instance;
    }


    Package *create(uint32_t typeId);

    std::string generateSerializationString(uint32_t typeId, Package *package);

    Package *createPackageFromSerializationString(const std::string &serializationString);

    template<class T>
    void registerPackage(uint32_t typeId) {
        constructorsMap[typeId] = [](void) -> Package * { return new T(); };
        deserializingFunctionsMap[typeId] = [](Package *basePackage, const std::string &serialisationString) -> void {
            boost::iostreams::basic_array_source<char> device(serialisationString.data(), serialisationString.size());
            boost::iostreams::stream<boost::iostreams::basic_array_source<char> > a(device);
            boost::archive::binary_iarchive inputArchive(a);
            inputArchive & *(basePackage);
            inputArchive & *(static_cast<T *>(basePackage));
        };
        serializingFunctiosMap[typeId] = [](Package *basePackage) -> std::string {
            std::string serialisationString;
            boost::iostreams::back_insert_device<std::string> inserter(serialisationString);
            boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
            boost::archive::binary_oarchive outputArchive(s);
            outputArchive & *basePackage;
            outputArchive & *(static_cast<T *>(basePackage));
            return serialisationString;
        };
    }

private:
    std::map<uint32_t, std::function<Package *(void)>> constructorsMap;
    std::map<uint32_t, std::function<void(Package *basePackage, const std::string &serializationStr)>> deserializingFunctionsMap;
    std::map<uint32_t, std::function<std::string(Package *basePackage)>> serializingFunctiosMap;

    Package *createFromSerializationString(uint32_t typeId, const std::string &serializationString);
};


#endif //TIN_PACKAGEFACTORY_H
