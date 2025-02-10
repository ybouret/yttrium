
#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace Apex
    {
        const char * const Rational:: CallSign = "Apex:: Rational";

        const char * Rational:: callSign() const noexcept { return CallSign; }


        Rational:: Rational(const Rational &q) :
        Number(),
        numer(q.numer),
        denom(q.denom)
        {

        }

        Rational:: ~Rational() noexcept
        {
        }


        Rational:: Rational(const integer_t n) :
        Number(),
        numer(n),
        denom(1)
        {
        }

        Rational & Rational:: xch(Rational &q)
        {
            Coerce(numer).xch( Coerce(q.numer) );
            Coerce(denom).xch( Coerce(q.denom) );
            return *this;
        }


        Rational & Rational:: operator=(const Rational &q)
        {
            { Rational _(q); xch(_); }
            return *this;
        }

        Rational & Rational:: operator=(const integer_t n)
        {
            { Rational _(n); xch(_); }
            return *this;
        }

        Rational:: Rational(const Natural &n) :
        Number(),
        numer(n),
        denom(1)
        {
        }

        std::ostream & operator<<(std::ostream &os, const Rational &q)
        {
            if(1==q.denom)
            {
                os << q.numer;
            }
            else
            {
                os << '(' << q.numer << '/' << q.denom << ')';
            }
            return os;
        }


        void Rational:: simplify()
        {
            if(denom->bits<=0) throw Specific::Exception(CallSign, "Zero Denominator");

            if(numer.s == __Zero__) {
                assert(0==numer);
                Coerce(denom) = 1;
                return;
            }

            assert(denom>0);
            assert(numer.n>0);

            const Natural g = Natural::GCD_(denom,numer.n);
            if(g->bits>1)
            {
                Natural N = numer.n/g; assert(N>0);
                Natural D = denom/g;   assert(D>0);
                N.xch( Coerce(numer.n) );
                D.xch( Coerce(denom)   );
            }

        }

        Rational:: Rational(const integer_t n, const natural_t d) :
        Number(),
        numer(n),
        denom(d)
        {
            simplify();
        }

        Rational:: Rational(const Integer  &n) :
        Number(),
        numer(n),
        denom(1)
        {
        }

        Rational::  Rational(const Integer &n, const Natural &d) :
        Number(),
        numer(n),
        denom(d)
        {
            simplify();
        }

        Rational::  Rational(const Natural &n, const Natural &d) :
        Number(),
        numer(n),
        denom(d)
        {
            simplify();
            assert(numer>=0);
        }

        Rational:: Rational(Random::Bits &ran, const size_t nbits, const size_t dbits) :
        Number(),
        numer(ran,nbits),
        denom(ran,dbits<=1 ? 1 : dbits)
        {
            simplify();
        }
    }


}


