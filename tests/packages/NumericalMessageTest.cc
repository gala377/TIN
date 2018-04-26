//
// Created by Martyna on 26.04.18.
//

#include <gtest/gtest.h>
#include <string>
#include "../../inc/packages/NumericalMessage.h"

TEST(MessageAfterSeralizationHasNotBeenCHanged, PositiveNos) {

    NumericalMessage message(206);
    NumericalMessage newMessage;
    newMessage.deserialize(message.serialize());

    ASSERT_EQ(message.getNum(), newMessage.getNum());
}
