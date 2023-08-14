
#include "y/associative/be-key.hpp"
#include <cstring>
#include "y/stream/output.hpp"

namespace Yttrium
{
    BE_Key:: ~BE_Key() noexcept
    {
        memset(code,0,sizeof(code));
    }

    BE_Key:: BE_Key(const BE_Key &other) noexcept :
    Memory::ReadOnlyBuffer(),
    size(other.size),
    code()
    {
        memcpy(code,other.code,sizeof(code));
    }

    const void * BE_Key:: ro_addr() const noexcept
    {
        return code;
    }

    size_t BE_Key:: measure() const noexcept
    {
        return size;
    }

    void BE_Key::setup(const uint8_t x) noexcept
    {
        code[0] = x;
    }

    void BE_Key::setup(const uint16_t x) noexcept
    {
        code[1] = uint8_t(x);
        code[0] = uint8_t(x>>8);
    }

    void BE_Key::setup(const uint32_t x) noexcept
    {
        code[3] = uint8_t(x);
        code[2] = uint8_t(x>>8);
        code[1] = uint8_t(x>>16);
        code[0] = uint8_t(x>>24);
    }


    void BE_Key::setup(const uint64_t x) noexcept
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


    std::ostream & operator<<(std::ostream &os, const BE_Key &key)
    {
        return key.displayHexadecimal(os);
    }


    OutputStream & operator<<(OutputStream &os, const BE_Key &key)
    {
        return key.displayHexadecimal(os);
    }
}

