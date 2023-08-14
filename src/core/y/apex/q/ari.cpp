
#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Rational Rational:: Abs(const Rational &q)
        {
            Rational a = q;
            Sign::ReplaceByAbs( Coerce(a.numer.s) );
            return a;
        }

        Rational Rational:: abs() const
        {
            return Abs(*this);
        }


        Rational Rational:: Sqrt(const Rational &q)
        {
            const Integer u   = q.numer.sqrt();
            const Natural du2 = q.denom * u.n * u.n;
            Natural       v   = q.denom.sqrt();
            while(v*v*q.numer.n<du2) ++v;
            return Rational(u,v);
        }

        Rational Rational:: sqrt() const
        {
            return Sqrt(*this);
        }

    }
    
}
