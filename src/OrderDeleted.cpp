//
// Created by danter on 2017-09-01.
//

#include "OrderDeleted.h"

OrderDeleted::OrderDeleted(mitch::types::UInt32 nano, mitch::types::UInt64 orderId,
         mitch::types::UInt32 instrumentId)
        : MitchMessage(ORDER_DELETED_MESSAGE_LENGTH, ORDER_DELETED, APPLICATION),
          nanoSeconds(nano), orderId(orderId), instrumentId(instrumentId)
{

}
