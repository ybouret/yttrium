
#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        static const char DivBy0[] = "Division by zero";

        //----------------------------------------------------------------------
        Rational   operator / (const Rational &a, const Rational &b)
        {
            if(b.numer.s == __Zero__) throw Specific::Exception(Rational::CallSign,DivBy0);
            if(a.numer.s == __Zero__) return Rational();

            const Integer N = a.numer * b.denom; assert(0!=N);
            const Integer D = a.denom * b.numer; assert(0!=D);
            Coerce(N.s) = Sign::Product(N.s,D.s);
            return Rational(N,D.n);
        }

        //----------------------------------------------------------------------
        Rational   operator / (const Rational &a, const int64_t b)
        {
            const SignType s = Sign::Of(b);
            if(__Zero__ == s)         throw Specific::Exception(Rational::CallSign,DivBy0);
            if(__Zero__ == a.numer.s) return Rational();

            const uint64_t absb = b<0 ? -b : b;
            const Natural  D    = absb * a.denom;                      assert(0!=D);
            const Integer  N( Sign::Product(s,a.numer.s), a.numer.n ); assert(0!=N);

            return Rational(N,D);
        }

        Rational   operator / (const int64_t a, const Rational &b)
        {
            if(__Zero__ == b.numer.s) throw Specific::Exception(Rational::CallSign,DivBy0);
            const SignType s = Sign::Of(a);
            if(__Zero__ == s ) return Rational();

            const uint64_t absa = a < 0 ? -a : a;
            const Natural  absN = absa * b.denom;
            const Integer  N( Sign::Product(s,b.numer.s), absN);
            return Rational(N,b.numer.n);
        }

        //----------------------------------------------------------------------
        Rational   operator / (const Rational &a, const Integer &b)
        {
            if( __Zero__ == b.s )      throw Specific::Exception(Rational::CallSign,DivBy0);
            if( __Zero__ == a.numer.s) return Rational();

            const Integer N( Sign::Product(a.numer.s,b.s), a.numer.n);
            const Natural D = a.denom * b.n;

            return Rational(N,D);
        }

        Rational   operator / (const Integer &a, const Rational &b)
        {
            if( __Zero__ == b.numer.s ) throw  Specific::Exception(Rational::CallSign,DivBy0);
            if( __Zero__ == a.s )       return Rational(0);

            const Natural AN = a.n * b.denom;
            const Integer N( Sign::Product(b.numer.s,a.s), AN);
            return Rational(N,b.numer.n);
        }

        //----------------------------------------------------------------------
        Rational   operator / (const Rational &a, const Natural &b)
        {
            if(b.bits()<=0)            throw  Specific::Exception(Rational::CallSign,DivBy0);
            if( __Zero__ == a.numer.s) return Rational();

            const Natural D = b * a.denom;
            return Rational(a.numer,D);
        }


       // Y_APQ_BINARY_IMPL(/)


    }

}
