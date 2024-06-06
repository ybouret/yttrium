#include "y/kemp/rational.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        Rational Rational:: Add(const Rational &lhs, const Rational &rhs)
        {
            const Integer num = lhs.numer * rhs.denom + rhs.numer * lhs.denom;
            const Natural den = rhs.denom * lhs.denom;
            return Rational(num,den);
        }
        
        Rational  Rational:: Add(const Rational &lhs, const Integer &rhs)
        {
            const Integer num = lhs.numer + rhs * lhs.denom;
            return Rational(num,lhs.denom);
        }

        Rational Rational:: Add(const Rational &lhs, const Natural &rhs)
        {
            const Integer num = lhs.numer + rhs * lhs.denom;
            return Rational(num,lhs.denom);
        }

        Rational Rational:: Add(const Rational &lhs, const int64_t  rhs)
        {
            const Integer r   = rhs;
            const Integer num = lhs.numer + r * lhs.denom;
            return Rational(num,lhs.denom);
        }


        Rational Rational:: Add(const Integer  &lhs, const Rational &rhs)
        {
            const Integer num = lhs * rhs.denom + rhs.numer;
            return Rational(num,rhs.denom);
        }

        Rational Rational:: Add(const Natural  &lhs, const Rational &rhs)
        {
            const Integer num = lhs * rhs.denom + rhs.numer;
            return Rational(num,rhs.denom);   
        }


        Rational Rational:: Add(const int64_t   lhs, const Rational &rhs)
        {
            const Integer l   = lhs;
            const Integer num = l * rhs.denom + rhs.numer;
            return Rational(num,rhs.denom);
        }


        void Rational:: incr()
        {
            const Integer num = numer + denom;
            Rational      tmp(num,denom);
            xch(tmp);
        }

        Rational & Rational:: operator++()
        {
            incr();
            return *this;
        }

        Rational Rational:: operator++(int)
        {
            const Rational temp(*this);
            incr();
            return temp;
        }

    }
}


