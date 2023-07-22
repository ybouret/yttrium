
#include "y/apex/rational.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Rational Rational:: operator-() const
        {
            Rational res = *this;
            Sign::Change( Coerce(res.numer.s) );
            return res;
        }
        
    }

}
