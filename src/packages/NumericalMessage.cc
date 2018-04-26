//
// Created by Martyna on 25.04.18.
//

#include "../../inc/packages/NumericalMessage.h"

NumericalMessage::NumericalMessage(uint32_t num) : number_(num) {
}

char *NumericalMessage::serialize() const {
    char *data = new char[getSerializedStrSize()];
    memcpy(data, &number_, sizeof(number_));
    return data;
}

void NumericalMessage::deserialize(const char *serialized){
    memcpy(&number_, serialized, 4);
}

size_t NumericalMessage::getSerializedStrSize()const {
    return sizeof(number_);
}

int32_t NumericalMessage::getNum() const{
    return number_;
}
