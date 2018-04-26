//
// Created by Martyna on 26.04.18.
//
#include "../../inc/packages/Message.h"

char * Message::serialize() const {};

void  Message::deserialize(const char *) {};

uint32_t  Message::getSerializedMessageSize() const {
    return 0;
};