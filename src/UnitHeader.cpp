//
// Created by danter on 2017-09-01.
//

#include "UnitHeader.h"
UnitHeader::UnitHeader(const mitch::types::UInt16& length, const mitch::types::UInt8& messageCount,
        char marketDataGroup, const mitch::types::UInt8& sequenceNumber)
        :length(length), messageCount(messageCount), marketDataGroup(marketDataGroup),
         sequenceNumber(sequenceNumber) {

}
