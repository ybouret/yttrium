
#include "y/apex/rational.hpp"

namespace Yttrium
{
    namespace Apex
    {

        Rational   operator * (const Rational &a, const Rational &b)
        {
            if(Sign::ZeroIsFoundIn( Sign::MakePair(a.numer.s, b.numer.s)))
                return Rational(0);
            assert(a.numer!=0);
            assert(b.numer!=0);
            const Integer N = a.numer * b.numer;
            const Natural D = a.denom * b.denom;
            return Rational(N,D);
        }

        Rational   operator * (const Rational &a, const int64_t b)
        {
            if( Sign::ZeroIsFoundIn( Sign::MakePair(a.numer.s, Sign::Of(b))))
                return Rational(0);
            assert(a.numer!=0);
            assert(b!=0);
            const Integer N = a.numer  * b;
            return Rational(N,a.denom);
        }
        

    }

}

