
#include "y/apex/modular.hpp"
#include "y/system/exception.hpp"
#include <iostream>

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
            return 0;
        }
    }

}


