
#include "y/apex/modular.hpp"
#include "y/system/exception.hpp"
#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Natural Modular::Exp(const Natural &b,
                             const Natural &e,
                             const Natural &modulus)
        {
            Natural base     = b;
            Natural exponent = e;

            Natural result   = 1;
            base %= modulus;
            while(exponent.bytes()>0)
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

            while(newr.bytes()>0)
            {
                Natural::Div(quotient, remainder, r, newr);
               // const apn quotient = r/newr;
                {
                    const apz temp = t - quotient * newt;
                    t    = newt;
                    newt = temp;
                }
                {
                    //const apn _ = r - quotient * newr;
                    r    = newr;
                    newr = remainder;
                }
            }
            if(r>1)
                throw Specific::Exception("Modular::Inv","singular value");

            while(t.s != Positive) t += n;
            return t.n;
        }
    }

}


