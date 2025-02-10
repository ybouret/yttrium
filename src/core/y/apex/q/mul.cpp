#include "y/apex/rational.hpp"

namespace Yttrium
{
    namespace Apex
    {

        Rational Rational::Mul(const Rational &a, const Rational &b)
        {
            switch( Sign::MakePair( a.numer.s, b.numer.s) )
            {
                case ZN_Signs:
                case ZZ_Signs:
                case ZP_Signs:
                case NZ_Signs:
                case PZ_Signs:
                    return Rational(0);

                case NN_Signs:
                case PP_Signs:
                case NP_Signs:
                case PN_Signs:
                    break;
            }
            const Integer num = a.numer * b.numer; assert(0!=num);
            const Natural den = a.denom * b.denom;
            return Rational(num,den);
        }

    }

}

namespace Yttrium
{
    namespace Apex
    {

        Rational Rational:: Mul(const Rational &a, const Integer &b)
        {
            switch( Sign::MakePair( a.numer.s, b.s) )
            {
                case ZN_Signs:
                case ZZ_Signs:
                case ZP_Signs:
                case NZ_Signs:
                case PZ_Signs:
                    return Rational(0);

                case NN_Signs:
                case PP_Signs:
                case NP_Signs:
                case PN_Signs:
                    break;
            }
            const Integer num = a.numer * b; assert(0!=num);
            return Rational(num,a.denom);
        }

        Rational Rational:: Mul(const Integer &a, const Rational &b)
        {
            return Mul(b,a);
        }
    }

}
