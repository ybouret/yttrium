#include "y/text/base64/decode.hpp"
#include "y/system/exception.hpp"
#include "y/text/ascii/printable.hpp"

namespace Yttrium
{

    namespace Base64
    {
        
        const char * const Decode::CallSign = "Base64::Decode";

        const int8_t Decode::Table[256] =
        {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, 62, -1, 63,
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
            -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, 63,
            -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
        };


        static inline
        uint8_t Byte(const char c)
        {
            const uint8_t u = c;
            const int8_t  i = Decode::Table[u];
            if(i<0) throw Specific::Exception(Decode::CallSign, "invalid char '%s'", ASCII::Printable::Char[u]);
            return uint8_t(i);
        }

        static const uint8_t maskLo2 = 0x03;
        static const uint8_t maskHi6 = 0xfc;
        static const uint8_t maskLo4 = 0x0f;
        static const uint8_t maskHi4 = 0xf0;
        static const uint8_t maskHi2 = 0xc0;
        static const uint8_t maskLo6 = 0x3f;

        void Decode:: _1(uint8_t code[], const char c0, const char c1)
        {
            assert(0!=code);
            const uint8_t b0 = Byte(c0); assert(b0<64);
            const uint8_t b1 = Byte(c1); assert(b1<64);
            if(0 != (b1&maskLo4) )
                throw Specific::Exception(Decode::CallSign,
                                          "invalid second in '%s%s'",
                                          ASCII::Printable::Char[uint8_t(c0)],
                                          ASCII::Printable::Char[uint8_t(c1)]);
            code[0] = (b0<<2) | (b1>>4);
        }

    }
}



