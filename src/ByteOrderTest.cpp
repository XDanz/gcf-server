//
// Created by danter on 2017-09-02.
//

#include <limits.h>
#include <stdint.h>
#include <iostream>

#if CHAR_BIT != 8
#error "unsupported char size"
#endif

// 00 00 00 30
// 00 00 03 02
enum
{
    O32_LITTLE_ENDIAN = 0x03020100ul,
    O32_BIG_ENDIAN = 0x00010203ul,
    O32_PDP_ENDIAN = 0x01000302ul
};

static const union {
    unsigned char bytes[4];
    uint32_t value;
} o32_host_order =
        { { 0, 1, 2, 3 } };

union RES {
    unsigned char bytes[4];
    uint32_t value;
};


#define O32_HOST_ORDER (o32_host_order.value)

int
main (int argc, char *argv[])
{
    if (O32_HOST_ORDER == O32_LITTLE_ENDIAN) {
        std::cout<< "Little endian" << std::endl;
        std::cout<< "Val " << O32_LITTLE_ENDIAN << std::endl;

        unsigned val  = O32_LITTLE_ENDIAN;
        RES r;
        r.bytes;
        for (size_t i  = 0; i < sizeof(val); i++)
            r.bytes[i] = (unsigned char)( val >> (24 - (CHAR_BIT * i)));


        std::cout<< "Big endian" << std::endl;
        std::cout<< "Val " << r.value << std::endl;

    } else
        std::cout<< "Big endian" << std::endl;



}