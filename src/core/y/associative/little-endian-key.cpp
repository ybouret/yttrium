
#include "y/associative/little-endian-key.hpp"
#include "y/associative/little-endian.hpp"
#include "y/stream/output.hpp"
#include <cstring>

namespace Yttrium
{
    void LittleEndianKey:: clear() noexcept
    {
        memset(code,0,sizeof(code));

    }

    LittleEndianKey:: ~LittleEndianKey() noexcept
    {
        clear();
    }

    LittleEndianKey:: LittleEndianKey(const LittleEndianKey &other) noexcept :
    Memory::ReadOnlyBuffer(),
    size(other.size),
    code()
    {
        memcpy(code,other.code,sizeof(code));
    }

    const void * LittleEndianKey:: ro_addr() const noexcept
    {
        return &code[0];
    }

    size_t LittleEndianKey:: measure() const noexcept
    {
        return size;
    }


    template <>
    UnsignedInt<1>::Type LittleEndianKey:: get<1>() const noexcept
    {
        return code[0];
    }

    void LittleEndianKey::setup(const uint8_t &x) noexcept
    {
        LittleEndian::Make(code,x);
        assert(x==get<1>());
    }


    template <>
    UnsignedInt<2>::Type LittleEndianKey:: get<2>() const noexcept
    {
        const uint16_t b0 = code[1];
        const uint16_t b1 = code[0];
        return b0 | (b1<<8);
    }

    void LittleEndianKey::setup(const uint16_t &x) noexcept
    {
        LittleEndian::Make(code,x);
        assert( x == get<2>() );
    }

    template <>
    UnsignedInt<4>::Type LittleEndianKey:: get<4>() const noexcept
    {
        const uint32_t b0 = code[3];
        const uint32_t b1 = code[2];
        const uint32_t b2 = code[1];
        const uint32_t b3 = code[0];

        return b0 | (b1<<8) | (b2<<16) | (b3<<24);
    }

    void LittleEndianKey::setup(const uint32_t &x) noexcept
    {
        LittleEndian::Make(code,x);
        assert( x == get<4>() );
    }

    template <>
    UnsignedInt<8>::Type LittleEndianKey:: get<8>() const noexcept
    {
        const uint64_t b0 = code[7];
        const uint64_t b1 = code[6];
        const uint64_t b2 = code[5];
        const uint64_t b3 = code[4];
        const uint64_t b4 = code[3];
        const uint64_t b5 = code[2];
        const uint64_t b6 = code[1];
        const uint64_t b7 = code[0];

        return b0 | (b1<<8) | (b2<<16) | (b3<<24) | (b4<<32) | (b5<<40) | (b6<<48) | (b7<<56);
    }


    void LittleEndianKey::setup(const uint64_t &x) noexcept
    {
        LittleEndian::Make(code,x);
        assert( x == get<8>() );
    }




    std::ostream & operator<<(std::ostream &os, const LittleEndianKey &key)
    {
        return key.displayHexadecimal(os);
    }


    OutputStream & operator<<(OutputStream &os, const LittleEndianKey &key)
    {
        return key.displayHexadecimal(os);
    }

    bool operator==(const LittleEndianKey &lhs, const LittleEndianKey &rhs) noexcept
    {
        return 0 == memcmp(lhs.code,rhs.code,8);
    }

}

