#include "y/apex/rational.hpp"
#include "y/io/chars.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        const char * const Rational:: CallSign = "Apex::Rational";

        Rational:: Rational() :
        Number(),
        numer(),
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

        Rational:: Rational(const int64_t n, const uint64_t d) :
        Number(),
        numer(n),
        denom(d)
        {
            zDenom();
            update();
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


    }
}



