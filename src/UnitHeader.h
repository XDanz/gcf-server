//
// Created by danter on 2017-09-01.
//

#ifndef GCF_SERVER_UNITHEADER_H
#define GCF_SERVER_UNITHEADER_H
#include "MitchTypes.h"

class UnitHeader {

public:
    UnitHeader(const mitch::types::UInt16& length, const mitch::types::UInt8& messageCount, char marketDataGroup,
            const mitch::types::UInt8& sequenceNumber);
    const int headerLength = 8;

private:
    mitch::types::UInt16 length;  /// Total length of the Unit including this header and all payload messages
    mitch::types::UInt8 messageCount;  // Message count
    char marketDataGroup;
    mitch::types::UInt8 sequenceNumber;  /// SequenceNumber

};

#endif //GCF_SERVER_UNITHEADER_H
