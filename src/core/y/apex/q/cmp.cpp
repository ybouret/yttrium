#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        bool operator==(const Rational &a, const Rational &b) noexcept
        {
            return a.numer == b.numer && a.denom == b.denom;
        }

    }

}

namespace Yttrium
{
    namespace Apex
    {
        bool operator!=(const Rational &a, const Rational &b) noexcept
        {
            return a.numer != b.numer || a.denom != b.denom;
        }

    }

}
