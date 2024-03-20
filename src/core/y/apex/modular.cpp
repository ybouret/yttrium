
#include "y/apex/modular.hpp"

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
                {
                    const Natural prod = result * base;
                    result = prod % modulus;
                }
                exponent.shr();
                const Natural base2 = base.sqr();
                base = base2 % modulus;
            }
            return result;
        }
    }

}


