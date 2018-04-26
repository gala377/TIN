//
// Created by Martyna on 26.04.18.
//

#ifndef TIN_PACKAGEFACTORY_H
#define TIN_PACKAGEFACTORY_H


#include "NumericalMessage.h"
#include "Package.h"
#include "Message.h"

Package createPackage(const Message &message, uint32_t typeId);
Package createPackage(const Message &message);
Package createPackage(const NumericalMessage &message);

Message *deserializePackage(const Package &pack);


#endif //TIN_PACKAGEFACTORY_H
