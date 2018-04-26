//
// Created by Martyna on 25.04.18.
//

#ifndef TIN_MESSAGE_H
#define TIN_MESSAGE_H

#include <stdint.h>

class Message {
public:
    virtual char *serialize() const;

    virtual void deserialize(const char *serialized);

    virtual uint32_t getSerializedMessageSize() const;
};

#endif //TIN_MESSAGE_H
