
#include "y/jive/char.hpp"
#include "y/memory/exclusive/impl.hpp"

Y_Studio(Yttrium::Jive::Char,AtExit::MaximumLongevity-7,128);

namespace Yttrium
{
    namespace Jive
    {

        Y_EXCLUSIVE_IMPL(Char);

        Char:: ~Char() noexcept {}

        Char:: Char(const Context &ctx, const uint8_t chr) noexcept :
        Context(ctx),
        next(0),
        prev(0),
        data(chr)
        {
        }

        Char:: Char(const Char &other) noexcept :
        Context(other),
        next(0),
        prev(0),
        data(other.data)
        {
        }

        uint8_t & Char:: operator*() noexcept
        {
            return data;
        }

        const uint8_t & Char:: operator*() const noexcept
        {
            return data;
        }


    }

}
