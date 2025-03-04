
#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        Natural & Natural:: shr() noexcept
        {
            assert(0!=code);
            (void) code->shr();
            return *this;
        }

        Natural & Natural:: shr(const size_t n)
        {
            make( code->shr(n) );
            return *this;
        }

        Natural & Natural:: shl(const size_t n)
        {
            make( code->shl(n) );
            return *this;
        }

        Natural & Natural:: operator<<=(const size_t n)
        {
            return shl(n);
        }

        Natural & Natural:: operator>>=(const size_t n)
        {
            return shr(n);
        }

        Natural operator<<(const Natural &N, const size_t n)
        {
            Natural _(N);
            return _.shl(n);
        }

        Natural operator>>(const Natural &N, const size_t n)
        {
            Natural _(N);
            return _.shr(n);
        }

    }

}
