//
// Created by danter on 2017-09-01.
//

#ifndef GCF_SERVER_UINT8_H
#define GCF_SERVER_UINT8_H

namespace mitch {
    namespace types {

        class UInt8 {

        private:
            unsigned int value;
        };


        class UInt16 {

        private:
            unsigned int value;
        };

        class UInt32 {
        private:
            UInt32(unsigned int v) : value(v) { }
            static UInt32 value_of (unsigned int v) {
                return UInt32(v);
            }
        public:
            unsigned int value;
        };

        class UInt64 {

        private:
            unsigned int value;
        };

    };
};

#endif //GCF_SERVER_UINT8_H
