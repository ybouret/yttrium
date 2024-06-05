#include "y/kemp/rational.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        size_t Rational:: serialize(OutputStream &os) const
        {
            size_t nw = numer.serialize(os);
            return nw + denom.serialize(os);
        }

    }

}

