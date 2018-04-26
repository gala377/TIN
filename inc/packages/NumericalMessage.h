//
// Created by Martyna on 26.04.18.
//

#ifndef TIN_NUMERICALMESSAGE_H
#define TIN_NUMERICALMESSAGE_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "Message.h"

class NumericalMessage : public Message {
private:
    int32_t number_;

public:
    NumericalMessage() {};
    NumericalMessage(uint32_t num);

    char *serialize() const;
    void deserialize(const char *serialized);

    size_t getSerializedStrSize() const;
    int32_t getNum() const;

};

#endif //TIN_NUMERICALMESSAGE_H
