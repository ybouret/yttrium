
#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Rational Rational:: Abs(const Rational &q)
        {
            Rational a = q;
            Sign::MakeAbs( Coerce(a.numer.s) );
            return a;
        }
    }
    
}
