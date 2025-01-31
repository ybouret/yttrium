#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        bool operator==(const Rational &a, const Rational &b) noexcept
        {
            return (a.denom == b.denom) && (a.numer == b.numer);
        }

        bool operator==(const Rational &a, const integer_t b) noexcept
        {
            return 1 == a.denom && a.numer == b;
        }

        bool operator==(const integer_t a, const Rational &b) noexcept
        {
            return 1 == b.denom && a == b.numer;
        }

        bool operator==(const Rational &a, const Integer &b) noexcept
        {
            return 1 == a.denom && a.numer == b;
        }

        bool operator==(const Integer &a, const Rational &b) noexcept
        {
            return 1 == b.denom && a == b.numer;
        }

    }

}

namespace Yttrium
{
    namespace Apex
    {
        bool operator!=(const Rational &a, const Rational &b) noexcept
        {
            return (a.denom != b.denom) ||  (a.numer != b.numer) ;
        }

        bool operator!=(const Rational &a, const integer_t b) noexcept
        {
            return 1 != a.denom || a.numer != b;
        }

        bool operator!=(const integer_t a, const Rational &b) noexcept
        {
            return 1 != b.denom || a != b.numer;
        }

        bool operator!=(const Rational &a, const Integer &b) noexcept
        {
            return (1 != a.denom) || (a.numer != b);
        }

        bool operator!=(const Integer &a, const Rational &b) noexcept
        {
            return (1 != b.denom) || (a != b.numer);
        }
        
    }

}
