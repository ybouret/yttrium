#include "y/stream/io/char.hpp"
#include "y/memory/exclusive/impl.hpp"

Y_Studio(Yttrium::IO::Char,AtExit::MaximumLongevity-6,128);


namespace Yttrium
{
    namespace IO
    {
        Y_EXCLUSIVE_IMPL(Char);

        Char:: Char(const uint8_t c) noexcept : next(0), prev(0), data(c)
        {
        }

        Char:: ~Char() noexcept
        {
        }

        uint8_t &        Char::operator*()       noexcept { return data; }
        const  uint8_t & Char::operator*() const noexcept { return data; }

        Char:: Char(const Char &ch) noexcept : next(0), prev(0), data(ch.data) {}

    }

}
