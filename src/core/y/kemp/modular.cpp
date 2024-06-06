
#include "y/kemp/modular.hpp"
#include "y/kemp/integer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        Natural Modular::Exp(const Natural &b,
                             const Natural &e,
                             const Natural &modulus)
        {
            Natural base     = b;
            Natural exponent = e;

            Natural result   = 1;
            base %= modulus;
            while(exponent.bits()>0)
            {
                if(exponent.isOdd())
                    result = (result * base) % modulus;
                exponent.shr();
                base = base.sqr() % modulus;
            }
            return result;
        }
        



        Natural Modular::Inv(const Natural &a, const Natural &n)
        {
            apz t=0,newt=1;
            apn r=n,newr=a;
            apn quotient;
            apn remainder;

            while(newr.bits()>0)
            {
                Natural::Div(quotient, remainder, r, newr);
                {
                    apz _ = t - quotient * newt;
                    t.xch(newt);
                    newt.xch(_);
                }
                r.xch(newr);
                newr.xch(remainder);
            }

            if(r>1)
                throw Specific::Exception("Modular::Inv","singular value");

            while(t.s != Positive) t += n;
            return t.n;
        }
    }

}


