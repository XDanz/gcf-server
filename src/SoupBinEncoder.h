//
// Created by danter on 2018-01-15.
//

#include <string>

namespace soupbin {
    size_t encode(char* buf, const std::string& line);
    size_t decode_length(const char* buf);
}


