//
// Created by danter on 2017-09-01.
//

#include "MitchMessage.h"
MitchMessage::MitchMessage(int messageLength, MESSAGE_TYPE message_type, CATEGORY category)
        :messageLength(messageLength), messageType(message_type) , category(category)
{ }

int MitchMessage::getMessageLength() const
{
    return messageLength;
}

MESSAGE_TYPE MitchMessage::getMessage_type() const
{
    return messageType;
}
