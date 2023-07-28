
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

        Rational   operator * (const int64_t a, const Rational &b)
        {
            if( Sign::ZeroIsFoundIn( Sign::MakePair( Sign::Of(a),b.numer.s)))
                return Rational(0);
            assert(a!=0);
            assert(b.numer!=0);
            const Integer N = a * b.numer;
            return Rational(N,b.denom);
        }

        Rational   operator * (const Rational &a, const Integer &b)
        {
            if( Sign::ZeroIsFoundIn( Sign::MakePair(a.numer.s,b.s)))
                return Rational(0);
            assert(a.numer!=0);
            assert(b!=0);
            const Integer N = a.numer  * b;
            return Rational(N,a.denom);
        }

        Rational   operator * (const Integer &a, const Rational &b)
        {
            if( Sign::ZeroIsFoundIn( Sign::MakePair(a.s,b.numer.s)))
                return Rational(0);
            assert(a!=0);
            assert(b.numer!=0);
            const Integer N = a * b.numer;
            return Rational(N,b.denom);
        }

        Rational   operator * (const Rational &a, const Natural &b)
        {
            if( Sign::ZeroIsFoundIn( Sign::MakePair(a.numer.s,b.getSign())))
                return Rational(0);
            assert(a.numer!=0);
            assert(b!=0);
            const Integer N = a.numer  * b;
            return Rational(N,a.denom);
        }

        Rational   operator * (const Natural &a, const Rational &b)
        {
            if( Sign::ZeroIsFoundIn( Sign::MakePair(a.getSign(),b.numer.s)))
                return Rational(0);
            assert(a!=0);
            assert(b.numer!=0);
            const Integer N = a * b.numer;
            return Rational(N,b.denom);
        }

    }

}

