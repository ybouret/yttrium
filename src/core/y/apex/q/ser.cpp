


#include "y/apex/rational.hpp"
#include "y/io/stream/output.hpp"
#include "y/io/stream/input.hpp"

namespace Yttrium
{

    namespace Apex
    {
        size_t Rational:: serialize(OutputStream &fp) const
        {
            size_t written = numer.serialize(fp);
            return written + denom.serialize(fp);
        }

        Rational Rational:: ReadFrom(InputStream &fp)
        {
            const Integer n = Integer::ReadFrom(fp);
            const Natural d = Natural::ReadFrom(fp);
            return Rational(n,d);
        }
    }

}


