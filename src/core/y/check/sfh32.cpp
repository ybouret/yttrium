
#include "y/check/sfh32.hpp"

namespace Yttrium
{

    static uint32_t get16bits(const uint8_t *data) noexcept
    {
        uint32_t res = data[1];
        res <<= 8;
        res |=  data[0];
        return res;
    }

    uint32_t SFH32::Run(uint32_t hash, const void *buf, size_t len) noexcept
    {
        assert( Good(buf,len) );
        const uint8_t *data = static_cast<const uint8_t*>(buf);
        const size_t rem = len & 3;
        len >>= 2;

        /* Main loop */
        for (;len > 0; len--) {
            hash  += get16bits (data);
            const uint32_t tmp    = (get16bits (data+2) << 11) ^ hash;
            hash   = (hash << 16) ^ tmp;
            data  += 2*sizeof(uint16_t);
            hash  += hash >> 11;
        }

        /* Handle end cases */
        switch (rem) {
            case 3: hash += get16bits (data);
                hash ^= hash << 16;
                hash ^= ((int8_t)data[sizeof (uint16_t)]) << 18;
                hash += hash >> 11;
                break;
            case 2: hash += get16bits (data);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;
            case 1: hash += (int8_t)*data;
                hash ^= hash << 10;
                hash += hash >> 1;
        }

        /* Force "avalanching" of final 127 bits */
        hash ^= hash << 3;
        hash += hash >> 5;
        hash ^= hash << 4;
        hash += hash >> 17;
        hash ^= hash << 25;
        hash += hash >> 6;

        return hash;
    }

    uint32_t SFH32::Of(const void *buf, size_t len) noexcept
    {
        return SFH32::Run(uint32_t(len),buf,len);
    }
}
