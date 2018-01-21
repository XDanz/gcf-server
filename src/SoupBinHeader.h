//
// Created by danter on 2018-01-20.
//

#ifndef GCF_SERVER_SOUPBINHEADER_H
#define GCF_SERVER_SOUPBINHEADER_H
#include <cstdint>

namespace soupbin {
    enum MSG_TYPE {SEQUENCED_MSG = 'S', END_OF_SESSION = 'Z'};

    class SoupBinHeader
    {
    public:
        SoupBinHeader(MSG_TYPE msg_type, unsigned long payload_length):
                msg_type(msg_type), payload_length(payload_length)
        {}
        const uint16_t& getPayload_length() const
        {
            return payload_length;
        }

    private:
        const MSG_TYPE msg_type;
        const uint16_t& payload_length;
    };
}

#endif //GCF_SERVER_SOUPBINHEADER_H
