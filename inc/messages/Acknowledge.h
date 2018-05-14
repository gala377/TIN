//
// Created by gajus123 on 14.05.18.
//

#ifndef TIN_ACKNOWLEDGE_H
#define TIN_ACKNOWLEDGE_H

#include "Message.h"

class Acknowledge : public Message {
public:
    Acknowledge(uint32_t packet_id);

    uint32_t getConsumedPacketId() const;
private:
    uint32_t consumed_packet_id_;

    Acknowledge() {}
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Message>(*this);
        ar & consumed_packet_id_;
    }
};

#endif //TIN_ACKNOWLEDGE_H