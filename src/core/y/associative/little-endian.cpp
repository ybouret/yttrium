
#include "y/associative/little-endian.hpp"

namespace Yttrium
{

    void LittleEndian:: Make(uint8_t * const code, const uint8_t &x) noexcept
    {
        assert(0!=code);
        code[0] = x;
    }

    void LittleEndian:: Make(uint8_t * const code, const uint16_t &x) noexcept
    {
        assert(0!=code);
        code[1] = uint8_t(x);
        code[0] = uint8_t(x>>8);
    }

    void LittleEndian:: Make(uint8_t * const code, const uint32_t &x) noexcept
    {
        assert(0!=code);
        code[3] = uint8_t(x);
        code[2] = uint8_t(x>>8);
        code[1] = uint8_t(x>>16);
        code[0] = uint8_t(x>>24);
    }

    void LittleEndian:: Make(uint8_t * const code, const uint64_t &x) noexcept
    {
        assert(0!=code);
        code[7] = uint8_t(x);
        code[6] = uint8_t(x>>8);
        code[5] = uint8_t(x>>16);
        code[4] = uint8_t(x>>24);
        code[3] = uint8_t(x>>32);
        code[2] = uint8_t(x>>40);
        code[1] = uint8_t(x>>48);
        code[0] = uint8_t(x>>56);
    }

}

