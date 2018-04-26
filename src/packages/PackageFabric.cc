//
// Created by Martyna on 24.04.18.
//

#include "../../inc/packages/PackageFabric.h"

Package createPackage(const Message &message) {
    return createPackage(message, 0);
}

Package createPackage(const NumericalMessage &message) {
    return createPackage(message, 1);
}

Package createPackage(const Message &message, uint32_t typId) {
    char *serialized_str = message.serialize();
    Package package = Package(typId, message.getSerializedMessageSize(), serialized_str);
    return package;
}

Message *deserializePackage(const Package &package) {
    Message *message;
    switch (package.getTypeId()) {
        case 1:
            message = new NumericalMessage;
            break;
        default:
            message = new Message;
            break;
    }
    message->deserialize(package.getData());
    return message;
}