//
// Created by danter on 2017-09-01.
//

#ifndef GCF_SERVER_ORDERDELETED_H
#define GCF_SERVER_ORDERDELETED_H

#include "MitchTypes.h"
#include "MitchMessage.h"
class OrderDeleted : public MitchMessage {

public:
    OrderDeleted(mitch::types::UInt32 nano,
            mitch::types::UInt64 orderId, mitch::types::UInt32 instrumentId);
private:
    mitch::types::UInt32 nanoSeconds;
    mitch::types::UInt64 orderId;
    mitch::types::UInt32 instrumentId;

};

#endif //GCF_SERVER_ORDERDELETED_H
