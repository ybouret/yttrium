#include "y/kemp/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        const char * const Rational:: CallSign = "apq";

        const char * Rational:: callSign() const noexcept { return CallSign; }

        void   Rational:: validate()
        {
            Natural::Simplify( Coerce(numer.n), Coerce(denom) );
        }

        Rational:: ~Rational() noexcept
        {
        }

        Rational:: Rational() : Number(), numer(0), denom(1) {}

        Rational:: Rational(const size_t nbits, const size_t dbits, Random::Bits &ran) :
        Number(),
        numer(nbits,ran),
        denom(dbits,ran)
        {
            validate();
        }



        Rational:: Rational(const Rational &q) : Number(),
        numer(q.numer),
        denom(q.denom)
        {
        }

        Rational & Rational:: operator=(const Rational &q)
        {
            Rational _(q); xch(_); return *this;
        }


        Rational:: Rational(const SignType snum, const Natural &anum, const Natural &den) :
        Number(),
        numer(snum,anum),
        denom(den)
        {
            validate();
        }

        Rational & Rational:: ldz() noexcept
        {
            Coerce(numer).ldz();
            Coerce(denom).ld1();
            return *this;
        }


        Rational & Rational:: ld1() noexcept
        {
            Coerce(numer).ld1();
            Coerce(denom).ld1();
            return *this;
        }



        void Rational:: xch(Rational &q) noexcept
        {
            Coerce(numer).xch( Coerce(q.numer) );
            Coerce(denom).xch( Coerce(q.denom) );

        }

        Rational & Rational:: neg() noexcept
        {
            Sign::ReplaceByOpposite( Coerce(numer.s) );
            return *this;
        }


        String Rational:: toString() const
        {
            String res = numer.toString();
            if(denom>1)
                res += '/' + denom.toDec();
            return res;
        }

        std::ostream & operator<<(std::ostream &os, const Rational &q)
        {
            os << q.toString();
            return os;
        }

      
        Rational Rational:: operator+() const
        {
            return *this;
        }

        Rational Rational:: operator-() const
        {
            Rational q = *this;
            q.neg();
            return q;
        }

        Integer & Integer:: operator=(const Rational &q)
        {

            if( q.denom._not(1) ) throw Specific::Exception(CallSign,"assign a non integer rational");

            Integer temp = q.numer;
            xch(temp);

            return *this;

        }

    }

}


