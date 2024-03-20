#include "y/apex/modular.hpp"
#include "y/apex/integer.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace {

}

Y_UTEST(apex_modular)
{
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



    {
        const unsigned a = 2;
        const unsigned n = 97;
        //const unsigned b = inverse(a,n);
        //std::cerr << a << "^(-1) [" << n << "] = " << b << std::endl;

        const apn A = a;
        const apn N = n;
        const apn B = Apex::Modular::Inv(A,N);
        std::cerr << A << "^(-1) [" << N << "] = " << B << std::endl;

    }



}
Y_UDONE()

