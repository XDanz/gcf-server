//
// Created by danter on 2018-01-19.
//

#ifndef GCF_SERVER_SOUPBINSEQUENCED_H
#define GCF_SERVER_SOUPBINSEQUENCED_H
#include <string>
#include "SoupBinHeader.h"

namespace soupbin {
    class SoupBinSequenced
    {
    public:
        SoupBinSequenced (const std::string& payload): payload(payload)
        {
            hdr = new SoupBinHeader { SEQUENCED_MSG, payload.size()+1};
        }

        SoupBinHeader* getHdr() const
        {
            return hdr;
        }

        virtual ~SoupBinSequenced()
        {
            delete hdr;
        }

        const std::string& getPayload() const
        {
            return payload;
        }

    private:
        const std::string& payload;
        soupbin::SoupBinHeader* hdr;
    };
}

#endif //GCF_SERVER_SOUPBINSEQUENCED_H
