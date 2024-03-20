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
    std::cerr << "result = " << result << std::endl;

    const apn check = Apex::Modular::Exp(base, exponent, modulus);
    std::cerr << "check  = " << check << std::endl;


}
Y_UDONE()

