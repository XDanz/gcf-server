//
// Created by danter on 2017-09-01.
//

#ifndef GCF_SERVER_MITCHMESSAGE_H
#define GCF_SERVER_MITCHMESSAGE_H

#include "MessageHeader.h"
// BaseMessage
enum CATEGORY {
    HEADER, ADMINISTRATIVE, APPLICATION
};

const int ORDER_DELETED_MESSAGE_LENGTH = 17;

class MitchMessage {

public:
    MitchMessage(int messageLength, MESSAGE_TYPE message_type, CATEGORY category);

    int getMessageLength() const;

    MESSAGE_TYPE getMessage_type() const;

protected:
    int messageLength;
    MESSAGE_TYPE messageType;
    CATEGORY category;
};

#endif //GCF_SERVER_MITCHMESSAGE_H
