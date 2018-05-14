//
// Created by gajus123 on 14.05.18.
//

#include "../../inc/messages/Acknowledge.h"

Acknowledge::Acknowledge(uint32_t packet_id) :
    consumed_packet_id_(packet_id) {
}

uint32_t Acknowledge::getConsumedPacketId() const {
    return consumed_packet_id_;
}

BOOST_CLASS_EXPORT(Acknowledge)