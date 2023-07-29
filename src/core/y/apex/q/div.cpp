
#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        static const char DivBy0[] = "Division by zero";

        Rational   operator / (const Rational &a, const Rational &b)
        {
            if(b.numer.s == __Zero__) throw Specific::Exception(Rational::CallSign,DivBy0);
            if(a.numer.s == __Zero__) return Rational();

            const Integer N = a.numer * b.denom; assert(0!=N);
            const Integer D = a.denom * b.numer; assert(0!=D);
            Coerce(N.s) = Sign::Product(N.s,D.s);
            return Rational(N,D.n);
        }

    }

}
