//
// Created by danter on 2017-09-01.
//

#ifndef GCF_SERVER_MITCHOUTPUT_H
#define GCF_SERVER_MITCHOUTPUT_H

#include <cstddef>
#include "MitchTypes.h"
class MitchOutput {

public:
    size_t writeAlpha(const char* buf, char* ibuf, size_t size) {
        for (size_t i = 0; i < size; i++)
            ibuf[i] = buf[i];
        return size;
    }

    size_t writeUInt32(const mitch::types::UInt32& uInt32, char* ibuf) {
        unsigned int val = uInt32.value;
        for (size_t i = 0; i < sizeof(val); i++)
            ibuf[i] = (char)(0xFF & (val >> (i * 8)) );
    }

};

#endif //GCF_SERVER_MITCHOUTPUT_H
