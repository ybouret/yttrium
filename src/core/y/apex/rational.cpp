#include "y/apex/rational.hpp"
#include "y/stream/io/chars.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        const char * const Rational:: CallSign = "apq";

        Rational:: Rational() :
        Number(),
        numer(),
        denom(1)
        {
        }

        Rational:: Rational(const int64_t z) :
        Number(),
        numer(z),
        denom(1)
        {
        }

        Rational:: Rational(const Integer &z) :
        Number(),
        numer(z),
        denom(1)
        {
        }

        Rational:: Rational(const Natural &n) :
        Number(),
        numer(n),
        denom(1)
        {
        }

        void Rational:: zDenom() const
        {
            if(denom<=0) throw Specific::Exception(CallSign,"Zero Denominator");
        }
        
        void Rational:: update()
        {
            assert(denom>0);
            Natural::Simplify(Coerce(numer.n),Coerce(denom));
        }


#define Y_RATIONAL_CTOR(LHS,RHS)                \
Rational:: Rational(const LHS n, const RHS d) : \
Number(), numer(n), denom(d)                    \
{ zDenom(); update(); }

        Y_RATIONAL_CTOR(int64_t  , uint64_t )
        Y_RATIONAL_CTOR(int64_t  , Natural &)
        Y_RATIONAL_CTOR(Integer &, uint64_t )
        Y_RATIONAL_CTOR(Integer &, Natural &)


        Rational:: Rational(const size_t numerBits,const size_t denomBits, Random::Bits &ran) :
        Number(),
        numer(numerBits,ran),
        denom(denomBits,ran)
        {
            zDenom(); update();
        }

        Rational:: ~Rational() noexcept
        {
        }

        void Rational:: appendTo(IO::Chars &cache) const
        {
            if(denom>1)
            {
                cache << '(';
                numer.appendTo(cache);
                cache << '/';
                denom.appendDec(cache);
                cache << ')';
            }
            else
            {
                numer.appendTo(cache);
            }
        }

        std::ostream & operator<<(std::ostream &os, const Rational &q)
        {
            IO::Chars chars;
            q.appendTo(chars);
            Number::Display(os,chars);
            return os;
        }

        Rational:: Rational(const Rational &q) :
        Number(),
        numer(q.numer),
        denom(q.denom)
        {
        }

        void Rational:: xch(Rational &other) noexcept
        {
            Coerce(numer).xch( Coerce(other.numer) );
            Coerce(denom).xch( Coerce(other.denom) );
        }

        Rational & Rational::operator=(const Rational &q)
        {
            { Rational tmp(q); xch(tmp); }
            return *this;
        }


        Rational & Rational:: operator=(const int64_t z)
        {
            { Rational tmp(z); xch(tmp); }
            return *this;
        }

        Rational & Rational:: operator=(const Integer &z)
        {
            { Rational tmp(z); xch(tmp); }
            return *this;
        }

        Rational & Rational:: operator=(const Natural &z)
        {
            { Rational tmp(z); xch(tmp); }
            return *this;
        }


        void Rational:: zset() noexcept
        {
            static const uint64_t _1(1);
            Coerce(numer).zset();
            Coerce(denom) = _1;
        }


    }
}

namespace Yttrium
{
    namespace Apex
    {

        Integer:: Integer(const Rational &q) :
        s(q.numer.s),
        n(q.numer.n)
        {
            if(1!=q.denom) throw Specific::Exception(CallSign,"assign non integer rational!");
        }

        Integer & Integer:: operator=(const Rational &q)
        {
            Integer tmp(q);
            xch(tmp);
            return *this;
        }

    }
}



