#include "y/apex/modular.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(apex_modular)
{
    const apn base     = 4;
    const apn exponent = 13;
    const apn modulus  = 497;

    apn result = 1;
    for(apn i=exponent;i>0;--i)
    {
        result *= base;
    }
    result %= modulus;
    Y_CHECK(445==result);

    const apn check = Apex::Modular::Exp(base, exponent, modulus);
    Y_CHECK(result==check);


}
Y_UDONE()

