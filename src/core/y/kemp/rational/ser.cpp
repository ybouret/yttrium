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

        Rational Rational:: ReadFrom(InputStream &fp)
        {
            const Integer num = Integer::ReadFrom(fp);
            const Natural den = Natural::ReadFrom(fp);
            return Rational(num,den);
        }

    }

}

