//
// Created by Martyna on 11.05.18.
//

#include "../../inc/packages/Acknowledge.h"

#include <boost/serialization/export.hpp>

uint32_t Acknowledge::getAcknowledgedPackedId() const {
    return acknowledgedPackedId_;
}

Acknowledge::Acknowledge(uint32_t acknowledgedPackedId_) : acknowledgedPackedId_(acknowledgedPackedId_) {}

BOOST_CLASS_EXPORT(Acknowledge)