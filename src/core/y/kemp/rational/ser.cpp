#include "y/kemp/rational.hpp"
#include "y/stream/io/variable-info.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        size_t Rational:: serialize(OutputStream &os) const
        {
            size_t nw = numer.serialize(os);
            return nw + denom.serialize(os);
        }

        Rational Rational:: ReadFrom(InputStream &fp, const char *name)
        {
            if(!name) name = CallSign;
            IO::VariableInfo<256> info;
            const Integer num = Integer::ReadFrom(fp, info("%s.numerator",name)   );
            const Natural den = Natural::ReadFrom(fp, info("%s.denominator",name) );
            return Rational(num,den);
        }

    }

}

