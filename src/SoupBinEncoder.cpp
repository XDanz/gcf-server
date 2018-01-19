//
// Created by danter on 2018-01-15.
//

#include <string>
#include <cstring>
#include <netinet/in.h>
#include <iostream>
#include "SoupBinEncoder.h"

size_t soupbin::encode(char* buf, const std::string& line)
{
    size_t sz = line.size() + 1;
    uint16_t a = htons(uint16_t(sz));

    auto *ptr = reinterpret_cast<char*>(&a);
    int i = 0;
    for (; i <sizeof(short);i++ ) buf[i] = *ptr++;
    buf[i++] = 'S';

    for (char it : line) buf[i++] = it;
    return sz+2;
}

size_t soupbin::decode_length(const char* buf)
{
    return ntohs(*reinterpret_cast<const uint16_t*>(buf));

};