
#include "y/associative/little-endian-key.hpp"
#include <cstring>
#include "y/stream/output.hpp"

namespace Yttrium
{
    Little_Endian_Key:: ~Little_Endian_Key() noexcept
    {
        memset(code,0,sizeof(code));
    }

    Little_Endian_Key:: Little_Endian_Key(const Little_Endian_Key &other) noexcept :
    Memory::ReadOnlyBuffer(),
    size(other.size),
    code()
    {
        memcpy(code,other.code,sizeof(code));
    }

    const void * Little_Endian_Key:: ro_addr() const noexcept
    {
        return &code[0];
    }

    size_t Little_Endian_Key:: measure() const noexcept
    {
        return size;
    }


    template <>
    UnsignedInt<1>::Type Little_Endian_Key:: get<1>() const noexcept
    {
        return code[0];
    }

    void Little_Endian_Key::setup(const uint8_t &x) noexcept
    {
        code[0] = x;
        assert(x==get<1>());
    }


    template <>
    UnsignedInt<2>::Type Little_Endian_Key:: get<2>() const noexcept
    {
        uint16_t b0 = code[1];
        uint16_t b1 = code[0];
        return b0 | (b1<<8);
    }

    void Little_Endian_Key::setup(const uint16_t &x) noexcept
    {
        code[1] = uint8_t(x);
        code[0] = uint8_t(x>>8);
        assert( x == get<2>() );
    }

    template <>
    UnsignedInt<4>::Type Little_Endian_Key:: get<4>() const noexcept
    {
        uint32_t b0 = code[3];
        uint32_t b1 = code[2];
        uint32_t b2 = code[1];
        uint32_t b3 = code[0];

        return b0 | (b1<<8) | (b2<<16) | (b3<<24);
    }

    void Little_Endian_Key::setup(const uint32_t &x) noexcept
    {
        code[3] = uint8_t(x);
        code[2] = uint8_t(x>>8);
        code[1] = uint8_t(x>>16);
        code[0] = uint8_t(x>>24);
        assert( x == get<4>() );
    }

    template <>
    UnsignedInt<8>::Type Little_Endian_Key:: get<8>() const noexcept
    {
        uint64_t b0 = code[7];
        uint64_t b1 = code[6];
        uint64_t b2 = code[5];
        uint64_t b3 = code[4];
        uint64_t b4 = code[3];
        uint64_t b5 = code[2];
        uint64_t b6 = code[1];
        uint64_t b7 = code[0];

        return b0 | (b1<<8) | (b2<<16) | (b3<<24) | (b4<<32) | (b5<<40) | (b6<<48) | (b7<<56);
    }


    void Little_Endian_Key::setup(const uint64_t &x) noexcept
    {
        code[7] = uint8_t(x);
        code[6] = uint8_t(x>>8);
        code[5] = uint8_t(x>>16);
        code[4] = uint8_t(x>>24);
        code[3] = uint8_t(x>>32);
        code[2] = uint8_t(x>>40);
        code[1] = uint8_t(x>>48);
        code[0] = uint8_t(x>>56);

    }




    std::ostream & operator<<(std::ostream &os, const Little_Endian_Key &key)
    {
        return key.displayHexadecimal(os);
    }


    OutputStream & operator<<(OutputStream &os, const Little_Endian_Key &key)
    {
        return key.displayHexadecimal(os);
    }
}

